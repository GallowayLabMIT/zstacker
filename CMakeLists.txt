cmake_minimum_required (VERSION 3.12)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)
vcpkg_install(${VCPKG_DEPENDENCIES})

add_subdirectory(openvdb)


project(zstacker CXX)

add_executable(zstacker)
target_link_libraries(zstacker PRIVATE openvdb tiff)
set(CMAKE_CXX_STANDARD 17)
