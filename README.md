# User Authentication System

## Project Overview
C++ command-line user authentication system with advanced Makefile for debug/release builds. Features secure login, password hashing, and OpenSSL integration.

## Key Features
- User account creation with email validation
- Secure password handling and storage
- Password strength enforcement
- Salted password hashing using MD5 (for demonstration purposes)
- User login functionality
- Data persistence using file I/O

## Technologies Used
- C++
- OpenSSL library
- File I/O for data persistence

## Design Decisions and Best Practices
- **Input Validation**: Implemented robust input validation for email addresses and passwords to enhance security and user experience.
- **Password Security**:
  - Enforced strong password policy (length, complexity)
  - Implemented salted hashing to protect against rainbow table attacks
  - Used hidden input for password entry

## Security Considerations
- While MD5 is used for demonstration, it's acknowledged that it's not suitable for production use. In a real-world scenario, a more secure algorithm like bcrypt or Argon2 would be implemented.
- Discussed the limitations of file-based storage and the need for more secure storage solutions in production environments.


## How to Compile and Run

Ensure you have a C++ compiler and OpenSSL installed on your system.
### I compile and run the program with Makefile and using MinGW32 Make (Windows)
1. Compile the program:
   ```bash
   mingw32-make.exe
   ```
2. Run the compiled program:
   ```bash
   .\programmet-debug.exe
   ```

## Usage Instructions
The program offers a menu-driven interface:

1. **Create an account**
   - Enter a valid email address
   - Create a strong password (8+ characters, including uppercase, lowercase, digit, and special character)

2. **Login**
   - Enter the registered email and password

3. **Exit**
   - Close the application
