git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg.exe install zlib:x64-windows blosc:x64-windows openexr:x64-windows tbb:x64-windows boost-iostreams:x64-windows boost-system:x64-windows boost-any:x64-windows boost-algorithm:x64-windows boost-uuid:x64-windows boost-interprocess:x64-windows
