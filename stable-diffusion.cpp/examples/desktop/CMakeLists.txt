# SD-Studio: native Windows desktop app (WebView2) around sd-server's embedded WebUI.
# Needs the WebView2 SDK: pass -DSD_WEBVIEW2_DIR=<dir with include/WebView2.h and x64/WebView2Loader.dll.lib>.
set(TARGET sd-studio)

set(WEBVIEW2_LOADER "${SD_WEBVIEW2_DIR}/x64/WebView2Loader.dll")

add_executable(${TARGET} WIN32
    main.cpp
    app.rc
    app.manifest
)
set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "SD-Studio")
target_include_directories(${TARGET} PRIVATE "${SD_WEBVIEW2_DIR}/include")
target_compile_definitions(${TARGET} PRIVATE UNICODE _UNICODE NOMINMAX)
target_compile_features(${TARGET} PRIVATE cxx_std_17)
if(MSVC)
    target_compile_options(${TARGET} PRIVATE /utf-8 /W4 /permissive-)
endif()
target_link_libraries(${TARGET} PRIVATE
    "${SD_WEBVIEW2_DIR}/x64/WebView2Loader.dll.lib"
    dwmapi dxgi winhttp ws2_32 shlwapi shell32 ole32
)

# The loader must sit next to SD-Studio.exe.
add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${WEBVIEW2_LOADER}" "$<TARGET_FILE_DIR:${TARGET}>"
    VERBATIM
)

install(TARGETS ${TARGET} RUNTIME)
install(FILES "${WEBVIEW2_LOADER}" TYPE BIN)
