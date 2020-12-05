cmake_minimum_required (VERSION 3.12)
include(ExternalProject)

project(zstacker_proj CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/openvdb/cmake)


set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    ExternalProject_Add(
        openvdb_ext
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/ext/openvdb
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/openvdb
        BUILD_ALWAYS OFF
        CMAKE_ARGS
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
            -DVCPKG_TARGET_TRIPLET=x64-windows
            -DCMAKE_GENERATOR_PLATFORM=x64
            -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/ext/openvdb/install
        CMAKE_CACHE_ARGS
            OPENVDB_BUILD_BINRAIES=OFF
            OPENVDB_CORE_SHARED=OFF)
        CACHE STRING "Vcpkg toolchain file")
endif()
ExternalProject_Add(
    tiff_ext
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/ext/libtiff
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libtiff
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/ext/libtiff/install)


add_executable(zstacker src/main.cpp)
add_dependencies(zstacker openvdb_ext tiff_ext)

# Find TIFF and OpenVDB libraries
set(TIFF_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/ext/libtiff/install/lib)
set(TIFF_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/ext/libtiff/install/include)
find_package(TIFF)
find_package(OpenVDB)

target_link_libraries(zstacker PRIVATE OpenVDB::openvdb TIFF::TIFF)
