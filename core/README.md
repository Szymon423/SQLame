# Core module

## Language syntax

### Create

```json
{
    "CREATE": {
        "TABLE": {
            "NAME": "MY_TABLE",
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

### Select

```json
{
    "SELECT": {
        "WHAT": ["id", "value", "valid"],
        "FROM": "my_table",
        "WHERE": {
            "id": {
                "IN": [123, 456],
                ">": 123,
                "<": 456,
                "=": 333
            }
        }
    }
}
```

### Order

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

### Drop

```json
{
    "DROP" : {
        "TABLE": {
            "NAME": "MY_TABLE"
        }
    }
}
```

### Insert

```json
{
    "INSERT" : {
        "INTO": "my_table",
        "VALUES": [
            {
                "id": 0,
                "name": "test-name-0",
                "valid": true,
                "value": 123.4,
                "timestamp": 1717159918,
                "something": 0,
            },
            {
                "id": 1,
                "name": "test-name-1",
                "valid": true,
                "value": 0.456,
                "timestamp": 1717159919,
                "something": 0,
            }    
        ]
    }
}
```