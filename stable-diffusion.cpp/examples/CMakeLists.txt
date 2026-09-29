include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(cli)
add_subdirectory(server)

# SD-Studio desktop app (Windows only; needs the WebView2 SDK, see examples/desktop/CMakeLists.txt).
if(WIN32 AND SD_WEBVIEW2_DIR)
    add_subdirectory(desktop)
endif()
