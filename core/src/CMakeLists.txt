# Zbieranie wszystkich plików źródłowych w katalogu src
file(GLOB SOURCES 
    *.cpp
    *.hpp
)

# Dodanie plików źródłowych do biblioteki
add_library(core_lib SHARED ${SOURCES})

# Ustawienie flag kompilatora
target_compile_options(core_lib PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(core_lib
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

target_include_directories(core_lib PRIVATE
    ${CMAKE_SOURCE_DIR}/../communication/src
)