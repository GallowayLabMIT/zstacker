cmake_minimum_required (VERSION 3.12)
include(ExternalProject)

project(zstacker_proj CXX)
set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    #include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/include)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib)
    set(IlmBase_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows)
    set(TBB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows)
    set(Blosc_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows)
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/packages/tbb_x64-windows/debug)

    # Ensure TBB debug builds get to the right place
    add_custom_target(copy-tbb-debug-bin ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/packages/tbb_x64-windows/debug/bin
            ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/bin)
    add_custom_target(copy-tbb-debug-lib ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/packages/tbb_x64-windows/debug/lib
            ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib)
endif()

add_subdirectory(openvdb)
add_subdirectory(libtiff)

if(WIN32)
    add_dependencies(openvdb_static copy-tbb-debug-bin copy-tbb-debug-lib)
endif()

add_executable(zstacker src/main.cpp)

target_link_libraries(zstacker PRIVATE openvdb tiff)
