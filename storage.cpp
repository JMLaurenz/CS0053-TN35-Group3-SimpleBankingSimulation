#include "storage.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

string escapeCsvField(const string& value) {
    string escaped = "\"";

    for (char character : value) {
        if (character == '"') {
            escaped += "\"\"";
        } else {
            escaped += character;
        }
    }

    escaped += '"';
    return escaped;
}

// Reads one CSV field, including quoted fields with commas and escaped quotes.
bool readCsvField(istringstream& row, string& field) {
    field.clear();

    if (row.peek() == '"') {
        row.get();
        bool closedQuote = false;

        while (row.good()) {
            char character = static_cast<char>(row.get());

            if (character == '"') {
                if (row.peek() == '"') {
                    row.get();
                    field += '"';
                } else {
                    closedQuote = true;
                    break;
                }
            } else {
                field += character;
            }
        }

        if (!closedQuote) {
            return false;
        }

        return row.peek() == ',' || row.peek() == char_traits<char>::eof();
    }

    while (true) {
        int nextCharacter = row.peek();
        if (nextCharacter == char_traits<char>::eof() || nextCharacter == ',') {
            break;
        }
        field += static_cast<char>(row.get());
    }

    return true;
}

bool parseAccountRow(const string& line, Account& account) {
    istringstream row(line);
    string accountNumber;
    string name;
    string pin;
    string balanceText;
    string activeText;

    if (!readCsvField(row, accountNumber) || row.get() != ',' ||
        !readCsvField(row, name) || row.get() != ',' ||
        !readCsvField(row, pin) || row.get() != ',' ||
        !readCsvField(row, balanceText) || row.get() != ',' ||
        !readCsvField(row, activeText)) {
        return false;
    }

    if (row.peek() != char_traits<char>::eof()) {
        return false;
    }

    try {
        size_t charactersRead = 0;
        double balance = stod(balanceText, &charactersRead);

        if (charactersRead != balanceText.length() ||
            (activeText != "0" && activeText != "1")) {
            return false;
        }

        account.accountNumber = accountNumber;
        account.name = name;
        account.pin = pin;
        account.balance = balance;
        account.active = (activeText == "1");
    } catch (...) {
        return false;
    }

    return true;
}

} // namespace

// CSV columns: accountNumber,name,pin,balance,active (1 = active, 0 = inactive).
// Invalid rows and a header row are skipped.
vector<Account> loadAccounts(const string& filename) {
    vector<Account> accounts;
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        return accounts;
    }

    string line;
    while (getline(inputFile, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back(); // Handle Windows CRLF line endings.
        }

        if (line.empty()) {
            continue;
        }

        Account account;
        if (parseAccountRow(line, account)) {
            accounts.push_back(account);
        }
    }

    return accounts;
}

bool saveAccounts(const vector<Account>& accounts, const string& filename) {
    ofstream outputFile(filename, ios::trunc);

    if (!outputFile.is_open()) {
        return false;
    }

    for (const Account& account : accounts) {
        outputFile << escapeCsvField(account.accountNumber) << ','
                   << escapeCsvField(account.name) << ','
                   << escapeCsvField(account.pin) << ','
                   << account.balance << ','
                   << (account.active ? 1 : 0) << '\n';

        if (!outputFile) {
            return false;
        }
    }

    return true;
}
