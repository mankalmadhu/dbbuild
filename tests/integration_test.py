import subprocess
import os

DB_BIN = "./db"
DB_FILE = "mydb.db"

def run_db(commands):
    if os.path.exists(DB_FILE):
        os.remove(DB_FILE)
    
    process = subprocess.Popen(
        [DB_BIN],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    
    commands_str = "\n".join(commands) + "\n.exit\n"
    stdout, stderr = process.communicate(commands_str)
    
    if os.path.exists(DB_FILE):
        os.remove(DB_FILE)
        
    return stdout

def test_full_scan():
    print("Running Scenario 1: Full Scan...")
    output = run_db([
        "insert 1 alice alice@example.com",
        "insert 2 bob bob@example.com",
        "select"
    ])
    assert "(1, alice, alice@example.com)" in output, "Alice missing"
    assert "(2, bob, bob@example.com)" in output, "Bob missing"
    print("  -> Passed!")

def test_filtered_select():
    print("Running Scenario 2: Filtered Select (select 2)...")
    output = run_db([
        "insert 1 alice alice@example.com",
        "insert 2 bob bob@example.com",
        "select 2"
    ])
    assert "(2, bob, bob@example.com)" in output, "Bob missing"
    assert "(1, alice, alice@example.com)" not in output, "Alice should not be returned"
    print("  -> Passed!")

def test_duplicate_key():
    print("Running Scenario 3: Duplicate Key...")
    output = run_db([
        "insert 1 alice alice@example.com",
        "insert 1 bob bob@example.com"
    ])
    assert "Executed. 1 rows affected." in output, "First insert should succeed"
    assert "Error: Duplicate key." in output, "Second insert should fail with Duplicate key"
    print("  -> Passed!")

if __name__ == "__main__":
    if not os.path.exists(DB_BIN):
        print("Error: ./db not found. Please run 'make' first.")
        exit(1)
        
    print("--- Running Python Integration Tests ---")
    test_full_scan()
    test_filtered_select()
    test_duplicate_key()
    print("--- All integration tests passed! ---")
