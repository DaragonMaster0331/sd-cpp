// SD-Studio: native Windows desktop app for the stable-diffusion.cpp WebUI.
//
// Starts sd-server.exe hidden (GPU/CUDA engine when an NVIDIA driver is present, otherwise the CPU
// engine), bound to 127.0.0.1 only, and shows the same embedded WebUI in a WebView2 window: no
// browser and no console window. The engine is tied to the app with a job object, so it can never
// outlive it. The UI talks to the host through window.chrome.webview messages:
//   page -> host  "getEngine" | "setEngine:<auto|cuda|cpu>" | "prefs:<locale>|<theme>"
//   host -> page  {"type":"engine","mode":..,"active":..,"device":..,"cuda":bool,"driver":bool,"cudaEngine":bool}
//
// Settings: %LOCALAPPDATA%\SD-Studio\settings.ini   Engine log: %LOCALAPPDATA%\SD-Studio\logs\engine.log
// Test options: --capture=<png> [--capture-delay=<ms>] [--script=<js>] [--engine=cpu|cuda|auto] [--port=<n>]

#include <winsock2.h>  // before windows.h, which would otherwise pull in the old winsock.h
#include <ws2tcpip.h>
#include <windows.h>
#include <dwmapi.h>
#include <dxgi.h>
#include <intrin.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <winhttp.h>
#include <wrl.h>

#include <atomic>
#include <cwctype>
#include <iterator>
#include <string>
#include <thread>
#include <vector>

#include "WebView2.h"
#include "resource.h"

using Microsoft::WRL::Callback;
using Microsoft::WRL::ComPtr;

namespace {

constexpr UINT WM_APP_ENGINE_READY  = WM_APP + 1;
constexpr UINT WM_APP_ENGINE_FAILED = WM_APP + 2;
constexpr UINT_PTR TIMER_CAPTURE    = 1;
constexpr wchar_t kWindowClass[]    = L"SDStudioWindow";
constexpr wchar_t kAppName[]        = L"SD-Studio";
constexpr int kDefaultPort          = 17861;

// ---------------------------------------------------------------------------------------------
// Localised text for the host's own loading / error pages (the WebUI has its own dictionaries).

struct HostText {
    const wchar_t* lang;
    const wchar_t* loading;
    const wchar_t* starting_gpu;
    const wchar_t* starting_cpu;
    const wchar_t* failed;
    const wchar_t* missing;
    const wchar_t* log_file;
};

const HostText kTexts[] = {
    {L"en", L"Loading model…", L"Starting the GPU engine (CUDA)", L"Starting the CPU engine", L"The engine stopped unexpectedly.", L"Required files were not found.", L"Log file"},
    {L"ko", L"@@ @@@@ @…", L"GPU @@(CUDA) @@ @", L"CPU @@ @@ @", L"@@@ @@@ @@ @@@@@@@.", L"필@한 @@@ @@ @ @@@@.", L"@@ @@"},
    {L"ja", L"モデルを読み込み中…", L"GPU エンジン（CUDA）を起動中", L"CPU エンジンを起動中", L"エンジンが予期せず終了しました。", L"必要なファイルが見つかりません。", L"ログファイル"},
    {L"zh-CN", L"正在加载模型…", L"正在启动 GPU 引擎（CUDA）", L"正在启动 CPU 引擎", L"引擎意外停止。", L"找不到所需文件。", L"日志文件"},
    {L"zh-TW", L"正在載入模型…", L"正在啟動 GPU 引擎（CUDA）", L"正在啟動 CPU 引擎", L"引擎意外停止。", L"找不到所需的檔案。", L"記錄檔"},
    {L"es", L"Cargando modelo…", L"Iniciando el motor GPU (CUDA)", L"Iniciando el motor CPU", L"El motor se detuvo inesperadamente.", L"No se encontraron los archivos necesarios.", L"Archivo de registro"},
    {L"fr", L"Chargement du modèle…", L"Démarrage du moteur GPU (CUDA)", L"Démarrage du moteur CPU", L"Le moteur s'est arrêté de manière inattendue.", L"Fichiers requis introuvables.", L"Fichier journal"},
    {L"de", L"Modell wird geladen…", L"GPU-Engine (CUDA) wird gestartet", L"CPU-Engine wird gestartet", L"Die Engine wurde unerwartet beendet.", L"Benötigte Dateien wurden nicht gefunden.", L"Protokolldatei"},
    {L"it", L"Caricamento del modello…", L"Avvio del motore GPU (CUDA)", L"Avvio del motore CPU", L"Il motore si è arrestato in modo imprevisto.", L"File necessari non trovati.", L"File di log"},
    {L"pt-BR", L"Carregando modelo…", L"Iniciando o mecanismo de GPU (CUDA)", L"Iniciando o mecanismo de CPU", L"O mecanismo parou inesperadamente.", L"Arquivos necessários não encontrados.", L"Arquivo de log"},
    {L"ru", L"Загрузка модели…", L"Запуск GPU-движка (CUDA)", L"Запуск CPU-движка", L"Движок неожиданно остановился.", L"Не найдены необходимые файлы.", L"Файл журнала"},
    {L"vi", L"Đang tải mô hình…", L"Đang khởi động bộ xử lý GPU (CUDA)", L"Đang khởi động bộ xử lý CPU", L"Bộ xử lý đã dừng bất ngờ.", L"Không tìm thấy các tệp cần thiết.", L"Tệp nhật ký"},
};

// Same palettes as the WebUI's styles.css, for the host's pages, window background and title bar.
struct Palette {
    const wchar_t* id;
    COLORREF bg, panel, text, muted, primary, accent;
};

const Palette kPalettes[] = {
    {L"dracula", RGB(0x1e, 0x1f, 0x29), RGB(0x28, 0x2a, 0x36), RGB(0xf8, 0xf8, 0xf2), RGB(0xa4, 0xac, 0xd4), RGB(0xbd, 0x93, 0xf9), RGB(0xff, 0x79, 0xc6)},
    {L"one-dark", RGB(0x1b, 0x1d, 0x23), RGB(0x28, 0x2c, 0x34), RGB(0xd7, 0xda, 0xe0), RGB(0x9d, 0xa5, 0xb4), RGB(0x61, 0xaf, 0xef), RGB(0xc6, 0x78, 0xdd)},
    {L"github-dark", RGB(0x01, 0x04, 0x09), RGB(0x0d, 0x11, 0x17), RGB(0xe6, 0xed, 0xf3), RGB(0x91, 0x98, 0xa1), RGB(0x23, 0x86, 0x36), RGB(0x44, 0x93, 0xf8)},
    {L"tokyo-night", RGB(0x16, 0x16, 0x1e), RGB(0x1a, 0x1b, 0x26), RGB(0xc0, 0xca, 0xf5), RGB(0x9a, 0xa5, 0xce), RGB(0x7a, 0xa2, 0xf7), RGB(0xbb, 0x9a, 0xf7)},
    {L"catppuccin-mocha", RGB(0x11, 0x11, 0x1b), RGB(0x1e, 0x1e, 0x2e), RGB(0xcd, 0xd6, 0xf4), RGB(0xa6, 0xad, 0xc8), RGB(0xcb, 0xa6, 0xf7), RGB(0xf5, 0xc2, 0xe7)},
    {L"nord", RGB(0x24, 0x29, 0x33), RGB(0x2e, 0x34, 0x40), RGB(0xec, 0xef, 0xf4), RGB(0xae, 0xb7, 0xc7), RGB(0x88, 0xc0, 0xd0), RGB(0x81, 0xa1, 0xc1)},
    {L"monokai", RGB(0x1a, 0x1a, 0x17), RGB(0x27, 0x28, 0x22), RGB(0xf8, 0xf8, 0xf2), RGB(0xb1, 0xad, 0x97), RGB(0xa6, 0xe2, 0x2e), RGB(0xf9, 0x26, 0x72)},
    {L"vscode-dark", RGB(0x18, 0x18, 0x18), RGB(0x1f, 0x1f, 0x1f), RGB(0xcc, 0xcc, 0xcc), RGB(0x9d, 0x9d, 0x9d), RGB(0x00, 0x78, 0xd4), RGB(0x4d, 0xaa, 0xfc)},
};

// ---------------------------------------------------------------------------------------------

struct Options {
    std::wstring engine_override;  // auto / cuda / cpu, not saved
    std::wstring capture;          // PNG path: screenshot the UI once it has loaded, then exit
    int capture_delay_ms = 3000;
    std::wstring script;           // JavaScript file run in the UI before the capture
    int port = 0;
    bool devtools = false;
};

struct App {
    HWND hwnd = nullptr;
    HBRUSH background = nullptr;
    ComPtr<ICoreWebView2Controller> controller;
    ComPtr<ICoreWebView2> webview;
    Options opt;

