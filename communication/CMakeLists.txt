cmake_minimum_required(VERSION 3.16)
project(communication LANGUAGES CXX)

# Ustawienie standardu C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ustawienie domyślnego typu budowania
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Dodanie katalogu źródłowego
add_subdirectory(src)

# Dodanie katalogu testowego
enable_testing()
add_subdirectory(tests)

# Ustawienie katalogu wyjściowego dla bibliotek
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


