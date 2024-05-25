#!/bin/bash

# Domyślnie ustawienie trybu Release
BUILD_TYPE="Release"

# Enable testing
ENABLE_TESTS=false

# Sprawdzenie, czy podano flagę -d
while getopts ":dt" opt; do
  case ${opt} in
    d )
      BUILD_TYPE="Debug"
      ;;
    t )
      ENABLE_TESTS=true
      ;;
    \? )
      echo "Nieprawidłowy argument, poprawne użycie:" 1>&2
      echo " > debug:   ./build.sh -d" 1>&2
      echo " > release: ./build.sh" 1>&2
      exit 1
      ;;
  esac
done

echo "Building communication module in $BUILD_TYPE..."
# Tworzenie odpowiedniego katalogu build
BUILD_DIR="build/${BUILD_TYPE}"
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

# Uruchomienie CMake i kompilacja
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DENABLE_TESTS=${ENABLE_TESTS} ../..
make

# Kopiuj do katalogu docelowego
TARGET_DIRECTORY="build/$BUILD_TYPE/"
mkdir -p "../../../$TARGET_DIRECTORY"
cp "./src/libcommunication.so" "../../../$TARGET_DIRECTORY"

# Powrót do katalogu głównego
cd ../..