    std::wstring exe_dir, data_dir, settings_path, log_path, webview_data;
    std::wstring cpu_engine, cuda_engine, models_dir;
    std::wstring diffusion_file, llm_file, vae_file;

    HANDLE job = nullptr;
    HANDLE server = nullptr;
    std::atomic<unsigned> generation{0};
    int port = kDefaultPort;
    std::wstring mode = L"auto";  // saved preference
    std::wstring active;          // engine actually running: cuda / cpu
    std::wstring device;          // GPU or CPU name shown in the UI
    bool driver = false;          // NVIDIA driver (nvcuda.dll) present
    std::wstring locale;          // last UI language reported by the page
    std::wstring theme = L"dracula";
    bool captured = false;
};

App g;

// --- small helpers ----------------------------------------------------------------------------

bool FileExists(const std::wstring& path) {
    DWORD attr = GetFileAttributesW(path.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

std::wstring FullPath(const std::wstring& path) {
    wchar_t buf[MAX_PATH * 4];
    DWORD n = GetFullPathNameW(path.c_str(), static_cast<DWORD>(std::size(buf)), buf, nullptr);
    return (n && n < std::size(buf)) ? std::wstring(buf) : path;
}

std::wstring ParentDir(const std::wstring& path) {
    size_t pos = path.find_last_of(L"\\/");
    return pos == std::wstring::npos ? path : path.substr(0, pos);
}

std::wstring IniRead(const wchar_t* section, const wchar_t* key, const std::wstring& fallback = L"") {
    wchar_t buf[2048];
    GetPrivateProfileStringW(section, key, fallback.c_str(), buf, static_cast<DWORD>(std::size(buf)), g.settings_path.c_str());
    return buf;
}

int IniInt(const wchar_t* section, const wchar_t* key, int fallback) {
    return static_cast<int>(GetPrivateProfileIntW(section, key, fallback, g.settings_path.c_str()));
}

void IniWrite(const wchar_t* section, const wchar_t* key, const std::wstring& value) {
    WritePrivateProfileStringW(section, key, value.c_str(), g.settings_path.c_str());
}

std::string ToUtf8(const std::wstring& text) {
    if (text.empty()) return {};
    int n = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);
    std::string out(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), out.data(), n, nullptr, nullptr);
    return out;
}

std::wstring FromUtf8(const std::string& text) {
    if (text.empty()) return {};
    int n = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
    std::wstring out(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), out.data(), n);
    return out;
}

