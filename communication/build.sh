#!/bin/bash

# Domyślnie ustawienie trybu Release
BUILD_TYPE="Release"

# Sprawdzenie, czy podano flagę -d
while getopts "d" opt; do
  case ${opt} in
    d )
      BUILD_TYPE="Debug"
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
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ../..
make

# Powrót do katalogu głównego
cd ../..
