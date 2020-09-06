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
# vst-host static library
#
file(GLOB_RECURSE SOURCES_STATIC
    src/static/*.h
    src/static/*.cpp
)
add_library(vst-host-static STATIC ${SOURCES_STATIC})
target_include_directories(vst-host-static PUBLIC 3rdparty/FST/fst 3rdparty/FST/src)
target_compile_features(vst-host-static PRIVATE cxx_std_17)

#
# vst-host command line executable
#
file(GLOB_RECURSE SOURCES_CLI
    src/cli/*.h
    src/cli/*.cpp
)
add_executable(vst-host-cli ${SOURCES_CLI})
target_link_libraries(vst-host-cli vst-host-static)
target_include_directories(vst-host-cli PUBLIC src/static)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT vst-host-cli)
target_compile_features(vst-host-cli PRIVATE cxx_std_17)

#
# vst-host GUI executable
#
file(GLOB_RECURSE SOURCES_GUI
    src/gui/*.h
    src/gui/*.cpp
)
add_executable(vst-host WIN32 ${SOURCES_GUI})
target_link_libraries(vst-host imgui vst-host-static)
target_include_directories(vst-host-cli PUBLIC src/static)
target_compile_features(vst-host PRIVATE cxx_std_17)
