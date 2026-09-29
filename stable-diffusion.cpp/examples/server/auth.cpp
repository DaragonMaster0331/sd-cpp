#include "auth.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <random>
#include <set>
#include <sstream>

#ifdef _WIN32
#include <bcrypt.h>
#endif

#include <json.hpp>
#include "common/log.h"

using json   = nlohmann::json;
namespace fs = std::filesystem;

namespace {

constexpr int kIterations          = 600000;  // OWASP 2023 recommendation for PBKDF2-HMAC-SHA256
constexpr int64_t kSessionSeconds  = 12 * 3600;
constexpr int64_t kRememberSeconds = 30 * 24 * 3600;
constexpr int kMaxFailures         = 5;
constexpr int64_t kLockSeconds     = 60;
constexpr const char* kCookie      = "sdcpp_session";

// --- SHA-256 / HMAC / PBKDF2 (FIPS 180-4, RFC 2104, RFC 8018) --------------------------------------

class Sha256 {
public:
    Sha256() {
        static const uint32_t init[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                                         0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
        memcpy(h_, init, sizeof(h_));
    }

    void update(const void* data, size_t len) {
        const uint8_t* p = static_cast<const uint8_t*>(data);
        total_ += len;
        while (len > 0) {
            size_t take = std::min<size_t>(64 - used_, len);
            memcpy(buf_ + used_, p, take);
            used_ += take;
            p += take;
            len -= take;
            if (used_ == 64) {
                compress(buf_);
                used_ = 0;
            }
        }
    }

    std::array<uint8_t, 32> finish() {
        uint64_t bits = total_ * 8;
        buf_[used_++] = 0x80;
        if (used_ > 56) {
            memset(buf_ + used_, 0, 64 - used_);
            compress(buf_);
            used_ = 0;
        }
        memset(buf_ + used_, 0, 56 - used_);
        for (int i = 0; i < 8; ++i) {
            buf_[56 + i] = static_cast<uint8_t>(bits >> (56 - 8 * i));
        }
        compress(buf_);
        std::array<uint8_t, 32> out{};
        for (int i = 0; i < 8; ++i) {
            out[4 * i]     = static_cast<uint8_t>(h_[i] >> 24);
            out[4 * i + 1] = static_cast<uint8_t>(h_[i] >> 16);
            out[4 * i + 2] = static_cast<uint8_t>(h_[i] >> 8);
            out[4 * i + 3] = static_cast<uint8_t>(h_[i]);
        }
        return out;
    }

private:
    static uint32_t rotr(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }

    void compress(const uint8_t* block) {
        static const uint32_t k[64] = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
        uint32_t w[64];
        for (int i = 0; i < 16; ++i) {
            w[i] = (uint32_t(block[4 * i]) << 24) | (uint32_t(block[4 * i + 1]) << 16) |
                   (uint32_t(block[4 * i + 2]) << 8) | uint32_t(block[4 * i + 3]);
        }
        for (int i = 16; i < 64; ++i) {
            uint32_t s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
            uint32_t s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i]        = w[i - 16] + s0 + w[i - 7] + s1;
        }
        uint32_t a = h_[0], b = h_[1], c = h_[2], d = h_[3], e = h_[4], f = h_[5], g = h_[6], h = h_[7];
        for (int i = 0; i < 64; ++i) {
            uint32_t t1 = h + (rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25)) + ((e & f) ^ (~e & g)) + k[i] + w[i];
            uint32_t t2 = (rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
            h           = g;
            g           = f;
            f           = e;
            e           = d + t1;
            d           = c;
            c           = b;
            b           = a;
            a           = t1 + t2;
        }
        h_[0] += a;
        h_[1] += b;
        h_[2] += c;
        h_[3] += d;
        h_[4] += e;
        h_[5] += f;
        h_[6] += g;
        h_[7] += h;
    }

