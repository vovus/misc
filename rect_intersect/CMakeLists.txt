cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

set(3rd_DIR 3rd)
set (CMAKE_INSTALL_PREFIX install)

include_directories( ${3rd_DIR} src)

set (SOURCES src/main.cpp src/io_helper.cpp)

add_executable(test.exe ${SOURCES} test/test.cpp test/rects.json)
add_executable(main.exe ${SOURCES} src/app.cpp)

install(TARGETS test.exe main.exe DESTINATION . COMPONENT binaries)
install(FILES test/rects.json DESTINATION . COMPONENT config)