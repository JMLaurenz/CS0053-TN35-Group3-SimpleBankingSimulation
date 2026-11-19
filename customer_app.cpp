#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "account_service.h"
#include "storage.h"
#include "transaction_log.h"
#include "transaction_service.h"
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

bool customerLogin(const vector<Account>& accounts, string& loggedAccount) {
    string accountNumber;
    string pin;

    cout << "\n===== CUSTOMER LOGIN =====\n";
    cout << "Account Number: ";
    getline(cin, accountNumber);

    cout << "PIN: ";
    getline(cin, pin);

    int index = findAccountIndex(accounts, accountNumber);
    if (index != -1 && accounts[index].active && accounts[index].pin == pin) {
        loggedAccount = accountNumber;
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

string getAccountNumberInput(const string& prompt) {
    string accountNumber;

    while (true) {
        cout << prompt;
        getline(cin, accountNumber);
        if (isValidAccountNumber(accountNumber)) {
            return accountNumber;
        }
        cout << "Invalid account number. Use digits only.\n";
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

double getPositiveAmountInput() {
    string input;
    double amount = 0.0;

    while (true) {
        cout << "Amount: ";
        getline(cin, input);
        if (parsePositiveAmount(input, amount)) {
            return amount;
        }
        cout << "Enter a positive finite amount.\n";
    }
}

bool saveTransactionChange(
    vector<Account>& accounts,
    const vector<Account>& originalAccounts,
    vector<TransactionRecord>& records,
    const vector<TransactionRecord>& originalRecords,
    const string& type,
    const string& sourceAccount,
    const string& destinationAccount,
    double amount) {
    if (!appendTransactionRecord(records, type, sourceAccount,
                                 destinationAccount, amount) ||
        !saveAccounts(accounts) || !saveTransactionRecords(records)) {
        accounts = originalAccounts;
        records = originalRecords;
        saveAccounts(accounts);
        saveTransactionRecords(records);
        return false;
    }

    return true;
}

void viewAccountDetails(const vector<Account>& accounts,
                        const string& loggedAccount) {
    int index = findAccountIndex(accounts, loggedAccount);

    cout << "\n===== ACCOUNT DETAILS =====\n";
    if (index == -1) {
        cout << "Account could not be found.\n";
    } else {
        cout << "Account number: " << accounts[index].accountNumber << '\n'
             << "Name: " << accounts[index].name << '\n'
             << "Balance: " << fixed << setprecision(2)
             << accounts[index].balance << '\n'
             << "Status: " << (accounts[index].active ? "Active" : "Inactive")
             << '\n';
    }

    pauseScreen();
}

void depositMoney(vector<Account>& accounts,
                  vector<TransactionRecord>& records,
                  const string& loggedAccount) {
    cout << "\n===== DEPOSIT MONEY =====\n";
    double amount = getPositiveAmountInput();
    vector<Account> originalAccounts = accounts;
    vector<TransactionRecord> originalRecords = records;

    if (deposit(accounts, loggedAccount, amount) &&
        saveTransactionChange(accounts, originalAccounts, records,
                              originalRecords, "DEPOSIT", loggedAccount, "-",
                              amount)) {
        cout << "Deposit completed successfully.\n";
    } else {
        cout << "Deposit failed.\n";
    }

    pauseScreen();
}

void withdrawMoney(vector<Account>& accounts,
                   vector<TransactionRecord>& records,
                   const string& loggedAccount) {
    cout << "\n===== WITHDRAW MONEY =====\n";
    double amount = getPositiveAmountInput();
    vector<Account> originalAccounts = accounts;
    vector<TransactionRecord> originalRecords = records;

    if (withdraw(accounts, loggedAccount, amount) &&
        saveTransactionChange(accounts, originalAccounts, records,
                              originalRecords, "WITHDRAW", loggedAccount, "-",
                              amount)) {
        cout << "Withdrawal completed successfully.\n";
    } else {
        cout << "Withdrawal failed. Check the available balance.\n";
    }

    pauseScreen();
}

void transferMoney(vector<Account>& accounts,
                   vector<TransactionRecord>& records,
                   const string& loggedAccount) {
    cout << "\n===== TRANSFER MONEY =====\n";
    string receiverAccount = getAccountNumberInput("Recipient account number: ");
    double amount = getPositiveAmountInput();
    vector<Account> originalAccounts = accounts;
    vector<TransactionRecord> originalRecords = records;

    if (transfer(accounts, loggedAccount, receiverAccount, amount) &&
        saveTransactionChange(accounts, originalAccounts, records,
                              originalRecords, "TRANSFER", loggedAccount,
                              receiverAccount, amount)) {
        cout << "Transfer completed successfully.\n";
    } else {
        cout << "Transfer failed. Check the recipient account, account status, "
                "and available balance.\n";
    }

    pauseScreen();
}

void viewTransactionHistory(const vector<TransactionRecord>& records,
                            const string& loggedAccount) {
    cout << "\n===== TRANSACTION HISTORY =====\n";
    bool found = false;

    for (const TransactionRecord& record : records) {
        if (record.sourceAccount == loggedAccount ||
            record.destinationAccount == loggedAccount) {
            cout << record.type << " | From: " << record.sourceAccount
                 << " | To: " << record.destinationAccount
                 << " | Amount: " << fixed << setprecision(2)
                 << record.amount << '\n';
            found = true;
        }
    }

    if (!found) {
        cout << "No transactions found for this account.\n";
    }

    pauseScreen();
}

void updatePersonalInformation(vector<Account>& accounts,
                               const string& loggedAccount) {
    cout << "\n===== UPDATE PERSONAL INFORMATION =====\n";
    string newName = getRequiredTextInput("New name: ");
    string newPin = getPinInput("New PIN: ");
    vector<Account> originalAccounts = accounts;

    if (updateAccount(accounts, loggedAccount, newName, newPin) &&
        saveAccounts(accounts)) {
        cout << "Personal information updated successfully.\n";
    } else {
        accounts = originalAccounts;
        cout << "Unable to update personal information.\n";
    }

    pauseScreen();
}

void customerMenu(vector<Account>& accounts,
                  vector<TransactionRecord>& records,
                  const string& loggedAccount) {
    bool loggedIn = true;

    while (loggedIn) {
        cout << "\n====================================\n"
             << "       CUSTOMER BANKING SERVICE\n"
             << "====================================\n"
             << "1. View Account Details\n"
             << "2. Check Balance\n"
             << "3. Deposit Money\n"
             << "4. Withdraw Money\n"
             << "5. Transfer Money\n"
             << "6. View Transaction History\n"
             << "7. Update Personal Information\n"
             << "8. Logout\n"
             << "====================================\n";

        switch (getMenuChoice(1, 8)) {
            case 1:
                viewAccountDetails(accounts, loggedAccount);
                break;
            case 2:
                cout << "\nCurrent balance: " << fixed << setprecision(2)
                     << getAccountBalance(accounts, loggedAccount) << '\n';
                pauseScreen();
                break;
            case 3:
                depositMoney(accounts, records, loggedAccount);
                break;
            case 4:
                withdrawMoney(accounts, records, loggedAccount);
                break;
            case 5:
                transferMoney(accounts, records, loggedAccount);
                break;
            case 6:
                viewTransactionHistory(records, loggedAccount);
                break;
            case 7:
                updatePersonalInformation(accounts, loggedAccount);
                break;
            case 8:
                cout << "\nCustomer logged out.\n";
                loggedIn = false;
                break;
        }
    }
}

int main() {
    cout << "====================================\n"
         << "       SIMPLE BANKING SYSTEM\n"
         << "====================================\n";

    vector<Account> accounts = loadAccounts();
    vector<TransactionRecord> records = loadTransactionRecords();
    string loggedAccount;

    if (customerLogin(accounts, loggedAccount)) {
        customerMenu(accounts, records, loggedAccount);
    }

    cout << "\nCustomer application closed.\n";
    return 0;
}