    uint32_t h_[8];
    uint64_t total_ = 0;
    uint8_t buf_[64];
    size_t used_ = 0;
};

class HmacSha256 {
public:
    HmacSha256(const uint8_t* key, size_t key_len) {
        uint8_t k[64] = {0};
        if (key_len > 64) {
            Sha256 s;
            s.update(key, key_len);
            auto digest = s.finish();
            memcpy(k, digest.data(), digest.size());
        } else if (key_len > 0) {
            memcpy(k, key, key_len);
        }
        uint8_t ipad[64], opad[64];
        for (int i = 0; i < 64; ++i) {
            ipad[i] = k[i] ^ 0x36;
            opad[i] = k[i] ^ 0x5c;
        }
        inner_.update(ipad, 64);  // pre-absorbed pads: each MAC then costs two compressions
        outer_.update(opad, 64);
    }

    std::array<uint8_t, 32> mac(const void* data, size_t len) const {
        Sha256 in = inner_;
        in.update(data, len);
        auto inner_digest = in.finish();
        Sha256 out = outer_;
        out.update(inner_digest.data(), inner_digest.size());
        return out.finish();
    }

private:
    Sha256 inner_, outer_;
};

// --- encoding / misc -------------------------------------------------------------------------------

std::vector<uint8_t> random_bytes(size_t n) {
    std::vector<uint8_t> out(n);
#ifdef _WIN32
    if (BCryptGenRandom(nullptr, out.data(), static_cast<ULONG>(n), BCRYPT_USE_SYSTEM_PREFERRED_RNG) == 0) {
        return out;
    }
#endif
    std::random_device rd;
    for (auto& b : out) {
        b = static_cast<uint8_t>(rd());
    }
    return out;
}

std::string to_hex(const uint8_t* data, size_t n) {
    static const char digits[] = "0123456789abcdef";
    std::string out;
    out.reserve(n * 2);
    for (size_t i = 0; i < n; ++i) {
        out += digits[data[i] >> 4];
        out += digits[data[i] & 15];
    }
    return out;
}

std::string to_hex(const std::vector<uint8_t>& data) { return to_hex(data.data(), data.size()); }

std::vector<uint8_t> from_hex(const std::string& hex) {
    std::vector<uint8_t> out;
    if (hex.size() % 2) return out;
    auto nibble = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    };
    for (size_t i = 0; i < hex.size(); i += 2) {
        int hi = nibble(hex[i]), lo = nibble(hex[i + 1]);
        if (hi < 0 || lo < 0) return {};
        out.push_back(static_cast<uint8_t>(hi * 16 + lo));
    }
    return out;
}

std::string base64url(const uint8_t* data, size_t n) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    std::string out;
    for (size_t i = 0; i < n; i += 3) {
        uint32_t v = uint32_t(data[i]) << 16;
        if (i + 1 < n) v |= uint32_t(data[i + 1]) << 8;
        if (i + 2 < n) v |= uint32_t(data[i + 2]);
        out += table[(v >> 18) & 63];
        out += table[(v >> 12) & 63];
        if (i + 1 < n) out += table[(v >> 6) & 63];
        if (i + 2 < n) out += table[v & 63];
    }
    return out;
}

bool base64url_decode(const std::string& text, std::string& out) {
    out.clear();
    uint32_t buffer = 0;
    int bits        = 0;
    for (char c : text) {
        int v;
        if (c >= 'A' && c <= 'Z') v = c - 'A';
        else if (c >= 'a' && c <= 'z') v = c - 'a' + 26;
        else if (c >= '0' && c <= '9') v = c - '0' + 52;
        else if (c == '-') v = 62;
        else if (c == '_') v = 63;
        else return false;
        buffer = (buffer << 6) | static_cast<uint32_t>(v);
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            out += static_cast<char>((buffer >> bits) & 0xff);
        }
    }
    return true;
}

bool constant_time_equal(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    unsigned char diff = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        diff |= static_cast<unsigned char>(a[i] ^ b[i]);
    }
    return diff == 0;
}

