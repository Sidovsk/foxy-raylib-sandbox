# foxy-raylib-sandbox
A sandbox project to experiment with Raylib and learn some basics graphics/game programming.


## Build Instructions

This project uses:

- C++
- CMake
- Ninja
- vcpkg
- Raylib

###  1. Install System Dependencies

On Ubuntu / Debian:

```bash
sudo apt install build-essential cmake ninja-build git
```

### 2. Install vcpkg

```bash
git clone https://github.com/microsoft/vcpkg.git
```
```bash
cd vcpkg
```
```bash
./bootstrap-vcpkg.sh
```
### 3. Configure the project and install dependencies
```bash
cmake -B build -G Ninja \
-DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

### 4. Build
```bash
cmake --build .
```

### 5. Run
```bash
./build/foxy-raylib-sandbox
```

### Extra
Cross compiling with x86_64-w64-mingw32-g++ 
```bash
cmake -B build-windows \ 
-G Ninja \
-DCMAKE_SYSTEM_NAME=Windows \
-DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
-DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake \
-DVCPKG_TARGET_TRIPLET=x64-mingw-static -DVCPKG_APPLOCAL_DEPS=OFF
```
```bash
cmake --build build-windows
```
