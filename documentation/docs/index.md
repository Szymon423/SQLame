# SQLame documentation

## Project description
Main idea is to create database engine which allows to store data. But worse.

## Syntax

* `CREATE` Command
```json
{
    "CREATE": {
        "TABLE": {
            "NAME": "my_table",
            "COLUMNS": [
                {
                    "NAME": "id",
                    "TYPE": "INT",
                    "ATTRIBUTES": [
                        "UNIQUE",
                        "PRIMARY_KEY",
                        "AUTOINCREMENT"
                    ]
                },
                {
                    "NAME": "name",
                    "TYPE": "TEXT"
                },
                {
                    "NAME": "valid",
                    "TYPE": "BOOLEAN"
                },
                {
                    "NAME": "value",
                    "TYPE": "DOUBLE"
                },
                {
                    "NAME": "timestamp",
                    "TYPE": "UNIX_TIME"
                },
                {
                    "NAME": "something",
                    "TYPE": "BLOB"
                }
            ]
        }
    }
}
```

* `SELECT` Command
```json
{
    "SELECT" : {
        "COLUMNS": ["id", "value", "valid"],
        "FROM": "my_table",
        "WHERE": {
            "id": {
                "in": [123, 456],
                ">": 123,
                "<": 456,
                "=": 333
            }
        },
        "ORDER": {
            "BY": "id",
            "HOW": "ASC"
        }
    }
}
```


## Commands
* `mkdocs serve` - Start the live-reloading docs server.
* `mkdocs build` - Build the documentation site.
* `mkdocs -h` - Print help message and exit.

## Project layout

    communication/          # The configuration file.
        src/
            CMakeLists.txt
            
        tests/
        build.sh
        clean.sh
        CMakeLists.txt
        README.md
    docs/
        index.md  # The documentation homepage.
        ...       # Other markdown pages, images and other files.
