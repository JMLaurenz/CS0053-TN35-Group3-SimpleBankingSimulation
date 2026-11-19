#include "account_service.h"
#include "storage.h"
#include "transaction_history.h"
#include "transaction_service.h"
#include "validation.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace {
int getMenuChoice(int minimum, int maximum) {
    string input;
    while (true) {
        cout << "Enter your choice: ";
        getline(cin, input);
        try {
            size_t charactersRead = 0;
            int choice = stoi(input, &charactersRead);
            if (charactersRead == input.length() && isValidMenuChoice(choice, minimum, maximum)) return choice;
        } catch (...) {}
        cout << "Invalid choice. Please enter a number from " << minimum << " to " << maximum << ".\n";
    }
}

string readRequiredText(const string& prompt) {
    string value;
    do {
        cout << prompt;
        getline(cin, value);
        if (!isValidRequiredText(value)) cout << "This field cannot be empty.\n";
    } while (!isValidRequiredText(value));
    return value;
}

string readAccountNumber(const string& prompt) {
    string value;
    do {
        value = readRequiredText(prompt);
        if (!isValidAccountNumber(value)) cout << "Account number must contain digits only.\n";
    } while (!isValidAccountNumber(value));
    return value;
}

string readPin() {
    string pin;
    do {
        pin = readRequiredText("PIN (4 digits): ");
        if (!isValidPin(pin)) cout << "PIN must contain exactly four digits.\n";
    } while (!isValidPin(pin));
    return pin;
}

void pauseScreen() { cout << "\nPress Enter to continue..."; cin.get(); }

bool adminLogin() {
    string username, password;
    cout << "\n===== ADMINISTRATOR LOGIN =====\nUsername: ";
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

void showAccount(const Account& account) {
    cout << "Account number: " << account.accountNumber << '\n'
         << "Name: " << account.name << '\n'
         << "Balance: $" << fixed << setprecision(2) << account.balance << '\n'
         << "Status: " << (account.active ? "Active" : "Inactive") << '\n';
}

void addBankAccount(vector<Account>& accounts) {
    Account account;
    account.accountNumber = readAccountNumber("Account number: ");
    if (findAccountIndex(accounts, account.accountNumber) != -1) {
        cout << "An account with that number already exists.\n";
        return;
    }
    account.name = readRequiredText("Account holder name: ");
    account.pin = readPin();
    account.active = true;
    cout << (createAccount(accounts, account) ? "Account created successfully.\n" : "Account could not be created.\n");
}

void viewAllAccounts(const vector<Account>& accounts) {
    if (accounts.empty()) { cout << "No accounts found.\n"; return; }
    for (const Account& account : accounts) { cout << "\n--------------------\n"; showAccount(account); }
}

void searchAccount(const vector<Account>& accounts) {
    string accountNumber = readAccountNumber("Account number to search: ");
    int index = findAccountIndex(accounts, accountNumber);
    if (index == -1) cout << "Account not found.\n";
    else showAccount(accounts[index]);
}

void updateAccountInformation(vector<Account>& accounts) {
    string accountNumber = readAccountNumber("Account number to update: ");
    int index = findAccountIndex(accounts, accountNumber);
    if (index == -1 || !accounts[index].active) { cout << "Active account not found.\n"; return; }
    string name = readRequiredText("New name: ");
    string pin = readPin();
    cout << (updateAccount(accounts, accountNumber, name, pin) ? "Account updated successfully.\n" : "Account could not be updated.\n");
}

void deactivateBankAccount(vector<Account>& accounts) {
    string accountNumber = readAccountNumber("Account number to deactivate: ");
    cout << (deactivateAccount(accounts, accountNumber) ? "Account deactivated successfully.\n" : "Account not found.\n");
}

void showReports(const vector<Account>& accounts, const vector<TransactionRecord>& transactions) {
    int activeCount = 0;
    double totalBalance = 0.0, deposits = 0.0, withdrawals = 0.0, transfers = 0.0;
    for (const Account& account : accounts) {
        if (account.active) ++activeCount;
        totalBalance += account.balance;
    }
    for (const TransactionRecord& transaction : transactions) {
        if (transaction.type == "DEPOSIT") deposits += transaction.amount;
        if (transaction.type == "WITHDRAWAL") withdrawals += transaction.amount;
        if (transaction.type == "TRANSFER") transfers += transaction.amount;
    }
    cout << "\n===== BANK REPORT =====\n"
         << "Total accounts: " << accounts.size() << '\n'
         << "Active accounts: " << activeCount << '\n'
         << "Inactive accounts: " << accounts.size() - activeCount << '\n'
         << "Total recorded balance: $" << fixed << setprecision(2) << totalBalance << '\n'
         << "Total deposits: $" << deposits << '\n'
         << "Total withdrawals: $" << withdrawals << '\n'
         << "Total transfers: $" << transfers << '\n';
}

void auditTransactions(const vector<TransactionRecord>& transactions) {
    if (transactions.empty()) { cout << "No transactions recorded.\n"; return; }
    for (const TransactionRecord& transaction : transactions) {
        cout << transaction.timestamp << " | " << transaction.type
             << " | From: " << transaction.fromAccount << " | To: " << transaction.toAccount
             << " | Amount: $" << fixed << setprecision(2) << transaction.amount << '\n';
    }
}

void persist(const vector<Account>& accounts, const vector<TransactionRecord>& transactions) {
    if (!saveAccounts(accounts) || !saveTransactions(transactions)) cout << "Warning: data could not be saved.\n";
}

void adminMenu(vector<Account>& accounts, vector<TransactionRecord>& transactions) {
    bool loggedIn = true;
    while (loggedIn) {
        cout << "\n====================================\n       ADMINISTRATOR SYSTEM\n====================================\n"
             << "1. Add Bank Account\n2. View All Accounts\n3. Search Account\n4. Update Account Information\n"
             << "5. Deactivate Account\n6. View Bank Reports\n7. Audit All Transactions\n8. Logout\n====================================\n";
        switch (getMenuChoice(1, 8)) {
            case 1: addBankAccount(accounts); persist(accounts, transactions); pauseScreen(); break;
            case 2: viewAllAccounts(accounts); pauseScreen(); break;
            case 3: searchAccount(accounts); pauseScreen(); break;
            case 4: updateAccountInformation(accounts); persist(accounts, transactions); pauseScreen(); break;
            case 5: deactivateBankAccount(accounts); persist(accounts, transactions); pauseScreen(); break;
            case 6: showReports(accounts, transactions); pauseScreen(); break;
            case 7: auditTransactions(transactions); pauseScreen(); break;
            case 8: loggedIn = false; cout << "Administrator logged out.\n"; break;
        }
    }
}
} // namespace

int main() {
    vector<Account> accounts = loadAccounts();
    vector<TransactionRecord> transactions = loadTransactions();
    cout << "====================================\n       SIMPLE BANKING SYSTEM\n====================================\n";
    if (adminLogin()) adminMenu(accounts, transactions);
    cout << "\nAdministrator application closed.\n";
    return 0;
}
