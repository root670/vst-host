cmake_minimum_required(VERSION 3.10)

project(vst-host)

#
# imgui static library
#
file(GLOB SOURCES_IMGUI
    3rdparty/imgui/*.h
    3rdparty/imgui/*.cpp
)

add_library(imgui STATIC ${SOURCES_IMGUI})

#
# vst-host executable
#
file(GLOB_RECURSE SOURCES_EXECUTABLE
    src/*.h
    src/*.cpp
)

add_executable(vst-host WIN32 ${SOURCES_EXECUTABLE})
target_link_libraries(vst-host imgui)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT vst-host)
