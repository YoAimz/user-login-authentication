#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <cctype>
#include <conio.h>
#include <algorithm>
using namespace std;

string md5Hash(const string& password) {
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5((const unsigned char*)password.c_str(), password.length(), result);

    stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)result[i];
    }

    return ss.str();
}


bool isValidPassword(const string& password) {
    bool passwordCheckLength = password.length() >= 8;
    bool passwordCheckUpperCase = false;
    bool passwordCheckLowerCase = false;
    bool passwordCheckDigit = false;
    bool passwordCheckSpecial = false;

    for (char ch : password) {
        if (isupper(ch)) {
            passwordCheckUpperCase = true;
        } else if (islower(ch)) {
            passwordCheckLowerCase = true;
        } else if (isdigit(ch)) {
            passwordCheckDigit = true;
        } else if (ispunct(ch)) {
            passwordCheckSpecial = true;
        }
    }

    return passwordCheckLength && passwordCheckUpperCase && passwordCheckLowerCase && 
           passwordCheckDigit && passwordCheckSpecial;
}

string saltPassword() {
    const int saltSize = 16;
    unsigned char salt[saltSize];
    RAND_bytes(salt, saltSize);

    stringstream ss;
    for (int i = 0; i < saltSize; i++) {
        ss << hex << setw(2) << setfill('0') << (int)salt[i];
    }
    return ss.str();
}

string makePasswordHidden() {
    string input;
    char writtenInputedPassword;
    
    while ((writtenInputedPassword = _getch()) != 13) {  
        if (writtenInputedPassword == '\b' && !input.empty()) {
            cout << '\b' << ' ' << '\b';
            input.pop_back();
        } 
        else if (isprint(writtenInputedPassword)) {
            cout << '*';
            input.push_back(writtenInputedPassword);
        }
    }

    cout << endl;
    return input;
}
int main() {
    bool endProgram = false;
    while (!endProgram) {
        ofstream usersFile("users.txt", ios::app);

        cout << "\nLog In Menu\n";
        cout << "Choose an option from the menu by typing a number that is shown\n";
        cout << "1. Create an account\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string username, password, fileLine;

                bool uniqueEmail = false;
                while (!uniqueEmail) {
                    cout << "Enter your email-address: ";
                    cin >> username;
                    if(username.find("@") == string::npos || username.find(".") == string::npos ){
                        cout << "Invalid email-address!\n";
                    }
                    else{
                        transform(username.begin(), username.end(), username.begin(), ::tolower);
                        ifstream usersFileCheck("users.txt");
                        while (getline(usersFileCheck, fileLine)) {
                            size_t usernamePos = fileLine.find(username + ":");
                            if (usernamePos != string::npos) {
                                cout << "Email-address is already in use, Please try again\n";
                                break;
                            }
                        }
                        usersFileCheck.close();

                        if (usersFileCheck.eof()) {
                            uniqueEmail = true;
                        }
                    }
                }

                do {
                    cout << "Enter your password: ";
                    password = makePasswordHidden();

                    if (!isValidPassword(password)) {
                        cout << "Invalid password. Your password needs the following requirements:\n";
                        cout << "-Minimum 8 characters\n";
                        cout << "-At least one uppercase character\n";
                        cout << "-At least one lowercase character\n";
                        cout << "-At least one digit\n";
                        cout << "-At least one special character\n";
                        cout << "Please Try Again!\n";
                    }
                } while (!isValidPassword(password));

                string salt = saltPassword();
                string hashedPassword = md5Hash(password + salt);

                usersFile << username << ":" << hashedPassword << ":" << salt << "\n";

                cout << "Account created successfully!\n";
                usersFile.close();
                break;
            }
            case 2: {
                string username, password;
                cout << "Enter your email address: ";
                cin >> username;

                cout << "Enter your password: ";
                transform(username.begin(), username.end(), username.begin(), ::tolower);
                password = makePasswordHidden();
                ifstream usersFile("users.txt");
                string fileLine;
                string storedHashedPassword, storedSalt;

                bool userFound = false;
                while (getline(usersFile, fileLine)) {
                    size_t firstColon = fileLine.find(":");
                    size_t secondColon = fileLine.find(":", firstColon + 1);
                    if (firstColon != string::npos && secondColon != string::npos) {
                        string storedUsername = fileLine.substr(0, firstColon);
                        storedHashedPassword = fileLine.substr(firstColon + 1, secondColon - firstColon - 1);
                        storedSalt = fileLine.substr(secondColon + 1);

                        if (storedUsername == username) {
                            string enteredHashedPassword = md5Hash(password + storedSalt);

                            if (enteredHashedPassword == storedHashedPassword) {
                                cout << "Login successful\n";
                                userFound = true;
                                break;
                            } 
                            else {
                                cout << "Wrong Password or Username, Try Again!\n";
                                userFound = true;
                                break;
                            }
                        }
                    }
                }

                if (!userFound) {
                    cout << "Wrong Password or Username, Try Again!\n";
                }

                break;
            }

            case 3:
                endProgram = true;
                break;

            default:
                cout << "Invalid choice, please try again\n";
        }

        usersFile.close(); 
    }

    return 0;
}