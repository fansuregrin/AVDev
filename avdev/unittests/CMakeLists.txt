cmake_minimum_required(VERSION 3.25)
project(avdev_test)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_SOURCE_DIR ../)
set(ROOT_DIR ../../)

include_directories(${ROOT_DIR})

find_package(GTest REQUIRED)
find_package(PkgConfig REQUIRED)

if (WIN32)
  if (MSYS)
    set(ENV{PKG_CONFIG_PATH} "C:\\msys64\\mingw64\\lib\\ffmpeg4.4\\pkgconfig")
    set(CMAKE_FIND_LIBRARY_SUFFIXES .dll ${CMAKE_FIND_LIBRARY_SUFFIXES})
  endif()
elseif (LINUX)
  set(ENV{PKG_CONFIG_PATH} "/usr/lib/ffmpeg4.4/pkgconfig")
  set(CMAKE_FIND_LIBRARY_SUFFIXES .so ${CMAKE_FIND_LIBRARY_SUFFIXES})
elseif (UNIX)
  # TODO
  set(CMAKE_FIND_LIBRARY_SUFFIXES .so ${CMAKE_FIND_LIBRARY_SUFFIXES})
endif()

pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
  libavformat
  libavcodec
  libswresample
  libswscale
  libavutil
)

enable_testing()

add_executable(media_file_unittest
  media_file_unittest.cc
  ${LIB_SOURCE_DIR}/media_file.cc
  ${LIB_SOURCE_DIR}/stream_info.cc
)
target_link_libraries(media_file_unittest
  GTest::gtest_main
  PkgConfig::LIBAV
)

include(GoogleTest)
gtest_discover_tests(media_file_unittest)