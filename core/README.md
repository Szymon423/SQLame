# Core module

## Language syntax

### Create

```json
{
    "create": {
        "table": {
            "name": "MY_TABLE",
            "columns": [
                {
                    "name": "id",
                    "type": "INT",
                    "attributes": {
                        "unique": true,         // default false
                        "primary_key": true,    // default false
                        "autoincrement": true   // default false, only on primary key
                    }
                },
                {
                    "name": "name",
                    "type": "TEXT"
                },
                {
                    "name": "valid",
                    "type": "BOOLEAN"
                },
                {
                    "name": "value",
                    "type": "DOUBLE"
                },
                {
                    "name": "timestamp",
                    "type": "UNIX_TIME" // can be also UNIX_TIME_MS 
                },
                {
                    "name": "something",
                    "type": "BLOB"
                }
            ]
        }
    }
}
```

### Select

```json
{
    "select" : {
        "what": ["id", "value", "valid"],
        "from": "my_table",
        "where": {
            "id": {
                "in": [123, 456],
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
    "select" : {
        "what": ["id", "value", "valid"],
        "from": "my_table",
        "where": {
            "id": {
                "in": [123, 456],
                ">": 123,
                "<": 456,
                "=": 333
            },
        },
        "order": {
            "by": "id",
            "how": "ASC"
        }
    }
}
```