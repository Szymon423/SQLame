cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE /mnt/c/Projects/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ustawienie domyślnego typu budowania
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

project(communication LANGUAGES CXX)

find_package(Poco REQUIRED COMPONENTS Util Net NetSSL Crypto)

# Dodanie katalogu źródłowego
add_subdirectory(src)

# testy
if(ENABLE_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

# Ustawienie katalogu wyjściowego dla bibliotek
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)