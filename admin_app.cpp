#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "account_service.h"
#include "storage.h"
#include "transaction_log.h"
#include "validation.h"

using namespace std;

int getMenuChoice(int minimum, int maximum) {
    string input;

    while (true) {
        cout << "Enter your choice: ";
        getline(cin, input);

        try {
            size_t charactersRead = 0;
            int choice = stoi(input, &charactersRead);

            if (charactersRead == input.length() &&
                isValidMenuChoice(choice, minimum, maximum)) {
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

string getAccountNumberInput() {
    string accountNumber;

    while (true) {
        getline(cin, accountNumber);
        if (isValidAccountNumber(accountNumber)) {
            return accountNumber;
        }
        cout << "Invalid account number. Use digits only: ";
    }
}

string getRequiredTextInput(const string& prompt) {
    string value;

    while (true) {
        cout << prompt;
        getline(cin, value);
        if (isValidRequiredText(value)) {
            return value;
        }
        cout << "This field cannot be blank.\n";
    }
}

string getPinInput(const string& prompt) {
    string pin;

    while (true) {
        cout << prompt;
        getline(cin, pin);
        if (isValidPin(pin)) {
            return pin;
        }
        cout << "PIN must contain exactly four digits.\n";
    }
}

double getAmountInput(const string& prompt, bool allowZero) {
    string input;
    double amount = 0.0;

    while (true) {
        cout << prompt;
        getline(cin, input);

        bool valid = allowZero
                         ? parseNonNegativeAmount(input, amount)
                         : parsePositiveAmount(input, amount);
        if (valid) {
            return amount;
        }

        cout << (allowZero ? "Enter a non-negative finite amount.\n"
                           : "Enter a positive finite amount.\n");
    }
}

bool getActiveStatus() {
    string input;

    while (true) {
        cout << "Active account? (Y/N): ";
        getline(cin, input);

        if (input == "Y" || input == "y") {
            return true;
        }
        if (input == "N" || input == "n") {
            return false;
        }

        cout << "Invalid status. Please enter Y or N.\n";
    }
}

void printAccount(const Account& account) {
    cout << "Account number: " << account.accountNumber << '\n'
         << "Name: " << account.name << '\n'
         << "Balance: " << fixed << setprecision(2) << account.balance << '\n'
         << "Status: " << (account.active ? "Active" : "Inactive") << '\n';
}

void addBankAccount(vector<Account>& accounts) {
    cout << "\n===== ADD BANK ACCOUNT =====\n";
    cout << "Account number: ";
    string accountNumber = getAccountNumberInput();

    if (findAccountIndex(accounts, accountNumber) != -1) {
        cout << "An account with that number already exists.\n";
        pauseScreen();
        return;
    }

    string name = getRequiredTextInput("Customer name: ");
    string pin = getPinInput("PIN: ");
    double balance = getAmountInput("Initial balance: ", true);
    bool active = getActiveStatus();
    vector<Account> originalAccounts = accounts;

    if (createAccount(accounts, {accountNumber, name, pin, balance, active}) &&
        saveAccounts(accounts)) {
        cout << "Account created successfully.\n";
    } else {
        accounts = originalAccounts;
        cout << "Unable to create or save the account.\n";
    }

    pauseScreen();
}

void viewAllAccounts(const vector<Account>& accounts) {
    cout << "\n===== ALL ACCOUNTS =====\n";

    if (accounts.empty()) {
        cout << "No accounts found.\n";
    }

    for (const Account& account : accounts) {
        printAccount(account);
        cout << "-------------------------\n";
    }

    pauseScreen();
}

void searchAccount(const vector<Account>& accounts) {
    cout << "\n===== SEARCH ACCOUNT =====\n";
    cout << "Account number: ";
    string accountNumber = getAccountNumberInput();
    int index = findAccountIndex(accounts, accountNumber);

    if (index == -1) {
        cout << "Account not found.\n";
    } else {
        printAccount(accounts[index]);
    }

    pauseScreen();
}

void updateAccountInformation(vector<Account>& accounts) {
    cout << "\n===== UPDATE ACCOUNT INFORMATION =====\n";
    cout << "Account number: ";
    string accountNumber = getAccountNumberInput();
    int index = findAccountIndex(accounts, accountNumber);

    if (index == -1) {
        cout << "Account not found.\n";
        pauseScreen();
        return;
    }

    cout << "Current status: "
         << (accounts[index].active ? "Active" : "Inactive") << '\n';
    string newName = getRequiredTextInput("New customer name: ");
    string newPin = getPinInput("New PIN: ");
    bool active = getActiveStatus();
    vector<Account> originalAccounts = accounts;

    if (updateAccount(accounts, accountNumber, newName, newPin, active) &&
        saveAccounts(accounts)) {
        cout << "Account information updated successfully.\n";
        cout << "New status: " << (active ? "Active" : "Inactive") << '\n';
    } else {
        accounts = originalAccounts;
        cout << "Unable to update or save the account.\n";
    }

    pauseScreen();
}

void deactivateBankAccount(vector<Account>& accounts) {
    cout << "\n===== DEACTIVATE ACCOUNT =====\n";
    cout << "Account number: ";
    string accountNumber = getAccountNumberInput();
    vector<Account> originalAccounts = accounts;

    if (deactivateAccount(accounts, accountNumber) && saveAccounts(accounts)) {
        cout << "Account deactivated successfully.\n";
    } else {
        accounts = originalAccounts;
        cout << "Account not found or could not be saved.\n";
    }

    pauseScreen();
}

void viewBankReports(const vector<Account>& accounts) {
    int activeCount = 0;
    int inactiveCount = 0;
    double totalBalance = 0.0;

    for (const Account& account : accounts) {
        account.active ? ++activeCount : ++inactiveCount;
        totalBalance += account.balance;
    }

    cout << "\n===== BANK REPORT =====\n"
         << "Total accounts: " << accounts.size() << '\n'
         << "Active accounts: " << activeCount << '\n'
         << "Inactive accounts: " << inactiveCount << '\n'
         << "Total recorded balance: " << fixed << setprecision(2)
         << totalBalance << '\n';
    pauseScreen();
}

void auditTransactions(const vector<TransactionRecord>& records) {
    cout << "\n===== TRANSACTION AUDIT =====\n";

    if (records.empty()) {
        cout << "No transactions recorded.\n";
    }

    for (const TransactionRecord& record : records) {
        cout << record.type << " | From: " << record.sourceAccount
             << " | To: " << record.destinationAccount
             << " | Amount: " << fixed << setprecision(2)
             << record.amount << '\n';
    }

    pauseScreen();
}

void adminMenu(vector<Account>& accounts,
               const vector<TransactionRecord>& records) {
    bool loggedIn = true;

    while (loggedIn) {
        cout << "\n====================================\n"
             << "       ADMINISTRATOR SYSTEM\n"
             << "====================================\n"
             << "1. Add Bank Account\n"
             << "2. View All Accounts\n"
             << "3. Search Account\n"
             << "4. Update Account Information\n"
             << "5. Deactivate Account\n"
             << "6. View Bank Reports\n"
             << "7. Audit All Transactions\n"
             << "8. Logout\n"
             << "====================================\n";

        switch (getMenuChoice(1, 8)) {
            case 1:
                addBankAccount(accounts);
                break;
            case 2:
                viewAllAccounts(accounts);
                break;
            case 3:
                searchAccount(accounts);
                break;
            case 4:
                updateAccountInformation(accounts);
                break;
            case 5:
                deactivateBankAccount(accounts);
                break;
            case 6:
                viewBankReports(accounts);
                break;
            case 7:
                auditTransactions(records);
                break;
            case 8:
                cout << "\nAdministrator logged out.\n";
                loggedIn = false;
                break;
        }
    }
}

int main() {
    cout << "====================================\n"
         << "       SIMPLE BANKING SYSTEM\n"
         << "====================================\n";

    if (adminLogin()) {
        vector<Account> accounts = loadAccounts();
        vector<TransactionRecord> records = loadTransactionRecords();
        adminMenu(accounts, records);
    }

    cout << "\nAdministrator application closed.\n";
    return 0;
}
