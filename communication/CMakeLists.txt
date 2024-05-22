cmake_minimum_required(VERSION 3.16)
project(communication LANGUAGES CXX)

# Ustawienie standardu C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dodanie katalogu źródłowego
add_subdirectory(src)

# Ustawienie katalogu wyjściowego dla bibliotek
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)