# Zbieranie wszystkich plików źródłowych w katalogu src
file(GLOB SOURCES 
    *.cpp
    *.hpp
)

# Dodanie plików źródłowych do biblioteki
add_library(core SHARED ${SOURCES})

# Ustawienie flag kompilatora
target_compile_options(core PRIVATE -Wall -Wextra -Wpedantic)