std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

int64_t now_seconds() {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

bool valid_username(const std::string& name) {
    if (name.empty() || name.size() > 32) return false;
    return std::all_of(name.begin(), name.end(), [](unsigned char c) {
        return std::isalnum(c) || c == '.' || c == '_' || c == '-';
    });
}

bool valid_password(const std::string& password) { return password.size() >= 8 && password.size() <= 256; }

bool is_loopback(const std::string& addr) {
    return addr == "127.0.0.1" || addr == "::1" || addr == "::ffff:127.0.0.1" || addr.rfind("127.", 0) == 0;
}

std::string hash_password(const std::string& password, const std::vector<uint8_t>& salt, int iterations) {
    return to_hex(auth_pbkdf2_sha256(password, salt, iterations));
}

void send_json(httplib::Response& res, int status, const json& body) {
    res.status = status;
    res.set_content(body.dump(), "application/json");
}

void send_error(httplib::Response& res, int status, const char* code, json extra = json::object()) {
    extra["error"] = code;
    send_json(res, status, extra);
}

json user_json(const std::string& name, const std::string& role) { return {{"name", name}, {"role", role}}; }

bool parse_body(const httplib::Request& req, httplib::Response& res, json& body) {
    try {
        body = req.body.empty() ? json::object() : json::parse(req.body);
        if (!body.is_object()) throw std::runtime_error("not an object");
        return true;
    } catch (...) {
        send_error(res, 400, "invalid_json");
        return false;
    }
}

std::string json_string(const json& body, const char* key) {
    auto it = body.find(key);
    return (it != body.end() && it->is_string()) ? it->get<std::string>() : std::string();
}

}  // namespace

std::vector<uint8_t> auth_sha256(const std::string& data) {
    Sha256 s;
    s.update(data.data(), data.size());
    auto digest = s.finish();
    return {digest.begin(), digest.end()};
}

std::vector<uint8_t> auth_pbkdf2_sha256(const std::string& password, const std::vector<uint8_t>& salt, int iterations) {
    HmacSha256 prf(reinterpret_cast<const uint8_t*>(password.data()), password.size());
    std::vector<uint8_t> block(salt);
    block.insert(block.end(), {0, 0, 0, 1});  // INT(1): a single 32-byte output block
    auto u = prf.mac(block.data(), block.size());
    auto t = u;
    for (int i = 1; i < iterations; ++i) {
        u = prf.mac(u.data(), u.size());
        for (size_t j = 0; j < t.size(); ++j) {
            t[j] ^= u[j];
        }
    }
    return {t.begin(), t.end()};
}

// --- AuthManager -----------------------------------------------------------------------------------

bool AuthManager::open(const std::string& path, std::string& error) {
    std::lock_guard<std::mutex> lock(mutex_);
    path_    = path;
    enabled_ = true;
    bool dirty = false;
    try {
        if (fs::exists(path_)) {
            std::ifstream in(path_, std::ios::binary);
            json data = json::parse(in);
            key_      = from_hex(data.value("secret", ""));
            for (const auto& entry : data.value("users", json::array())) {
                AuthUser user;
                user.name          = entry.value("name", "");
                user.role          = entry.value("role", "user") == "admin" ? "admin" : "user";
                user.salt          = entry.value("salt", "");
                user.hash          = entry.value("hash", "");
                user.iterations    = entry.value("iterations", kIterations);
                user.token_version = entry.value("token_version", int64_t(1));
                user.created       = entry.value("created", int64_t(0));
                if (valid_username(user.name) && !user.hash.empty()) users_.push_back(user);
            }
        }
    } catch (const std::exception& e) {
        error = "cannot read auth file '" + path_ + "': " + e.what();
        return false;
    }
    if (key_.size() != 32) {
        key_  = random_bytes(32);
        dirty = true;
    }
    if ((dirty || !fs::exists(path_)) && !save_locked(error)) {
        return false;
    }
    LOG_INFO("authentication enabled: %zu user(s) in %s%s", users_.size(), path_.c_str(),
             users_.empty() ? " (first start: create the administrator in the WebUI)" : "");
    return true;
}

