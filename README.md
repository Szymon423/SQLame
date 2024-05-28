# SQLame
That's my attempt to create most Lame, and propably slowest database.

## Idea
Main idea is to create database engine which allows to store data. 

> [!IMPORTANT] 
> WOW AMAZING!

But worse.

## Connection mechanism
I want it to provide TCP connection, by this I mean HTTPS, and rest API

## Querries
As name of this project suggest I want it to be `SQL'ish` - but worse. It won't be pure `SQL` syntax, but `SQL` in `JSON`.

```json
{
    "SELECT" : {
        "COLUMNS": ["id", "value", "valid"],
        "FROM": "my_table",
        "WHERE": {
            "id": {
                "in": [123, 456],
            }
        },
        "ORDER": {
            "BY": "id",
            "HOW": "ASC"
        }
    }
}
```

## Management
I want this engine to be managable from website. And here as always `Vue.js`.

## Data processing
I want data to be compressed, and encrypted.
 - Compression - Zstandard
 - Encryption - openssl/AES (Advanced Encryption Standard)


## File system

```plaintext
data/ - główny katalog zawierający wszystkie dane bazy danych.
│
├── metadata/ - katalog zawierający pliki metadanych.
│   ├── tables/ - katalog zawierający metadane dotyczące tabel.
│   │   └── table_name.meta - plik metadanych dla konkretnej tabeli.
│   └── indexes/ - katalog zawierający metadane dotyczące indeksów.
│       └── index_name.meta - plik metadanych dla konkretnego indeksu.
│
├── tables/ - katalog zawierający dane tabel.
│   └── table_name/ - katalog dla konkretnej tabeli.
│       ├── data.db - plik danych przechowujący rzeczywiste rekordy tabeli.
│       └── index_name.idx - plik indeksu dla konkretnej tabeli.
│
└── logs/ - katalog zawierający pliki logów transakcji.
    └── log_0001.log - plik logu transakcji.
```

## Dependencies
 - [nlohmann/json 3.11.3](https://github.com/nlohmann/json)
 - [spdlog 1.14.1](https://github.com/gabime/spdlog)
 - [POCO 1.13.1](https://pocoproject.org/)
 - [zstd](https://github.com/facebook/zstd)
 - [OpenSSL](https://github.com/openssl/openssl)