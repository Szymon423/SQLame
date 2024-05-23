# Zbieranie wszystkich plików źródłowych w katalogu src
file(GLOB SOURCES 
    *.cpp
    *.hpp
)

# Dodanie plików źródłowych do biblioteki
add_library(compression_encryption SHARED ${SOURCES})

# Ustawienie flag kompilatora
target_compile_options(compression_encryption PRIVATE -Wall -Wextra -Wpedantic)