bool AuthManager::save_locked(std::string& error) {
    json users = json::array();
    for (const auto& u : users_) {
        users.push_back({{"name", u.name}, {"role", u.role}, {"salt", u.salt}, {"hash", u.hash},
                         {"iterations", u.iterations}, {"token_version", u.token_version}, {"created", u.created}});
    }
    json data = {{"version", 1}, {"secret", to_hex(key_)}, {"users", users}};
    try {
        fs::path target(path_);
        if (target.has_parent_path()) fs::create_directories(target.parent_path());
        fs::path tmp = target;
        tmp += ".tmp";
        {
            std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
            out << data.dump(2);
            if (!out) throw std::runtime_error("write failed");
        }
        fs::rename(tmp, target);  // replace atomically
    } catch (const std::exception& e) {
        error = "cannot write auth file '" + path_ + "': " + e.what();
        LOG_ERROR("%s", error.c_str());
        return false;
    }
    return true;
}

AuthUser* AuthManager::find_locked(const std::string& name) {
    std::string key = lower(name);
    for (auto& u : users_) {
        if (lower(u.name) == key) return &u;
    }
    return nullptr;
}

size_t AuthManager::admin_count_locked() const {
    return static_cast<size_t>(std::count_if(users_.begin(), users_.end(), [](const AuthUser& u) { return u.role == "admin"; }));
}

std::string AuthManager::issue_token_locked(const AuthUser& user, bool remember) const {
    json payload = {{"u", user.name}, {"v", user.token_version}, {"e", now_seconds() + (remember ? kRememberSeconds : kSessionSeconds)},
                    {"r", remember}};
    std::string text = payload.dump();
    std::string body = base64url(reinterpret_cast<const uint8_t*>(text.data()), text.size());
    auto sig         = HmacSha256(key_.data(), key_.size()).mac(body.data(), body.size());
    return body + "." + base64url(sig.data(), sig.size());
}

void AuthManager::set_session_cookie(httplib::Response& res, const std::string& token, bool remember) const {
    std::string cookie = std::string(kCookie) + "=" + token + "; Path=/; HttpOnly; SameSite=Strict";
    if (remember) cookie += "; Max-Age=" + std::to_string(kRememberSeconds);
    res.set_header("Set-Cookie", cookie);
}

AuthIdentity AuthManager::identify(const httplib::Request& req) {
    std::string token;
    std::string authorization = req.get_header_value("Authorization");
    if (authorization.rfind("Bearer ", 0) == 0) {
        token = authorization.substr(7);
    } else {
        std::string cookies = req.get_header_value("Cookie");
        std::string prefix  = std::string(kCookie) + "=";
        size_t pos          = 0;
        while (pos < cookies.size()) {
            size_t end       = cookies.find(';', pos);
            std::string item = cookies.substr(pos, end == std::string::npos ? std::string::npos : end - pos);
            item.erase(0, item.find_first_not_of(' '));
            if (item.rfind(prefix, 0) == 0) token = item.substr(prefix.size());
            if (end == std::string::npos) break;
            pos = end + 1;
        }
    }
    size_t dot = token.find('.');
    if (token.empty() || dot == std::string::npos) return {};

    std::string body = token.substr(0, dot);
    std::lock_guard<std::mutex> lock(mutex_);
    auto sig = HmacSha256(key_.data(), key_.size()).mac(body.data(), body.size());
    if (!constant_time_equal(token.substr(dot + 1), base64url(sig.data(), sig.size()))) return {};
    std::string text;
    if (!base64url_decode(body, text)) return {};
    try {
        json payload = json::parse(text);
        if (payload.value("e", int64_t(0)) < now_seconds()) return {};
        AuthUser* user = find_locked(payload.value("u", ""));
        if (!user || user->token_version != payload.value("v", int64_t(-1))) return {};
        return {user->name, user->role, payload.value("r", false)};
    } catch (...) {
        return {};
    }
}

