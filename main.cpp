
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <cctype>

using namespace std;

class AuthSystem
{
private:
    const string filename = "users.txt";

    // Generate a random salt for each user
    string generateSalt()
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(0, 15);

        string salt = "";

        for (int i = 0; i < 16; i++)
        {
            salt += "0123456789abcdef"[dist(gen)];
        }

        return salt;
    }

    // Demo password hashing (not suitable for production)
    string hashPassword(string password, string salt)
    {
        size_t hashValue = hash<string>{}(salt + password);

        stringstream ss;
        ss << hex << hashValue;

        return ss.str();
    }

    // Check whether username already exists
    bool userExists(string username)
    {
        ifstream file(filename);

        string line, savedUsername;

        while (getline(file, line))
        {
            stringstream ss(line);

            getline(ss, savedUsername, '|');

            if (savedUsername == username)
            {
                return true;
            }
        }

        return false;
    }

    // Validate username
    bool validUsername(string username)
    {
        if (username.length() < 3 || username.length() > 20)
            return false;

        for (char ch : username)
        {
            if (!isalnum(static_cast<unsigned char>(ch)) && ch != '_')
                return false;
        }

        return true;
    }

public:

    // Registration function
    void registerUser()
    {
        string username, password;

        cout << "\n===== REGISTRATION =====\n";

        cout << "Enter username: ";
        getline(cin, username);

        if (!validUsername(username))
        {
            cout << "Username must be 3-20 characters "
                 << "and contain only letters, numbers or _.\n";
            return;
        }

        if (userExists(username))
        {
            cout << "Username already exists!\n";
            return;
        }

        cout << "Enter password: ";
        getline(cin, password);

        if (password.length() < 8)
        {
            cout << "Password must contain at least 8 characters.\n";
            return;
        }

        string salt = generateSalt();
        string passwordHash = hashPassword(password, salt);

        ofstream file(filename, ios::app);

        if (!file)
        {
            cout << "Error opening user database!\n";
            return;
        }

        file << username << "|" << salt
             << "|" << passwordHash << "\n";

        file.close();

        cout << "\nRegistration successful!\n";
    }

    // Login function
    void loginUser()
    {
        string username, password;

        cout << "\n===== LOGIN =====\n";

        cout << "Enter username: ";
        getline(cin, username);

        cout << "Enter password: ";
        getline(cin, password);

        ifstream file(filename);

        if (!file)
        {
            cout << "No registered users found!\n";
            return;
        }

        string line;
        string savedUsername, savedSalt, savedHash;

        while (getline(file, line))
        {
            stringstream ss(line);

            getline(ss, savedUsername, '|');
            getline(ss, savedSalt, '|');
            getline(ss, savedHash);

            if (savedUsername == username)
            {
                string enteredHash =
                    hashPassword(password, savedSalt);

                if (enteredHash == savedHash)
                {
                    cout << "\nLogin successful!\n";
                    cout << "Welcome, " << username << "!\n";
                }
                else
                {
                    cout << "\nIncorrect password!\n";
                }

                return;
            }
        }

        cout << "\nUsername not found!\n";
    }
};

// Main function
int main()
{
    AuthSystem system;

    int choice;
    string input;

    while (true)
    {
        cout << "\n========== LOGIN SYSTEM ==========\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";

        cout << "Enter your choice: ";

        getline(cin, input);

        stringstream ss(input);

        if (!(ss >> choice) || (ss >> ws && !ss.eof()))
        {
            cout << "Invalid choice! Enter 1, 2 or 3.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
                system.registerUser();
                break;

            case 2:
                system.loginUser();
                break;

            case 3:
                cout << "Thank you for using our system!\n";
                return 0;

            default:
                cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
