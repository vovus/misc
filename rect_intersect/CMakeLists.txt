cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

set(3rd_DIR ../3rd)
set(INSTSLL_DIR install)

include_directories( ${3rd_DIR} )

set (SOURCES main.cpp io_helper.cpp)

set (CMAKE_INSTALL_PREFIX install)

add_executable(test.exe ${SOURCES} test.cpp rects.json)
add_executable(main.exe ${SOURCES} app.cpp rects.json)

install(TARGETS test.exe main.exe DESTINATION . COMPONENT binaries)
install(FILES rects.json DESTINATION . COMPONENT config)