bool AuthManager::may_access_job(const httplib::Request& req, const std::string& owner) {
    if (!enabled_) return true;
    AuthIdentity id = identify(req);
    return id.admin() || (!id.name.empty() && id.name == owner);
}

bool AuthManager::same_origin(const httplib::Request& req, const std::string& origin) const {
    std::string host = req.get_header_value("Host");
    return !host.empty() && (origin == "http://" + host || origin == "https://" + host);
}

httplib::Server::HandlerResponse AuthManager::pre_route(const httplib::Request& req, httplib::Response& res) {
    static const std::set<std::string> public_paths = {
        "/", "/sdcpp/v1/auth/status", "/sdcpp/v1/auth/login", "/sdcpp/v1/auth/setup", "/sdcpp/v1/auth/logout"};

    // CSRF: browsers send Origin on cross-site writes; only the server's own pages may change state.
    std::string origin = req.get_header_value("Origin");
    if (!origin.empty() && req.method != "GET" && req.method != "HEAD" && !same_origin(req, origin)) {
        send_error(res, 403, "cross_origin");
        return httplib::Server::HandlerResponse::Handled;
    }
    if (public_paths.count(req.path)) {
        return httplib::Server::HandlerResponse::Unhandled;
    }
    if (identify(req).name.empty()) {
        send_error(res, 401, "unauthorized");
        return httplib::Server::HandlerResponse::Handled;
    }
    return httplib::Server::HandlerResponse::Unhandled;
}

