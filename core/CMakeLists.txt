cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE /mnt/c/Projects/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(COMMUNICATION_LIB   ${CMAKE_SOURCE_DIR}/../communication/build/${CMAKE_BUILD_TYPE}/src/libcommunication.so)

# Ustawienie domyślnego typu budowania
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Ustawienie ścieżki do toolchain vcpkg
project(compression_encryption LANGUAGES CXX)

# Znajdź pakiet nlohmann_json
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Poco REQUIRED COMPONENTS Util Net NetSSL Crypto)

# Dodanie katalogu źródłowego
add_subdirectory(src)

# testy
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Ustawienie katalogu wyjściowego dla bibliotek
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_executable(SQLame main.cpp)

message(${COMMUNICATION_LIB})

target_link_libraries(SQLame 
    core_lib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    Poco::Net
	Poco::NetSSL
	Poco::Crypto
	Poco::Util
    ${COMMUNICATION_LIB}
)

target_include_directories(SQLame PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/../communication/src
)