std::wstring JsonEscape(const std::wstring& text) {
    std::wstring out;
    for (wchar_t ch : text) {
        switch (ch) {
            case L'"': out += L"\\\""; break;
            case L'\\': out += L"\\\\"; break;
            case L'\n': out += L"\\n"; break;
            case L'\r': out += L"\\r"; break;
            case L'\t': out += L"\\t"; break;
            default:
                if (ch < 0x20) { wchar_t buf[8]; swprintf_s(buf, L"\\u%04x", ch); out += buf; } else { out += ch; }
        }
    }
    return out;
}

std::wstring HtmlEscape(const std::wstring& text) {
    std::wstring out;
    for (wchar_t ch : text) {
        switch (ch) {
            case L'&': out += L"&amp;"; break;
            case L'<': out += L"&lt;"; break;
            case L'>': out += L"&gt;"; break;
            case L'"': out += L"&quot;"; break;
            default: out += ch;
        }
    }
    return out;
}

std::wstring Quote(const std::wstring& arg) { return L"\"" + arg + L"\""; }

std::wstring CssColor(COLORREF c) {
    wchar_t buf[16];
    swprintf_s(buf, L"#%02x%02x%02x", GetRValue(c), GetGValue(c), GetBValue(c));
    return buf;
}

const Palette& CurrentPalette() {
    for (const auto& p : kPalettes) {
        if (g.theme == p.id) return p;
    }
    return kPalettes[0];
}

// Maps a BCP-47 tag to one of the WebUI's 12 languages (same rules as the WebUI's i18n).
std::wstring MatchLocale(std::wstring tag) {
    for (auto& ch : tag) ch = static_cast<wchar_t>(std::towlower(ch));
    if (tag.empty()) return L"";
    for (const auto& t : kTexts) {
        std::wstring code = t.lang;
        for (auto& ch : code) ch = static_cast<wchar_t>(std::towlower(ch));
        if (tag == code) return t.lang;
    }
    if (tag.rfind(L"zh", 0) == 0) {
        bool traditional = tag.find(L"-tw") != std::wstring::npos || tag.find(L"-hk") != std::wstring::npos ||
                           tag.find(L"-mo") != std::wstring::npos || tag.find(L"-hant") != std::wstring::npos;
        return traditional ? L"zh-TW" : L"zh-CN";
    }
    std::wstring primary = tag.substr(0, tag.find(L'-'));
    for (const auto& t : kTexts) {
        std::wstring code = t.lang;
        std::wstring code_primary = code.substr(0, code.find(L'-'));
        for (auto& ch : code_primary) ch = static_cast<wchar_t>(std::towlower(ch));
        if (primary == code_primary) return t.lang;
    }
    return L"";
}

const HostText& Text() {
    std::wstring lang = g.locale;
    if (lang.empty()) {
        wchar_t name[LOCALE_NAME_MAX_LENGTH] = L"";
        GetUserDefaultLocaleName(name, LOCALE_NAME_MAX_LENGTH);
        lang = MatchLocale(name);
    }
    for (const auto& t : kTexts) {
        if (lang == t.lang) return t;
    }
    return kTexts[0];
}

// --- hardware -----------------------------------------------------------------------------------

bool NvidiaDriverPresent() {
    HMODULE lib = LoadLibraryExW(L"nvcuda.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!lib) return false;
    FreeLibrary(lib);
    return true;
}

std::wstring NvidiaGpuName() {
    ComPtr<IDXGIFactory1> factory;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) return L"";
    ComPtr<IDXGIAdapter1> adapter;
    for (UINT i = 0; factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i, adapter.Reset()) {
        DXGI_ADAPTER_DESC1 desc{};
        if (SUCCEEDED(adapter->GetDesc1(&desc)) && desc.VendorId == 0x10DE) return desc.Description;
    }
    return L"";
}

std::wstring CpuName() {
    int regs[4] = {};
    char brand[49] = {};
    __cpuid(regs, 0x80000000);
    if (static_cast<unsigned>(regs[0]) < 0x80000004) return L"CPU";
    for (int i = 0; i < 3; ++i) {
        __cpuid(regs, 0x80000002 + i);
        memcpy(brand + i * 16, regs, 16);
    }
    std::wstring name = FromUtf8(brand);
    size_t first = name.find_first_not_of(L' ');
    size_t last = name.find_last_not_of(L' ');
    return first == std::wstring::npos ? L"CPU" : name.substr(first, last - first + 1);
}

bool PortFree(int port) {
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) return true;
    BOOL exclusive = TRUE;
    setsockopt(s, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<const char*>(&exclusive), sizeof(exclusive));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<u_short>(port));
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    bool ok = bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0;
    closesocket(s);
    return ok;
}

bool HttpOk(int port, const wchar_t* path) {
    bool ok = false;
    HINTERNET session = WinHttpOpen(kAppName, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session) return false;
    WinHttpSetTimeouts(session, 1000, 1000, 2000, 3000);
    HINTERNET connect = WinHttpConnect(session, L"127.0.0.1", static_cast<INTERNET_PORT>(port), 0);
    HINTERNET request = connect ? WinHttpOpenRequest(connect, L"GET", path, nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0) : nullptr;
    if (request && WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
        WinHttpReceiveResponse(request, nullptr)) {
        DWORD status = 0, size = sizeof(status);
        WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &status, &size, WINHTTP_NO_HEADER_INDEX);
        ok = status == 200;
    }
    if (request) WinHttpCloseHandle(request);
    if (connect) WinHttpCloseHandle(connect);
    WinHttpCloseHandle(session);
    return ok;
}

