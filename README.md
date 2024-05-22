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
    "select" : {
        "what": ["id", "value", "valid"],
        "from": "my_table",
        "where": "id > 2"
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

```plantuml
@startuml
folder data {
  folder metadata {
    folder tables {
      file table_name.meta
    }
    folder indexes {
      file index_name.meta
    }
  }
  folder tables {
    folder table_name {
      file data.db
      file index_name.idx
    }
  }
  folder logs {
    file log_0001.log
  }
}
@enduml
