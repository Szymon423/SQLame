import requests
import pytest
import json

# URL serwera bazy danych
BASE_URL = "http://localhost:9443"

# Przykładowe dane do testów
create_table = {
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


drop_table = {
    "DROP" : {
        "TABLE": {
            "NAME": "MY_TABLE"
        }
    }
}

def test_create_table():
    response = requests.post(f"{BASE_URL}", json=create_table)
    print(f"Status Code: {response.status_code}")
    assert response.status_code == 200
    print(f"Response Body: {response.text}")
    data = response.json()
    assert data["message"] == "Table MY_TABLE created succesfully"

def test_table_allready_exist():
    response = requests.post(f"{BASE_URL}", json=create_table)
    print(f"Status Code: {response.status_code}")
    assert response.status_code == 200
    print(f"Response Body: {response.text}")
    data = response.json()
    assert data["message"] == "Table allready exists."

def test_drop_table_exist():
    response = requests.post(f"{BASE_URL}", json=drop_table)
    print(f"Status Code: {response.status_code}")
    assert response.status_code == 200
    print(f"Response Body: {response.text}")
    data = response.json()
    assert data["message"] == "Table MY_TABLE dropped succesfully."

if __name__ == "__main__":
    pytest.main()
