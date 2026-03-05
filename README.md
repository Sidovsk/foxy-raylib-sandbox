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
cd vcpkg
./bootstrap-vcpkg.sh
```
## 3. Configure the project and install dependencies
```bash
cmake -B build -G Ninja \
-DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

## 4. Build
```bash
cmake --build .
```

## 5. Run
```bash
./build/foxy-raylib-sandbox
```
