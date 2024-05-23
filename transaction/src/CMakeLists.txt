# Zbieranie wszystkich plików źródłowych w katalogu src
file(GLOB SOURCES 
    *.cpp
    *.hpp
)

# Dodanie plików źródłowych do biblioteki
add_library(transaction SHARED ${SOURCES})

# Ustawienie flag kompilatora
target_compile_options(transaction PRIVATE -Wall -Wextra -Wpedantic)
