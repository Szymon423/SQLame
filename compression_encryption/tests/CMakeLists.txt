# Pobranie Google Test
include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# Pobranie zawartości
FetchContent_MakeAvailable(googletest)

# Zbieranie wszystkich plików testowych w katalogu tests/communication_tests
file(GLOB TEST_SOURCES *.cpp)

# Dodanie plików testowych do executable
add_executable(runCompressionEncryptionTests ${TEST_SOURCES})

# Połączenie z biblioteką GTest
target_link_libraries(runCompressionEncryptionTests 
  gtest 
  gtest_main 
  communication_encryption
)

target_include_directories(runCompressionEncryptionTests PUBLIC 
  ${CMAKE_SOURCE_DIR}/src
)

# Ustawienie flag kompilatora dla testów
target_compile_options(runCompressionEncryptionTests PRIVATE -Wall -Wextra -Wpedantic)

# Dodanie testów do CTest
include(GoogleTest)
gtest_discover_tests(runCompressionEncryptionTests)
