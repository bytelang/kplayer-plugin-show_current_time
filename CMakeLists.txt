cmake_minimum_required(VERSION 3.2)
set(CMAKE_MACOSX_RPATH 1)
project(VideoShowCurrentTimePlugin)

set(CMAKE_CXX_STANDARD 14)

# system
if (CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(CMAKE_SYSTEM_DARWIN 1)
endif ()
if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(CMAKE_SYSTEM_LINUX 1)
endif ()

# check librarys
include(FindPkgConfig)
pkg_check_modules(SPDLOG_LIBCONF REQUIRED spdlog>=1.0)
pkg_check_modules(FFMPEG_LIBCONF REQUIRED libavcodec>=58.53.0 libavutil>=56.30.100 libavformat>=58.28.101 libavfilter>=7.56.101)

# include path
include_directories(./
        ${FFMPEG_LIBCONF_INCLUDE_DIRS}
        ${SPDLOG_LIBCONF_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/plugin-pkg)

# shared library
add_library(VideoShowCurrentTimePlugin SHARED KPVideoShowCurrentTimePlugin.cpp)

# compile flags
target_compile_options(VideoShowCurrentTimePlugin PRIVATE -fPIC -fvisibility=hidden)

# link flags
if (APPLE)
    set_target_properties(VideoShowCurrentTimePlugin PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif ()

