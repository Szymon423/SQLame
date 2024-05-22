# Zbieranie wszystkich plików źródłowych w katalogu src
file(GLOB SOURCES *.cpp)

# Dodanie plików źródłowych do biblioteki
add_library(communication SHARED ${SOURCES})

# Ustawienie flag kompilatora
target_compile_options(communication PRIVATE -Wall -Wextra -Wpedantic)
