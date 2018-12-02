cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("tools/gate/cmake/HunterGate.cmake")

huntergate(
  URL "https://github.com/ruslo/hunter/archive/v0.23.34.tar.gz"
  SHA1 "70287b1ffa810ee4e952052a9adff9b4856d0d54"
)

project(Multithreads)
set(EXECUTABLE_NAME "Multithreads")
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
hunter_add_package(Boost COMPONENTS system log thread)
find_package(Boost CONFIG REQUIRED  system log thread)
find_package(GTest CONFIG REQUIRED)

add_executable(${EXECUTABLE_NAME} main.cpp)

target_include_directories(${EXECUTABLE_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third-party/Picosha2
)

target_link_libraries(${EXECUTABLE_NAME} GTest::main
        Boost::system
        Boost::log
        Boost::thread)

#target_link_libraries(${EXECUTABLE_NAME}
#        Boost::system
#       Boost::log)
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

#target_link_libraries(${EXECUTABLE_NAME} GTest::main)

enable_testing()
add_test(NAME unit_tests COMMAND ${EXECUTABLE_NAME})
