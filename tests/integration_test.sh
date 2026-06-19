#!/bin/bash

# Clean up before testing to ensure a fresh database
rm -f mydb.db

echo "Running REPL Integration Test..."

# We feed commands into the database REPL via a heredoc
# and store the output in /tmp for verification
OUTPUT_FILE="./tmp/db_integration_output.txt"

./db <<EOF > $OUTPUT_FILE
insert 1 alice alice@example.com
insert 2 bob bob@example.com
select
.exit
EOF

# Check if the output file contains the inserted rows
if grep -q "(1, alice, alice@example.com)" "$OUTPUT_FILE" && \
   grep -q "(2, bob, bob@example.com)" "$OUTPUT_FILE"; then
    echo "Integration test passed! Both rows were returned by select."
    rm -f mydb.db
    exit 0
else
    echo "Integration test failed!"
    echo "Actual output was saved to $OUTPUT_FILE:"
    cat "$OUTPUT_FILE"
    rm -f mydb.db
    exit 1
fi