void AuthManager::register_endpoints(httplib::Server& svr) {
    svr.Get("/sdcpp/v1/auth/status", [this](const httplib::Request& req, httplib::Response& res) {
        AuthIdentity id = identify(req);
        bool setup_required;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            setup_required = users_.empty();
        }
        send_json(res, 200, {{"enabled", true}, {"setup_required", setup_required},
                             {"user", id.name.empty() ? json(nullptr) : user_json(id.name, id.role)}});
    });

    // First start: create the administrator. Only from the local machine, only while no user exists.
    svr.Post("/sdcpp/v1/auth/setup", [this](const httplib::Request& req, httplib::Response& res) {
        if (!is_loopback(req.remote_addr)) return send_error(res, 403, "setup_local_only");
        json body;
        if (!parse_body(req, res, body)) return;
        std::string name = json_string(body, "username"), password = json_string(body, "password");
        if (!valid_username(name)) return send_error(res, 400, "invalid_username");
        if (!valid_password(password)) return send_error(res, 400, "weak_password");

        AuthUser user;
        user.name       = name;
        user.role       = "admin";
        user.iterations = kIterations;
        auto salt       = random_bytes(16);
        user.salt       = to_hex(salt);
        user.hash       = hash_password(password, salt, user.iterations);
        user.created    = now_seconds();

        std::lock_guard<std::mutex> lock(mutex_);
        if (!users_.empty()) return send_error(res, 409, "setup_done");
        users_.push_back(user);
        std::string error;
        if (!save_locked(error)) {
            users_.clear();
            return send_error(res, 500, "save_failed");
        }
        LOG_INFO("auth: administrator '%s' created", name.c_str());
        set_session_cookie(res, issue_token_locked(user, true), true);
        send_json(res, 200, {{"user", user_json(user.name, user.role)}});
    });

    svr.Post("/sdcpp/v1/auth/login", [this](const httplib::Request& req, httplib::Response& res) {
        json body;
        if (!parse_body(req, res, body)) return;
        std::string name = json_string(body, "username"), password = json_string(body, "password");
        bool remember    = body.value("remember", false);
        std::string key  = lower(name);

        AuthUser user;
        bool found = false;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = failures_.find(key);
            if (it != failures_.end() && it->second.locked_until > now_seconds()) {
                return send_error(res, 429, "locked", {{"retry_after", it->second.locked_until - now_seconds()}});
            }
            if (AuthUser* u = find_locked(name)) {
                user  = *u;
                found = true;
            }
        }
        // Hash even for unknown users, so response time does not reveal which names exist.
        std::vector<uint8_t> salt = found ? from_hex(user.salt) : std::vector<uint8_t>(16, 0);
        std::string computed      = hash_password(password, salt, found ? user.iterations : kIterations);
        bool ok                   = found && constant_time_equal(computed, user.hash);

        std::lock_guard<std::mutex> lock(mutex_);
        if (!ok) {
            Failures& f = failures_[key];
            if (++f.count >= kMaxFailures) {
                f.count        = 0;
                f.locked_until = now_seconds() + kLockSeconds;
                LOG_WARN("auth: too many failed sign-ins for '%s', locked for %d s", name.c_str(), int(kLockSeconds));
            }
            return send_error(res, 401, "invalid_credentials");
        }
        failures_.erase(key);
        AuthUser* current = find_locked(user.name);
        if (!current) return send_error(res, 401, "invalid_credentials");
        set_session_cookie(res, issue_token_locked(*current, remember), remember);
        send_json(res, 200, {{"user", user_json(current->name, current->role)}});
    });

    svr.Post("/sdcpp/v1/auth/logout", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Set-Cookie", std::string(kCookie) + "=; Path=/; HttpOnly; SameSite=Strict; Max-Age=0");
        send_json(res, 200, {{"ok", true}});
    });

    // Change the signed-in user's own password; other sessions of that user end.
    svr.Post("/sdcpp/v1/auth/password", [this](const httplib::Request& req, httplib::Response& res) {
        AuthIdentity id = identify(req);
        if (id.name.empty()) return send_error(res, 401, "unauthorized");
        json body;
        if (!parse_body(req, res, body)) return;
        std::string current = json_string(body, "current"), password = json_string(body, "password");
        if (!valid_password(password)) return send_error(res, 400, "weak_password");

        AuthUser user;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            AuthUser* u = find_locked(id.name);
            if (!u) return send_error(res, 401, "unauthorized");
            user = *u;
        }
        if (!constant_time_equal(hash_password(current, from_hex(user.salt), user.iterations), user.hash)) {
            return send_error(res, 403, "wrong_password");
        }
        auto salt           = random_bytes(16);
        std::string hash    = hash_password(password, salt, kIterations);

        std::lock_guard<std::mutex> lock(mutex_);
        AuthUser* u = find_locked(id.name);
        if (!u) return send_error(res, 401, "unauthorized");
        u->salt       = to_hex(salt);
        u->hash       = hash;
        u->iterations = kIterations;
        u->token_version++;
        std::string error;
        if (!save_locked(error)) return send_error(res, 500, "save_failed");
        set_session_cookie(res, issue_token_locked(*u, id.remember), id.remember);
        send_json(res, 200, {{"ok", true}});
    });

    // --- administration ---
    auto require_admin = [this](const httplib::Request& req, httplib::Response& res) {
        AuthIdentity id = identify(req);
        if (id.name.empty()) {
            send_error(res, 401, "unauthorized");
        } else if (!id.admin()) {
            send_error(res, 403, "forbidden");
        }
        return id;
    };

    svr.Get("/sdcpp/v1/auth/users", [this, require_admin](const httplib::Request& req, httplib::Response& res) {
        if (!require_admin(req, res).admin()) return;
        json users = json::array();
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& u : users_) {
            users.push_back({{"name", u.name}, {"role", u.role}, {"created", u.created}});
        }
        send_json(res, 200, {{"users", users}});
    });

    svr.Post("/sdcpp/v1/auth/users", [this, require_admin](const httplib::Request& req, httplib::Response& res) {
        AuthIdentity admin = require_admin(req, res);
        if (!admin.admin()) return;
        json body;
        if (!parse_body(req, res, body)) return;
        std::string name = json_string(body, "username"), password = json_string(body, "password");
        std::string role = json_string(body, "role") == "admin" ? "admin" : "user";
        if (!valid_username(name)) return send_error(res, 400, "invalid_username");
        if (!valid_password(password)) return send_error(res, 400, "weak_password");
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (find_locked(name)) return send_error(res, 409, "user_exists");
        }
        AuthUser user;
        user.name       = name;
        user.role       = role;
        user.iterations = kIterations;
        auto salt       = random_bytes(16);
        user.salt       = to_hex(salt);
        user.hash       = hash_password(password, salt, user.iterations);
        user.created    = now_seconds();

        std::lock_guard<std::mutex> lock(mutex_);
        if (find_locked(name)) return send_error(res, 409, "user_exists");
        users_.push_back(user);
        std::string error;
        if (!save_locked(error)) {
            users_.pop_back();
            return send_error(res, 500, "save_failed");
        }
        LOG_INFO("auth: '%s' created %s '%s'", admin.name.c_str(), role.c_str(), name.c_str());
        send_json(res, 201, {{"user", user_json(user.name, user.role)}});
    });

    svr.Post(R"(/sdcpp/v1/auth/users/([A-Za-z0-9._\-]+)/password)", [this, require_admin](const httplib::Request& req, httplib::Response& res) {
        AuthIdentity admin = require_admin(req, res);
        if (!admin.admin()) return;
        json body;
        if (!parse_body(req, res, body)) return;
        std::string password = json_string(body, "password");
        if (!valid_password(password)) return send_error(res, 400, "weak_password");
        auto salt        = random_bytes(16);
        std::string hash = hash_password(password, salt, kIterations);

        std::lock_guard<std::mutex> lock(mutex_);
        AuthUser* u = find_locked(req.matches[1]);
        if (!u) return send_error(res, 404, "not_found");
        u->salt       = to_hex(salt);
        u->hash       = hash;
        u->iterations = kIterations;
        u->token_version++;             // ends that user's sessions
        failures_.erase(lower(u->name));  // an administrator reset also lifts a sign-in lockout
        std::string error;
        if (!save_locked(error)) return send_error(res, 500, "save_failed");
        LOG_INFO("auth: '%s' reset the password of '%s'", admin.name.c_str(), u->name.c_str());
        if (lower(u->name) == lower(admin.name)) set_session_cookie(res, issue_token_locked(*u, admin.remember), admin.remember);
        send_json(res, 200, {{"ok", true}});
    });

    svr.Delete(R"(/sdcpp/v1/auth/users/([A-Za-z0-9._\-]+))", [this, require_admin](const httplib::Request& req, httplib::Response& res) {
        AuthIdentity admin = require_admin(req, res);
        if (!admin.admin()) return;
        std::string name = req.matches[1];
        if (lower(name) == lower(admin.name)) return send_error(res, 400, "self_delete");

        std::lock_guard<std::mutex> lock(mutex_);
        AuthUser* u = find_locked(name);
        if (!u) return send_error(res, 404, "not_found");
        if (u->role == "admin" && admin_count_locked() <= 1) return send_error(res, 400, "last_admin");
        AuthUser removed = *u;
        users_.erase(std::remove_if(users_.begin(), users_.end(), [&](const AuthUser& x) { return lower(x.name) == lower(name); }),
                     users_.end());
        std::string error;
        if (!save_locked(error)) {
            users_.push_back(removed);
            return send_error(res, 500, "save_failed");
        }
        LOG_INFO("auth: '%s' deleted '%s'", admin.name.c_str(), removed.name.c_str());
        send_json(res, 200, {{"ok", true}});
    });
}
