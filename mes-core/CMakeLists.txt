cmake_minimum_required(VERSION 3.16)
project(mes_core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(mes_core STATIC
    src/db_conn.cpp
    src/tx000.cpp
)

target_include_directories(mes_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/transactions
    ${CMAKE_CURRENT_SOURCE_DIR}/include/models
)

find_package(SQLite3 REQUIRED)
target_link_libraries(mes_core PUBLIC SQLite3::SQLite3)

# --- Quick scratch test executable ---
add_executable(test_db_conn tests/test_db_conn.cpp)
add_executable(test_db_pool tests/test_db_pool.cpp)
add_executable(test_tx000   tests/test_tx000.cpp)
target_link_libraries(test_db_conn PRIVATE mes_core)
target_link_libraries(test_db_pool PRIVATE mes_core)
target_link_libraries(test_tx000   PRIVATE mes_core)