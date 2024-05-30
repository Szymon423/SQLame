![SQLame logo](./img/logo.png)


# SQLame
That's my attempt to create most Lame, and propably slowest database.


## Idea
Main idea is to create database engine which allows to store data. 

> [!IMPORTANT] 
> But worse.


## Connection mechanism
I want it to provide TCP connection, by this I mean HTTP/HTTPS, and rest API


## Querries
As name of this project suggest I want it to be `SQL'ish` - but worse. It won't be pure `SQL` syntax, but `SQL` in `JSON`.

```json
{
    "SELECT": {
        "COLUMNS": ["id", "value", "valid"],
        "FROM": "my_table",
        "WHERE": {
            "id": {
                "IN": [123, 456],
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
data/ - the main directory containing all database data.
│
├── metadata/ - directory containing metadata files.
│   ├── tables/ - directory containing metadata about tables.
│   │ └── table_name.meta - metadata file for a specific table.
│   └── indexes/ - directory containing metadata about indexes.
│       └── index_name.meta - metadata file for a specific index.
│
├── tables/ - directory containing table data.
│   └── table_name/ - directory for a specific table.
│       ├── data.db - data file storing the actual table records.
│       └── index_name.idx - index file for a specific table.
│
└── logs/ - directory containing transaction log files.
    └── log_0001.log.
```


## Dependencies
 - [nlohmann/json 3.11.3](https://github.com/nlohmann/json)
 - [spdlog 1.14.1](https://github.com/gabime/spdlog)
 - [POCO 1.13.1](https://pocoproject.org/)
 - [zstd](https://github.com/facebook/zstd)
 - [OpenSSL](https://github.com/openssl/openssl)

### Install with vcpkg
```sh
vcpkg install nlohmann-json
vcpkg install spdlog
vcpkg install POCO
vcpkg install POCO[netssl]
```

> [!NOTE] 
> Project will compile without problems if `vcpkg` is inside of this same directory as `SQLame`

```plaintext
Projects\
├── SQLame\
├── vcpkg\
```