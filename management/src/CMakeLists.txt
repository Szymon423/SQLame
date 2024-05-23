# Zbieranie wszystkich plików źródłowych w katalogu src
file(GLOB SOURCES 
    *.cpp
    *.hpp
)

# Dodanie plików źródłowych do biblioteki
add_library(management SHARED ${SOURCES})

# Ustawienie flag kompilatora
target_compile_options(management PRIVATE -Wall -Wextra -Wpedantic)
