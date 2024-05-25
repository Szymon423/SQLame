#!/bin/bash

# Domyślnie ustawienie trybu Release
BUILD_TYPE="Release"

# Enable testing
ENABLE_TESTS=false

# Build flags
BUILD_FLAGS=""

# Sprawdzenie, czy podano flagę -d
while getopts ":dt" opt; do
  case ${opt} in
    d )
      BUILD_TYPE="Debug"
      BUILD_FLAGS="${BUILD_FLAGS} -d"
      ;;
    t )
      ENABLE_TESTS=true
      BUILD_FLAGS="${BUILD_FLAGS} -t"
      ;;
    \? )
      echo "Nieprawidłowy argument, poprawne użycie:" 1>&2
      echo " > debug:   ./buildAll.sh -d" 1>&2
      echo " > release: ./buildAll.sh" 1>&2
      exit 1
      ;;
  esac
done

# Funkcja budująca program
build_program() {
    PROGRAM_NAME="$1"
    echo "Building $PROGRAM_NAME with flags $BUILD_FLAGS"
    cd "$PROGRAM_NAME"
    ./build.sh $BUILD_FLAGS || exit 1
    cd ..
}

# Lista programów do zbudowania
PROGRAMS=(
    "communication"
    "compression_encryption"
    "management"
    "transaction"
    "core"
)

# Budowanie każdego programu
for PROGRAM in "${PROGRAMS[@]}"; do
    build_program "$PROGRAM"
done

echo "All programs built successfully!"