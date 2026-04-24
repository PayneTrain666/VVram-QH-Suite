cmake_minimum_required(VERSION 3.20)

project(holovram-runtime
    VERSION 0.0.1
    DESCRIPTION "Virtual video RAM runtime using HoloRAM QH backing"
    LANGUAGES CXX
)

option(HOLOVRAM_ENABLE_CUDA "Enable CUDA backend" OFF)
option(HOLOVRAM_ENABLE_VULKAN "Enable Vulkan sparse backend" OFF)
option(HOLOVRAM_ENABLE_D3D12 "Enable D3D12 tiled backend" OFF)
option(HOLOVRAM_BUILD_TESTS "Build tests" ON)
option(HOLOVRAM_BUILD_EXAMPLES "Build examples" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(holovram_runtime
    src/core/hv_context.cpp
    src/core/hv_resource_manager.cpp
    src/core/hv_tile_manager.cpp
    src/core/hv_residency_tracker.cpp
    src/core/hv_quality_policy.cpp
    src/cache/vram_cache.cpp
    src/cache/qh_backing_store.cpp
    src/cache/disk_backing_store.cpp
    src/cache/eviction_policy.cpp
    src/prefetch/frame_predictor.cpp
    src/prefetch/camera_predictor.cpp
    src/prefetch/mip_predictor.cpp
    src/prefetch/ai_layer_predictor.cpp
    src/prefetch/request_scheduler.cpp
    src/resources/texture_2d.cpp
    src/resources/texture_3d.cpp
    src/resources/buffer.cpp
    src/resources/meshlet_cache.cpp
    src/resources/embedding_cache.cpp
    src/resources/model_weight_cache.cpp
    src/resources/video_tile_cache.cpp
    src/resources/gaussian_splat_cache.cpp
    src/metrics/tile_faults.cpp
    src/metrics/residency_stats.cpp
    src/metrics/frame_latency.cpp
    src/metrics/quality_metrics.cpp
)

target_include_directories(holovram_runtime PUBLIC include)

if(HOLOVRAM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(HOLOVRAM_BUILD_EXAMPLES)
    add_executable(virtual_texture_demo examples/virtual_texture_demo.cpp)
    target_link_libraries(virtual_texture_demo PRIVATE holovram_runtime)
endif()
