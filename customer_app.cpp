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

string readPin(const string& prompt) {
    string pin;
    do {
        pin = readRequiredText(prompt);
        if (!isValidPin(pin)) cout << "PIN must contain exactly four digits.\n";
    } while (!isValidPin(pin));
    return pin;
}

double readAmount(const string& prompt) {
    string input;
    double amount = 0.0;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (parsePositiveAmount(input, amount)) return amount;
        cout << "Enter a valid amount greater than zero.\n";
    }
}

void pauseScreen() { cout << "\nPress Enter to continue..."; cin.get(); }

int authenticate(const vector<Account>& accounts, string& accountNumber) {
    accountNumber = readAccountNumber("Account Number: ");
    string pin = readPin("PIN: ");
    int index = findAccountIndex(accounts, accountNumber);
    if (index == -1 || !accounts[index].active || accounts[index].pin != pin) return -1;
    return index;
}

void showDetails(const Account& account) {
    cout << "\n===== ACCOUNT DETAILS =====\n"
         << "Account number: " << account.accountNumber << '\n'
         << "Name: " << account.name << '\n'
         << "Balance: $" << fixed << setprecision(2) << account.balance << '\n'
         << "Status: " << (account.active ? "Active" : "Inactive") << '\n';
}

void showHistory(const vector<TransactionRecord>& transactions, const string& accountNumber) {
    bool found = false;
    for (const TransactionRecord& transaction : transactions) {
        if (transaction.fromAccount != accountNumber && transaction.toAccount != accountNumber) continue;
        found = true;
        cout << transaction.timestamp << " | " << transaction.type
             << " | From: " << transaction.fromAccount << " | To: " << transaction.toAccount
             << " | Amount: $" << fixed << setprecision(2) << transaction.amount << '\n';
    }
    if (!found) cout << "No transaction history found.\n";
}

void persist(const vector<Account>& accounts, const vector<TransactionRecord>& transactions) {
    if (!saveAccounts(accounts) || !saveTransactions(transactions)) cout << "Warning: data could not be saved.\n";
}

void customerMenu(vector<Account>& accounts, vector<TransactionRecord>& transactions, const string& accountNumber) {
    bool loggedIn = true;
    while (loggedIn) {
        int index = findAccountIndex(accounts, accountNumber);
        if (index == -1 || !accounts[index].active) { cout << "Account is no longer active.\n"; return; }

        cout << "\n====================================\n       CUSTOMER BANKING SERVICE\n====================================\n"
             << "1. View Account Details\n2. Check Balance\n3. Deposit Money\n4. Withdraw Money\n"
             << "5. Transfer Money\n6. View Transaction History\n7. Update Personal Information\n8. Logout\n====================================\n";

        switch (getMenuChoice(1, 8)) {
            case 1: showDetails(accounts[index]); pauseScreen(); break;
            case 2: cout << "Current balance: $" << fixed << setprecision(2) << accounts[index].balance << '\n'; pauseScreen(); break;
            case 3: {
                double amount = readAmount("Deposit amount: ");
                if (deposit(accounts, accountNumber, amount)) {
                    addTransaction(transactions, "DEPOSIT", accountNumber, "", amount);
                    persist(accounts, transactions);
                    cout << "Deposit successful.\n";
                } else cout << "Deposit failed.\n";
                pauseScreen();
                break;
            }
            case 4: {
                double amount = readAmount("Withdrawal amount: ");
                if (withdraw(accounts, accountNumber, amount)) {
                    addTransaction(transactions, "WITHDRAWAL", accountNumber, "", amount);
                    persist(accounts, transactions);
                    cout << "Withdrawal successful.\n";
                } else cout << "Withdrawal failed: insufficient funds or inactive account.\n";
                pauseScreen();
                break;
            }
            case 5: {
                string receiver = readAccountNumber("Recipient account number: ");
                double amount = readAmount("Transfer amount: ");
                if (transfer(accounts, accountNumber, receiver, amount)) {
                    addTransaction(transactions, "TRANSFER", accountNumber, receiver, amount);
                    persist(accounts, transactions);
                    cout << "Transfer successful.\n";
                } else cout << "Transfer failed: check the recipient, amount, and balance.\n";
                pauseScreen();
                break;
            }
            case 6: showHistory(transactions, accountNumber); pauseScreen(); break;
            case 7: {
                string name = readRequiredText("New name: ");
                string pin = readPin("New PIN: ");
                if (updateAccount(accounts, accountNumber, name, pin)) {
                    persist(accounts, transactions);
                    cout << "Personal information updated.\n";
                } else cout << "Information could not be updated.\n";
                pauseScreen();
                break;
            }
            case 8: loggedIn = false; cout << "Customer logged out.\n"; break;
        }
    }
}
} // namespace

int main() {
    vector<Account> accounts = loadAccounts();
    vector<TransactionRecord> transactions = loadTransactions();
    string accountNumber;

    cout << "====================================\n       SIMPLE BANKING SYSTEM\n====================================\n"
         << "\n===== CUSTOMER LOGIN =====\n";
    if (authenticate(accounts, accountNumber) == -1) {
        cout << "\nInvalid account number or PIN, or the account is inactive.\n"
             << "Customer application closed.\n";
        return 0;
    }

    cout << "\nLogin successful.\n";
    customerMenu(accounts, transactions, accountNumber);
    cout << "\nCustomer application closed.\n";
    return 0;
}
