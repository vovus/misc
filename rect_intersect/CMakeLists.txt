cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)

set(3rd_DIR 3rd)
set (CMAKE_INSTALL_PREFIX install)

include_directories( ${3rd_DIR} src)

set (SOURCES src/rect.cpp src/io_helper.cpp)
set (JSON_DATA test/sample3.json test/sample4.json test/sample6_32.json test/sample6_64.json test/sample7.json test/sample8.json)

add_executable(tests ${SOURCES} test/test.cpp ${JSON_DATA})
add_executable(main ${SOURCES} src/main.cpp)

install(TARGETS tests main DESTINATION . COMPONENT binaries)
install(FILES ${JSON_DATA} DESTINATION . COMPONENT config)