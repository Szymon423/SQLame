# Zbieranie wszystkich plików źródłowych w katalogu src
file(GLOB SOURCES 
    *.cpp
    *.hpp
)

# Dodanie plików źródłowych do biblioteki
add_library(communication SHARED ${SOURCES})

# Ustawienie flag kompilatora
target_compile_options(communication PRIVATE -Wall -Wextra -Wpedantic)

# Połącz bibliotekę z projektem
target_link_libraries(communication 
	Poco::Net
	Poco::NetSSL
	Poco::Crypto
	Poco::Util
	Poco::JWT
)