std::wstring LogTail(size_t max_lines) {
    HANDLE file = CreateFileW(g.log_path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
    if (file == INVALID_HANDLE_VALUE) return L"";
    LARGE_INTEGER size{};
    GetFileSizeEx(file, &size);
    const LONGLONG chunk = 16 * 1024;
    LARGE_INTEGER start{};
    start.QuadPart = size.QuadPart > chunk ? size.QuadPart - chunk : 0;
    SetFilePointerEx(file, start, nullptr, FILE_BEGIN);
    std::string data(static_cast<size_t>(size.QuadPart - start.QuadPart), '\0');
    DWORD read = 0;
    ReadFile(file, data.data(), static_cast<DWORD>(data.size()), &read, nullptr);
    CloseHandle(file);
    data.resize(read);
    std::wstring text = FromUtf8(data);
    size_t pos = text.size(), lines = 0;
    while (pos > 0 && lines <= max_lines) {
        pos = text.rfind(L'\n', pos - 1);
        if (pos == std::wstring::npos) { pos = 0; break; }
        ++lines;
    }
    return text.substr(pos);
}

// --- pages --------------------------------------------------------------------------------------

std::wstring UiUrl() { return L"http://127.0.0.1:" + std::to_wstring(g.port) + L"/"; }

void ShowPage(const std::wstring& title, const std::wstring& detail, bool spinner, const std::wstring& extra_html = L"") {
    if (!g.webview) return;
    const Palette& p = CurrentPalette();
    std::wstring html =
        L"<!doctype html><html lang=\"" + std::wstring(Text().lang) + L"\"><head><meta charset=\"utf-8\"><style>"
        L"html,body{margin:0;height:100%;background:" + CssColor(p.bg) + L";color:" + CssColor(p.text) + L";"
        L"font-family:'Segoe UI Variable Text','Segoe UI','Malgun Gothic','Yu Gothic UI','Microsoft YaHei UI',sans-serif}"
        L".wrap{min-height:100%;display:grid;place-items:center}"
        L".card{text-align:center;max-width:760px;padding:32px}"
        L".mark{width:76px;height:76px;border-radius:20px;margin:0 auto 24px;display:grid;place-items:center;"
        L"background:" + CssColor(p.panel) + L";border:2px solid " + CssColor(p.primary) + L";font:800 30px 'Segoe UI',sans-serif;"
        L"color:" + CssColor(p.primary) + L"}"
        L"h1{font-size:22px;margin:0 0 10px;font-weight:650}"
        L"p{color:" + CssColor(p.muted) + L";margin:0 0 6px;line-height:1.5}"
        L".spin{width:30px;height:30px;margin:26px auto 0;border-radius:50%;border:3px solid " + CssColor(p.panel) + L";"
        L"border-top-color:" + CssColor(p.primary) + L";animation:s 1s linear infinite}"
        L"@keyframes s{to{transform:rotate(360deg)}}"
        L"pre{text-align:left;background:" + CssColor(p.panel) + L";color:" + CssColor(p.muted) + L";padding:14px;border-radius:12px;"
        L"max-height:300px;overflow:auto;font:12px 'Cascadia Mono',Consolas,monospace;white-space:pre-wrap;margin-top:18px}"
        // Explicit Latin monospace fonts: Korean/Japanese fallback fonts draw '\' as a currency sign.
        L"code{color:" + CssColor(p.accent) + L";font:13px 'Cascadia Mono',Consolas,monospace}"
        L".path{font-family:'Cascadia Mono',Consolas,monospace;font-size:13px}"
        L"</style></head><body><div class=\"wrap\"><div class=\"card\"><div class=\"mark\">SD</div>"
        L"<h1>" + HtmlEscape(title) + L"</h1><p>" + HtmlEscape(detail) + L"</p>" +
        (spinner ? L"<div class=\"spin\"></div>" : L"") + extra_html + L"</div></div></body></html>";
    g.webview->NavigateToString(html.c_str());
}

std::wstring EngineDetail() {
    const HostText& t = Text();
    std::wstring detail = (g.active == L"cuda" ? t.starting_gpu : t.starting_cpu);
    if (!g.device.empty()) detail += L" · " + g.device;
    return detail;
}

void ShowLoading() { ShowPage(Text().loading, EngineDetail(), true); }

// with_log: show the engine log (only useful when the engine itself failed).
void ShowError(const std::wstring& title, const std::wstring& detail, bool with_log) {
    std::wstring extra;
    if (with_log) {
        extra = L"<p style=\"margin-top:18px\">" + std::wstring(Text().log_file) + L": <code>" + HtmlEscape(g.log_path) + L"</code></p>";
        std::wstring tail = LogTail(30);
        if (!tail.empty()) extra += L"<pre>" + HtmlEscape(tail) + L"</pre>";
    }
    ShowPage(title, detail, false, extra);
    if (!g.opt.capture.empty() && !g.captured) {  // test mode: screenshot the error page too
        g.captured = true;
        SetTimer(g.hwnd, TIMER_CAPTURE, static_cast<UINT>(g.opt.capture_delay_ms), nullptr);
    }
}

void ApplyWindowTheme() {
    const Palette& p = CurrentPalette();
    BOOL dark = TRUE;
    DwmSetWindowAttribute(g.hwnd, 20 /* DWMWA_USE_IMMERSIVE_DARK_MODE */, &dark, sizeof(dark));
    COLORREF caption = p.bg, text = p.text;
    DwmSetWindowAttribute(g.hwnd, 35 /* DWMWA_CAPTION_COLOR, Windows 11 */, &caption, sizeof(caption));
    DwmSetWindowAttribute(g.hwnd, 36 /* DWMWA_TEXT_COLOR, Windows 11 */, &text, sizeof(text));
    if (g.controller) {
        ComPtr<ICoreWebView2Controller2> controller2;
        if (SUCCEEDED(g.controller.As(&controller2))) {
            controller2->put_DefaultBackgroundColor({255, GetRValue(p.bg), GetGValue(p.bg), GetBValue(p.bg)});
        }
    }
}

// --- engine -------------------------------------------------------------------------------------

bool CudaUsable() { return !g.cuda_engine.empty() && g.driver; }

void PostEngineInfo() {
    if (!g.webview) return;
    std::wstring json = L"{\"type\":\"engine\",\"mode\":\"" + g.mode + L"\",\"active\":\"" + g.active +
                        L"\",\"device\":\"" + JsonEscape(g.device) + L"\",\"cuda\":" + (CudaUsable() ? L"true" : L"false") +
                        L",\"driver\":" + (g.driver ? L"true" : L"false") +
                        L",\"cudaEngine\":" + (g.cuda_engine.empty() ? L"false" : L"true") + L"}";
    g.webview->PostWebMessageAsJson(json.c_str());
}

void StopEngine() {
    ++g.generation;  // the watcher thread of the old engine stops reporting
    if (g.server) {
        TerminateProcess(g.server, 0);
        WaitForSingleObject(g.server, 5000);
        CloseHandle(g.server);
        g.server = nullptr;
    }
}

int ChoosePort() {
    int preferred = g.opt.port > 0 ? g.opt.port : IniInt(L"app", L"port", kDefaultPort);
    for (int port = preferred; port < preferred + 50; ++port) {
        if (PortFree(port)) return port;
    }
    return preferred;
}

void WatchEngine(unsigned gen, HANDLE process, int port) {
    bool ready = false;
    for (;;) {
        if (WaitForSingleObject(process, ready ? 1000 : 400) == WAIT_OBJECT_0) {
            if (g.generation == gen) PostMessageW(g.hwnd, WM_APP_ENGINE_FAILED, gen, 0);
            break;
        }
        if (g.generation != gen) break;
        // auth/status is public and answers with or without sign-in (capabilities returns 401 when signed out).
        if (!ready && HttpOk(port, L"/sdcpp/v1/auth/status")) {
            ready = true;
            PostMessageW(g.hwnd, WM_APP_ENGINE_READY, gen, 0);
        }
    }
    CloseHandle(process);
}

void StartEngine() {
    const HostText& text = Text();
    std::wstring want = g.opt.engine_override.empty() ? g.mode : g.opt.engine_override;
    g.active = (want != L"cpu" && CudaUsable()) ? L"cuda" : L"cpu";
    const std::wstring& dir = g.active == L"cuda" ? g.cuda_engine : g.cpu_engine;
    if (g.active == L"cuda") {
        g.device = NvidiaGpuName();
        if (g.device.empty()) g.device = L"NVIDIA GPU";
    } else {
        g.device = CpuName();
    }

    std::wstring missing;
    if (dir.empty()) missing += L"engine\\" + g.active + L"\\sd-server.exe  ";
    if (g.models_dir.empty()) missing += L"models\\" + g.diffusion_file + L"  ";
    if (!missing.empty()) {
        ShowError(text.missing, missing, false);
        return;
    }

    ShowLoading();
    unsigned gen = ++g.generation;
    g.port = ChoosePort();
    int width = IniInt(L"defaults", L"width", g.active == L"cuda" ? 1024 : 512);
    int height = IniInt(L"defaults", L"height", g.active == L"cuda" ? 1024 : 512);
    int steps = IniInt(L"defaults", L"steps", 4);
    int threads = IniInt(L"engine", L"threads", 0);

    std::wstring exe = dir + L"\\sd-server.exe";
    std::wstring cmd = Quote(exe) +
                       L" --diffusion-model " + Quote(g.models_dir + L"\\" + g.diffusion_file) +
                       L" --llm " + Quote(g.models_dir + L"\\" + g.llm_file) +
                       L" --vae " + Quote(g.models_dir + L"\\" + g.vae_file) +
                       L" --cfg-scale 1.0 --steps " + std::to_wstring(steps) +
                       L" -W " + std::to_wstring(width) + L" -H " + std::to_wstring(height) +
                       L" --diffusion-fa --listen-ip 127.0.0.1 --listen-port " + std::to_wstring(g.port);
    if (threads > 0) cmd += L" -t " + std::to_wstring(threads);
    // User sign-in (accounts in %LOCALAPPDATA%\SD-Studio\users.json); [auth] enabled=0 turns it off.
    if (IniInt(L"auth", L"enabled", 1) != 0) cmd += L" --auth-file " + Quote(g.data_dir + L"\\users.json");
    std::wstring extra = IniRead(L"engine", g.active == L"cuda" ? L"cuda_args" : L"cpu_args");
    if (!extra.empty()) cmd += L" " + extra;

    SECURITY_ATTRIBUTES sa{sizeof(sa), nullptr, TRUE};
    HANDLE log = CreateFileW(g.log_path.c_str(), FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (log != INVALID_HANDLE_VALUE) {
        SYSTEMTIME now;
        GetLocalTime(&now);
        wchar_t stamp[64];
        swprintf_s(stamp, L"%04d-%02d-%02d %02d:%02d:%02d", now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond);
        std::string header = "\r\n===== " + ToUtf8(stamp) + " SD-Studio: " + ToUtf8(g.active) + " engine (" + ToUtf8(g.device) + ")\r\n" + ToUtf8(cmd) + "\r\n";
        DWORD written = 0;
        WriteFile(log, header.data(), static_cast<DWORD>(header.size()), &written, nullptr);
    }

    STARTUPINFOW si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = nullptr;
    si.hStdOutput = log;
    si.hStdError = log;
    PROCESS_INFORMATION pi{};
    std::vector<wchar_t> cmdline(cmd.begin(), cmd.end());
    cmdline.push_back(L'\0');
    BOOL started = CreateProcessW(exe.c_str(), cmdline.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW | CREATE_SUSPENDED,
                                  nullptr, dir.c_str(), &si, &pi);
    if (log != INVALID_HANDLE_VALUE) CloseHandle(log);
    if (!started) {
        ShowError(text.failed, L"CreateProcess failed (" + std::to_wstring(GetLastError()) + L"): " + exe, false);
        return;
    }
    AssignProcessToJobObject(g.job, pi.hProcess);
    ResumeThread(pi.hThread);
    CloseHandle(pi.hThread);
    g.server = pi.hProcess;

    HANDLE watch = nullptr;
    DuplicateHandle(GetCurrentProcess(), pi.hProcess, GetCurrentProcess(), &watch, SYNCHRONIZE, FALSE, 0);
    std::thread(WatchEngine, gen, watch, g.port).detach();
}

void RestartEngine() {
    StopEngine();
    StartEngine();
}

// --- WebView2 -----------------------------------------------------------------------------------

void ResizeWebView() {
    if (!g.controller) return;
    RECT rc;
    GetClientRect(g.hwnd, &rc);
    g.controller->put_Bounds(rc);
}

bool IsOurUrl(const std::wstring& uri) {
    return uri.rfind(L"http://127.0.0.1:" + std::to_wstring(g.port) + L"/", 0) == 0 || uri.rfind(L"data:", 0) == 0 ||
           uri.rfind(L"about:", 0) == 0;
}

void OnWebMessage(const std::wstring& message) {
    if (message == L"getEngine") {
        PostEngineInfo();
    } else if (message.rfind(L"setEngine:", 0) == 0) {
        std::wstring mode = message.substr(10);
        if (mode != L"auto" && mode != L"cuda" && mode != L"cpu") return;
        std::wstring before = g.active;
        g.mode = mode;
        IniWrite(L"engine", L"mode", mode);
        g.opt.engine_override.clear();
        std::wstring target = (mode != L"cpu" && CudaUsable()) ? L"cuda" : L"cpu";
        if (target != before) {
            RestartEngine();
        } else {
            PostEngineInfo();
        }
    } else if (message.rfind(L"prefs:", 0) == 0) {
        std::wstring prefs = message.substr(6);
        size_t bar = prefs.find(L'|');
        std::wstring locale = MatchLocale(prefs.substr(0, bar));
        std::wstring theme = bar == std::wstring::npos ? L"" : prefs.substr(bar + 1);
        if (!locale.empty() && locale != g.locale) {
            g.locale = locale;
            IniWrite(L"ui", L"locale", locale);
        }
        for (const auto& p : kPalettes) {
            if (theme == p.id && theme != g.theme) {
                g.theme = theme;
                IniWrite(L"ui", L"theme", theme);
                ApplyWindowTheme();
            }
        }
    }
}

std::wstring ReadTextFile(const std::wstring& path) {
    HANDLE file = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (file == INVALID_HANDLE_VALUE) return L"";
    LARGE_INTEGER size{};
    GetFileSizeEx(file, &size);
    std::string data(static_cast<size_t>(size.QuadPart), '\0');
    DWORD read = 0;
    ReadFile(file, data.data(), static_cast<DWORD>(data.size()), &read, nullptr);
    CloseHandle(file);
    data.resize(read);
    return FromUtf8(data);
}

void Capture() {
    ComPtr<IStream> stream;
    if (FAILED(SHCreateStreamOnFileEx(g.opt.capture.c_str(), STGM_CREATE | STGM_WRITE, FILE_ATTRIBUTE_NORMAL, TRUE, nullptr, &stream))) {
        PostMessageW(g.hwnd, WM_CLOSE, 0, 0);
        return;
    }
    g.webview->CapturePreview(COREWEBVIEW2_CAPTURE_PREVIEW_IMAGE_FORMAT_PNG, stream.Get(),
        Callback<ICoreWebView2CapturePreviewCompletedHandler>([stream](HRESULT) -> HRESULT {
            stream->Commit(STGC_DEFAULT);
            PostMessageW(g.hwnd, WM_CLOSE, 0, 0);
            return S_OK;
        }).Get());
}

void OnWebViewCreated(ICoreWebView2Controller* controller) {
    g.controller = controller;
    g.controller->get_CoreWebView2(&g.webview);
    ApplyWindowTheme();

    ComPtr<ICoreWebView2Settings> settings;
    g.webview->get_Settings(&settings);
    settings->put_AreDevToolsEnabled(g.opt.devtools ? TRUE : FALSE);
    settings->put_IsStatusBarEnabled(FALSE);
    settings->put_IsWebMessageEnabled(TRUE);
    settings->put_IsZoomControlEnabled(TRUE);
    ResizeWebView();

    // The UI keeps language/theme in localStorage, which is per origin (port). If the port ever has
    // to change, seed the new origin with the preferences the page reported last time.
    std::wstring seed = L"(function(){try{if(location.hostname!=='127.0.0.1')return;var s=localStorage;";
    if (!g.locale.empty()) seed += L"if(s.getItem('sdcpp-webui-locale')===null)s.setItem('sdcpp-webui-locale',JSON.stringify('" + g.locale + L"'));";
    seed += L"if(s.getItem('sdcpp-webui-theme')===null)s.setItem('sdcpp-webui-theme',JSON.stringify('" + g.theme + L"'));}catch(e){}})();";
    g.webview->AddScriptToExecuteOnDocumentCreated(seed.c_str(), nullptr);

    g.webview->add_WebMessageReceived(
        Callback<ICoreWebView2WebMessageReceivedEventHandler>([](ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
            LPWSTR message = nullptr;
            if (SUCCEEDED(args->TryGetWebMessageAsString(&message)) && message) {
                OnWebMessage(message);
                CoTaskMemFree(message);
            }
            return S_OK;
        }).Get(), nullptr);

    g.webview->add_DocumentTitleChanged(
        Callback<ICoreWebView2DocumentTitleChangedEventHandler>([](ICoreWebView2* sender, IUnknown*) -> HRESULT {
            LPWSTR title = nullptr;
            sender->get_DocumentTitle(&title);
            std::wstring text = kAppName;
            if (title && *title && wcscmp(title, L"about:blank") != 0 && wcsncmp(title, L"data:", 5) != 0) text = std::wstring(title) + L" — " + kAppName;
            SetWindowTextW(g.hwnd, text.c_str());
            CoTaskMemFree(title);
            return S_OK;
        }).Get(), nullptr);

    // Offline app: never navigate anywhere but the local engine, and never open extra windows.
    g.webview->add_NavigationStarting(
        Callback<ICoreWebView2NavigationStartingEventHandler>([](ICoreWebView2*, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
            LPWSTR uri = nullptr;
            args->get_Uri(&uri);
            if (uri && !IsOurUrl(uri)) args->put_Cancel(TRUE);
            CoTaskMemFree(uri);
            return S_OK;
        }).Get(), nullptr);
    g.webview->add_NewWindowRequested(
        Callback<ICoreWebView2NewWindowRequestedEventHandler>([](ICoreWebView2*, ICoreWebView2NewWindowRequestedEventArgs* args) -> HRESULT {
            args->put_Handled(TRUE);
            return S_OK;
        }).Get(), nullptr);

    g.webview->add_NavigationCompleted(
        Callback<ICoreWebView2NavigationCompletedEventHandler>([](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {
            BOOL success = FALSE;
            args->get_IsSuccess(&success);
            LPWSTR uri = nullptr;
            sender->get_Source(&uri);
            bool ui = success && uri && std::wstring(uri).rfind(UiUrl(), 0) == 0;
            CoTaskMemFree(uri);
            if (ui && !g.opt.capture.empty() && !g.captured) {
                g.captured = true;
                if (!g.opt.script.empty()) g.webview->ExecuteScript(ReadTextFile(g.opt.script).c_str(), nullptr);
                SetTimer(g.hwnd, TIMER_CAPTURE, static_cast<UINT>(g.opt.capture_delay_ms), nullptr);
            }
            return S_OK;
        }).Get(), nullptr);

    StartEngine();
}

void FailWebView2(HRESULT hr) {
    wchar_t message[1024];
    swprintf_s(message,
               L"SD-Studio needs the Microsoft Edge WebView2 Runtime (error 0x%08lX).\n\n"
               L"It is part of Windows 11 and of up-to-date Windows 10. On an offline PC without it, extract the "
               L"WebView2 \"Fixed Version\" runtime (x64) into:\n\n%s\\WebView2Runtime",
               static_cast<unsigned long>(hr), g.exe_dir.c_str());
    MessageBoxW(g.hwnd, message, kAppName, MB_ICONERROR | MB_OK);
    DestroyWindow(g.hwnd);
}

void CreateWebView() {
    std::wstring fixed = g.exe_dir + L"\\WebView2Runtime";
    const wchar_t* browser = FileExists(fixed + L"\\msedgewebview2.exe") ? fixed.c_str() : nullptr;
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(browser, g.webview_data.c_str(), nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
            if (FAILED(result) || !env) {
                FailWebView2(result);
                return S_OK;
            }
            env->CreateCoreWebView2Controller(g.hwnd,
                Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                    if (FAILED(result) || !controller) {
                        FailWebView2(result);
                        return S_OK;
                    }
                    OnWebViewCreated(controller);
                    return S_OK;
                }).Get());
            return S_OK;
        }).Get());
    if (FAILED(hr)) FailWebView2(hr);
}

