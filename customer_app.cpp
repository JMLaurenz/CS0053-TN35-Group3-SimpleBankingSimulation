#include <iostream>
#include <string>
#include <limits>

using namespace std;

int getMenuChoice(int minimum, int maximum) {
    int choice;

    while (true) {
        cout << "Enter your choice: ";

        if (cin >> choice && choice >= minimum && choice <= maximum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }

        cout << "Invalid choice. Please enter a number from "
             << minimum << " to " << maximum << ".\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

bool customerLogin() {
    string accountNumber;
    string pin;

    cout << "\n===== CUSTOMER LOGIN =====\n";
    cout << "Account Number: ";
    getline(cin, accountNumber);

    cout << "PIN: ";
    getline(cin, pin);

    // Temporary login credentials for initial testing
    if (accountNumber == "100001" && pin == "1234") {
        cout << "\nLogin successful.\n";
        return true;
    }

    cout << "\nInvalid account number or PIN.\n";
    return false;
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

void customerMenu() {
    int choice;
    bool loggedIn = true;

    while (loggedIn) {
        cout << "\n====================================\n";
        cout << "       CUSTOMER BANKING SERVICE\n";
        cout << "====================================\n";
        cout << "1. View Account Details\n";
        cout << "2. Check Balance\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. Transfer Money\n";
        cout << "6. View Transaction History\n";
        cout << "7. Update Personal Information\n";
        cout << "8. Logout\n";
        cout << "====================================\n";

        choice = getMenuChoice(1, 8);

        switch (choice) {
            case 1:
                cout << "\n[To be connected to account_service.cpp]\n";
                pauseScreen();
                break;

            case 2:
                cout << "\n[To be connected to balance inquiry function]\n";
                pauseScreen();
                break;

            case 3:
                cout << "\n[To be connected to deposit function]\n";
                pauseScreen();
                break;

            case 4:
                cout << "\n[To be connected to withdrawal function]\n";
                pauseScreen();
                break;

            case 5:
                cout << "\n[To be connected to transfer function]\n";
                pauseScreen();
                break;

            case 6:
                cout << "\n[To be connected to transaction history function]\n";
                pauseScreen();
                break;

            case 7:
                cout << "\n[To be connected to customer profile update function]\n";
                pauseScreen();
                break;

            case 8:
                cout << "\nCustomer logged out.\n";
                loggedIn = false;
                break;
        }
    }
}

int main() {
    cout << "====================================\n";
    cout << "       SIMPLE BANKING SYSTEM\n";
    cout << "====================================\n";

    if (customerLogin()) {
        customerMenu();
    }

    cout << "\nCustomer application closed.\n";
    return 0;
}