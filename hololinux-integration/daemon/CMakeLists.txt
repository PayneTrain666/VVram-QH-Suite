cmake_minimum_required(VERSION 3.20)

project(holoram_daemon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(holoram_daemon
    src/main.cpp
    src/queue_client.cpp
    src/gpu_worker.cpp
    src/cpu_fallback.cpp
    src/stats_pusher.cpp
)

target_include_directories(holoram_daemon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/uapi)
