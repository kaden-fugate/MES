cmake_minimum_required(VERSION 3.15)
project(mes_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(mes_server src/main.cpp)
target_link_libraries(mes_server 
    ${Boost_LIBRARIES}
    ${SQLite3_LIBRARIES}
)