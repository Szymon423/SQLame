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
add_executable(runManagementTests ${TEST_SOURCES})

# Połączenie z biblioteką GTest
target_link_libraries(runManagementTests 
  gtest 
  gtest_main 
  management
)

target_include_directories(runManagementTests PUBLIC 
  ${CMAKE_SOURCE_DIR}/src
)

# Ustawienie flag kompilatora dla testów
target_compile_options(runManagementTests PRIVATE -Wall -Wextra -Wpedantic)

# Dodanie testów do CTest
include(GoogleTest)
gtest_discover_tests(runManagementTests)
