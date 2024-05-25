#!/bin/bash

rm -rf build

clean_program() {
    PROGRAM_NAME="$1"
    echo "Cleaning $PROGRAM_NAME..."
    rm -fr "$PROGRAM_NAME/build"
}

# Lista programów do zbudowania
PROGRAMS=(
    "communication"
    "compression_encryption"
    "management"
    "transaction"
    "core"
)

for PROGRAM in "${PROGRAMS[@]}"; do
    clean_program "$PROGRAM"
done

echo "Cleaned all build files"