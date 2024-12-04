#!/bin/bash
systype=$(./systype.sh)
if [[ ${systype} = "msys2" ]]; then
  MSYS2_BIN_DIR="C:\\msys64\\usr\bin"
  MSYS2_MINGW64_BIN_DIR="C:\\msys64\\mingw64\\bin"
  ${MSYS2_BIN_DIR}/mkdir -p build
  ${MSYS2_MINGW64_BIN_DIR}/cmake -S ./avdev/unittests -G "MSYS Makefiles" -B ./build \
    --fresh -DCMAKE_BUILD_TYPE=Debug
  ${MSYS2_MINGW64_BIN_DIR}/cmake --build build
elif [[ ${systype} = "linux" ]]; then
  mkdir -p build
  cmake -S ./avdev/unittests -G "Unix Makefiles" -B ./build \
    --fresh -DCMAKE_BUILD_TYPE=Debug
  cmake --build build
else
  echo "Unsupported system type: ${systype}"
fi