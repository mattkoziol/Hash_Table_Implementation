# Hash Table Password Manager

## Project Overview
This project implements a secure password server using a hash table data structure. The implementation includes a generic hash table class and a specialized password server class that handles user authentication and password management.

## Features
- Generic hash table implementation with separate chaining
- Password server with secure password encryption
- User management (add, remove, change password)
- File-based password storage and loading
- Dynamic resizing with prime number capacity

### Key Operations
- User authentication and management
- Password encryption using crypt()
- Hash table operations (insert, remove, find)
- File I/O for password persistence
- Dynamic resizing and rehashing

## Implementation Details
The project consists of two main components:

- **HashTable Class**: A generic hash table implementation using separate chaining
  - Template-based implementation for key-value pairs
  - Dynamic resizing with prime number capacities
  - Separate chaining collision resolution

- **PassServer Class**: A specialized password management system
  - Secure password encryption using crypt()
  - User management operations
  - File-based password storage

## Performance Analysis
### Time Complexities
- **Hash Table Operations**:
  - Insert: O(1) average case
  - Remove: O(1) average case
  - Find: O(1) average case
  - Rehash: O(n) when triggered
- **Password Server Operations**:
  - User Management: O(1) average case
  - Password Change: O(1) average case
  - File Operations: O(n) for loading/saving

## Compilation and Usage
To compile the project:
```bash
make
```

To run the program:
```bash
./proj4.x
```

## Example Usage
```cpp
// Create a password server
PassServer server(101);

// Add a user
pair<string, string> user = {"username", "password"};
server.addUser(user);

// Change password
server.changePassword(user, "newpassword");

// Find user
if (server.find("username")) {
    cout << "User found!" << endl;
}

// Save to file
server.write_to_file("passwords.txt");
```

## File Structure
- `hashtable.h`: Header file for hash table implementation
- `hashtable.hpp`: Implementation file for hash table
- `passserver.h`: Header file for password server
- `passserver.cpp`: Implementation file for password server
- `proj4.cpp`: Main program file
- `Makefile`: Build configuration

## Security Features
- Password encryption using crypt()
- Secure password storage
- Protected password file operations

## Author
Matthew Koziol

## Course
COP 4530 - Data Structures 