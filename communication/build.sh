#!/bin/bash

# Tworzenie katalogu build, jeśli nie istnieje
mkdir -p build
cd build

# Uruchomienie CMake i kompilacja
cmake ..
make

# Powrót do katalogu głównego
cd ..
