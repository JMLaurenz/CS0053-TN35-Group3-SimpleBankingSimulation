#include <iostream>
#include <string>
#include <limits>

using namespace std;

int getMenuChoice(int minimum, int maximum) {
    string input;

    while (true) {
        cout << "Enter your choice: ";
        getline(cin, input);

        try {
            size_t charactersRead;
            int choice = stoi(input, &charactersRead);

            // Reject entries with extra characters, like "2abc"
            if (charactersRead == input.length() &&
                choice >= minimum && choice <= maximum) {
                return choice;
            }
        } catch (...) {
            // The input was not a valid whole number.
        }

        cout << "Invalid choice. Please enter a number from "
             << minimum << " to " << maximum << ".\n";
    }
}

bool adminLogin() {
    string username;
    string password;

    cout << "\n===== ADMINISTRATOR LOGIN =====\n";
    cout << "Username: ";
    getline(cin, username);

    cout << "Password: ";
    getline(cin, password);

    // Temporary login credentials for initial testing
    if (username == "admin" && password == "admin123") {
        cout << "\nLogin successful.\n";
        return true;
    }

    cout << "\nInvalid administrator credentials.\n";
    return false;
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

void adminMenu() {
    int choice;
    bool loggedIn = true;

    while (loggedIn) {
        cout << "\n====================================\n";
        cout << "       ADMINISTRATOR SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Add Bank Account\n";
        cout << "2. View All Accounts\n";
        cout << "3. Search Account\n";
        cout << "4. Update Account Information\n";
        cout << "5. Deactivate or Delete Account\n";
        cout << "6. View Bank Reports\n";
        cout << "7. Audit All Transactions\n";
        cout << "8. Logout\n";
        cout << "====================================\n";

        choice = getMenuChoice(1, 8);

        switch (choice) {
            case 1:
                cout << "\n[To be connected to account_service.cpp]\n";
                pauseScreen();
                break;

            case 2:
                cout << "\n[To be connected to account viewing function]\n";
                pauseScreen();
                break;

            case 3:
                cout << "\n[To be connected to account search function]\n";
                pauseScreen();
                break;

            case 4:
                cout << "\n[To be connected to account update function]\n";
                pauseScreen();
                break;

            case 5:
                cout << "\n[To be connected to account deactivation function]\n";
                pauseScreen();
                break;

            case 6:
                cout << "\n[To be connected to reports.cpp]\n";
                pauseScreen();
                break;

            case 7:
                cout << "\n[To be connected to transaction audit function]\n";
                pauseScreen();
                break;

            case 8:
                cout << "\nAdministrator logged out.\n";
                loggedIn = false;
                break;
        }
    }
}

int main() {
    cout << "====================================\n";
    cout << "       SIMPLE BANKING SYSTEM\n";
    cout << "====================================\n";

    if (adminLogin()) {
        adminMenu();
    }

    cout << "\nAdministrator application closed.\n";
    return 0;
}