// --- setup --------------------------------------------------------------------------------------

void ParseCommandLine() {
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    for (int i = 1; argv && i < argc; ++i) {
        std::wstring arg = argv[i];
        auto value = [&](const wchar_t* name) -> const wchar_t* {
            size_t n = wcslen(name);
            return arg.compare(0, n, name) == 0 ? arg.c_str() + n : nullptr;
        };
        if (const wchar_t* engine = value(L"--engine=")) g.opt.engine_override = engine;
        if (const wchar_t* capture = value(L"--capture=")) g.opt.capture = FullPath(capture);
        if (const wchar_t* delay = value(L"--capture-delay=")) g.opt.capture_delay_ms = _wtoi(delay);
        if (const wchar_t* script = value(L"--script=")) g.opt.script = FullPath(script);
        if (const wchar_t* port = value(L"--port=")) g.opt.port = _wtoi(port);
        if (arg == L"--devtools") g.opt.devtools = true;
    }
    LocalFree(argv);
}

void ResolvePaths() {
    wchar_t module[MAX_PATH * 4];
    GetModuleFileNameW(nullptr, module, static_cast<DWORD>(std::size(module)));
    g.exe_dir = ParentDir(module);

    PWSTR local = nullptr;
    SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &local);
    g.data_dir = std::wstring(local ? local : L".") + L"\\SD-Studio";
    CoTaskMemFree(local);
    SHCreateDirectoryExW(nullptr, (g.data_dir + L"\\logs").c_str(), nullptr);
    g.settings_path = g.data_dir + L"\\settings.ini";
    g.log_path = g.data_dir + L"\\logs\\engine.log";
    g.webview_data = g.data_dir + L"\\WebView2";

    auto first_with = [](std::initializer_list<std::wstring> dirs, const std::wstring& file) -> std::wstring {
        for (const auto& dir : dirs) {
            std::wstring full = FullPath(dir);
            if (FileExists(full + L"\\" + file)) return full;
        }
        return L"";
    };
    // Packaged layout: SD-Studio\engine\{cpu,cuda}, SD-Studio\models.
    // Development layout: dist\SD-Studio next to dist\bin and dist\bin-cuda, models two levels up.
    g.cpu_engine = first_with({g.exe_dir + L"\\engine\\cpu", g.exe_dir + L"\\..\\bin"}, L"sd-server.exe");
    g.cuda_engine = first_with({g.exe_dir + L"\\engine\\cuda", g.exe_dir + L"\\..\\bin-cuda"}, L"sd-server.exe");

    g.diffusion_file = IniRead(L"models", L"diffusion", L"diffusion_models\\flux-2-klein-4b-Q8_0.gguf");
    g.llm_file = IniRead(L"models", L"llm", L"text_encoders\\qwen_3_4b-Q8_0.gguf");
    g.vae_file = IniRead(L"models", L"vae", L"vae\\flux2-vae-F16.gguf");
    std::wstring configured = IniRead(L"models", L"dir");
    g.models_dir = configured.empty()
                       ? first_with({g.exe_dir + L"\\models", g.exe_dir + L"\\..\\models", g.exe_dir + L"\\..\\..\\models"}, g.diffusion_file)
                       : first_with({configured}, g.diffusion_file);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_SIZE:
            ResizeWebView();
            return 0;
        case WM_DPICHANGED: {
            const RECT* rc = reinterpret_cast<const RECT*>(lparam);
            SetWindowPos(hwnd, nullptr, rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top, SWP_NOZORDER | SWP_NOACTIVATE);
            return 0;
        }
        case WM_APP_ENGINE_READY:
            if (static_cast<unsigned>(wparam) == g.generation && g.webview) g.webview->Navigate(UiUrl().c_str());
            return 0;
        case WM_APP_ENGINE_FAILED:
            if (static_cast<unsigned>(wparam) == g.generation) {
                if (g.server) {
                    CloseHandle(g.server);
                    g.server = nullptr;
                }
                ShowError(Text().failed, EngineDetail(), true);
            }
            return 0;
        case WM_TIMER:
            if (wparam == TIMER_CAPTURE) {
                KillTimer(hwnd, TIMER_CAPTURE);
                Capture();
            }
            return 0;
        case WM_CLOSE: {
            WINDOWPLACEMENT wp{sizeof(wp)};
            if (g.opt.capture.empty() && GetWindowPlacement(hwnd, &wp)) {
                const RECT& r = wp.rcNormalPosition;
                IniWrite(L"window", L"x", std::to_wstring(r.left));
                IniWrite(L"window", L"y", std::to_wstring(r.top));
                IniWrite(L"window", L"width", std::to_wstring(r.right - r.left));
                IniWrite(L"window", L"height", std::to_wstring(r.bottom - r.top));
                IniWrite(L"window", L"maximized", wp.showCmd == SW_SHOWMAXIMIZED ? L"1" : L"0");
            }
            DestroyWindow(hwnd);
            return 0;
        }
        case WM_DESTROY:
            StopEngine();
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

}  // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    HANDLE single = CreateMutexW(nullptr, TRUE, L"Local\\SD-Studio-SingleInstance");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        if (HWND existing = FindWindowW(kWindowClass, nullptr)) {
            ShowWindow(existing, IsIconic(existing) ? SW_RESTORE : SW_SHOW);
            SetForegroundWindow(existing);
        }
        return 0;
    }

    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    ParseCommandLine();
    ResolvePaths();
    g.mode = IniRead(L"engine", L"mode", L"auto");
    if (g.mode != L"cuda" && g.mode != L"cpu") g.mode = L"auto";
    g.locale = MatchLocale(IniRead(L"ui", L"locale"));
    g.theme = IniRead(L"ui", L"theme", L"dracula");
    g.driver = NvidiaDriverPresent();

    // Kill the engine together with the app, even if the app crashes.
    g.job = CreateJobObjectW(nullptr, nullptr);
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits{};
    limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    SetInformationJobObject(g.job, JobObjectExtendedLimitInformation, &limits, sizeof(limits));

    const Palette& palette = CurrentPalette();
    g.background = CreateSolidBrush(palette.bg);
    WNDCLASSEXW wc{sizeof(wc)};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = instance;
    wc.hIcon = LoadIconW(instance, MAKEINTRESOURCEW(IDI_APP));
    wc.hIconSm = static_cast<HICON>(LoadImageW(instance, MAKEINTRESOURCEW(IDI_APP), IMAGE_ICON,
                                               GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0));
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = g.background;
    wc.lpszClassName = kWindowClass;
    RegisterClassExW(&wc);

    UINT dpi = GetDpiForSystem();
    int width = IniInt(L"window", L"width", MulDiv(1440, dpi, 96));
    int height = IniInt(L"window", L"height", MulDiv(940, dpi, 96));
    int x = IniInt(L"window", L"x", CW_USEDEFAULT);
    int y = IniInt(L"window", L"y", CW_USEDEFAULT);
    RECT saved{x, y, x + width, y + height};
    if (x == CW_USEDEFAULT || !MonitorFromRect(&saved, MONITOR_DEFAULTTONULL)) x = y = CW_USEDEFAULT;

    g.hwnd = CreateWindowExW(0, kWindowClass, kAppName, WS_OVERLAPPEDWINDOW, x, y, width, height, nullptr, nullptr, instance, nullptr);
    ApplyWindowTheme();
    bool maximized = g.opt.capture.empty() && IniInt(L"window", L"maximized", 0) == 1;
    ShowWindow(g.hwnd, maximized ? SW_SHOWMAXIMIZED : show);
    UpdateWindow(g.hwnd);

    CreateWebView();

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    StopEngine();
    g.webview.Reset();
    g.controller.Reset();
    CloseHandle(g.job);
    DeleteObject(g.background);
    WSACleanup();
    CoUninitialize();
    CloseHandle(single);
    return 0;
}
