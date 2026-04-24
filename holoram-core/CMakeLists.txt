cmake_minimum_required(VERSION 3.20)

project(holoram-core
    VERSION 0.0.1
    DESCRIPTION "Quantum-holographic memory core library"
    LANGUAGES CXX
)

option(HOLO_ENABLE_CUDA "Enable CUDA substrate and kernels" OFF)
option(HOLO_BUILD_TESTS "Build tests" ON)
option(HOLO_BUILD_BENCHES "Build benchmarks" ON)
option(HOLO_BUILD_EXAMPLES "Build examples" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/HoloCompilerOptions.cmake)

add_library(holoram_core
    src/core.cpp
    src/codebook/cazac_zc.cpp
    src/codebook/mub_dft.cpp
    src/codebook/coherence.cpp
    src/codec/dense_codec.cpp
    src/codec/sparse_topk_codec.cpp
    src/codec/quant_int8.cpp
    src/codec/quant_int16.cpp
    src/codec/residual_codec.cpp
    src/demix/matched_filter.cpp
    src/demix/mmse.cpp
    src/demix/sic.cpp
    src/substrate/substrate_cpu.cpp
    src/substrate/slot_store.cpp
    src/substrate/slot_energy.cpp
    src/substrate/admission_policy.cpp
    src/writemap/writemap.cpp
    src/writemap/writemap_wal.cpp
    src/writemap/tile_map.cpp
    src/metrics/ber.cpp
    src/metrics/psnr.cpp
    src/metrics/ssim.cpp
    src/metrics/cosine.cpp
    src/metrics/timing.cpp
)

target_include_directories(holoram_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
holo_apply_common_options(holoram_core)

if(HOLO_ENABLE_CUDA)
    enable_language(CUDA)
    add_library(holoram_core_cuda
        cuda/codebook_upload.cu
        cuda/sparse_topk.cu
        cuda/sparse_write.cu
        cuda/mmse16.cu
        cuda/sic.cu
        cuda/spectral_shrink.cu
        cuda/substrate_kernels.cu
        cuda/batch_scheduler.cu
    )
    target_include_directories(holoram_core_cuda PUBLIC include)
    target_link_libraries(holoram_core_cuda PUBLIC holoram_core)
endif()

if(HOLO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(HOLO_BUILD_BENCHES)
    add_subdirectory(benches)
endif()

if(HOLO_BUILD_EXAMPLES)
    add_executable(qh_store_demo examples/qh_store_demo.cpp)
    target_link_libraries(qh_store_demo PRIVATE holoram_core)
    add_executable(sparse_density_sweep examples/sparse_density_sweep.cpp)
    target_link_libraries(sparse_density_sweep PRIVATE holoram_core)
endif()
