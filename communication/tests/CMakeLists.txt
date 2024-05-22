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
add_executable(runCommunicationTests ${TEST_SOURCES})

# Połączenie z biblioteką GTest
target_link_libraries(runCommunicationTests gtest gtest_main communication)

# Ustawienie flag kompilatora dla testów
target_compile_options(runCommunicationTests PRIVATE -Wall -Wextra -Wpedantic)

# Dodanie testów do CTest
include(GoogleTest)
gtest_discover_tests(runCommunicationTests)
