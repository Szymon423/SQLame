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
add_executable(runCoreTests ${TEST_SOURCES})

# Połączenie z biblioteką GTest
target_link_libraries(runCoreTests 
	gtest 
	gtest_main 
	core
)

target_include_directories(runCoreTests PUBLIC 
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/../communication/src
)

# Ustawienie flag kompilatora dla testów
target_compile_options(runCoreTests PRIVATE -Wall -Wextra -Wpedantic)

# Dodanie testów do CTest
include(GoogleTest)
gtest_discover_tests(runCoreTests)
