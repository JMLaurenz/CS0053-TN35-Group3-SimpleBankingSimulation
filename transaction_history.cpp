#include "transaction_history.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace {

string escapeCsvField(const string& value) {
    string escaped = "\"";
    for (char character : value) {
        if (character == '\"') {
            escaped += "\"\"";
        } else {
            escaped += character;
        }
    }
    escaped += '\"';
    return escaped;
}

bool readCsvField(istringstream& row, string& field) {
    field.clear();

    if (row.peek() == '\"') {
        row.get();
        bool closedQuote = false;
        while (row.good()) {
            char character = static_cast<char>(row.get());
            if (character == '\"') {
                if (row.peek() == '\"') {
                    row.get();
                    field += '\"';
                } else {
                    closedQuote = true;
                    break;
                }
            } else {
                field += character;
            }
        }
        return closedQuote &&
               (row.peek() == ',' || row.peek() == char_traits<char>::eof());
    }

    while (row.peek() != ',' && row.peek() != char_traits<char>::eof()) {
        field += static_cast<char>(row.get());
    }
    return true;
}

bool parseRow(const string& line, TransactionRecord& transaction) {
    istringstream row(line);
    string amountText;

    if (!readCsvField(row, transaction.timestamp) || row.get() != ',' ||
        !readCsvField(row, transaction.type) || row.get() != ',' ||
        !readCsvField(row, transaction.fromAccount) || row.get() != ',' ||
        !readCsvField(row, transaction.toAccount) || row.get() != ',' ||
        !readCsvField(row, amountText) ||
        row.peek() != char_traits<char>::eof()) {
        return false;
    }

    try {
        size_t charactersRead = 0;
        transaction.amount = stod(amountText, &charactersRead);
        return charactersRead == amountText.length();
    } catch (...) {
        return false;
    }
}

} // namespace

string currentTimestamp() {
    const time_t now = time(nullptr);
    tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif

    ostringstream output;
    output << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return output.str();
}

vector<TransactionRecord> loadTransactions(const string& filename) {
    vector<TransactionRecord> transactions;
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        return transactions;
    }

    string line;
    while (getline(inputFile, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }

        TransactionRecord transaction;
        if (parseRow(line, transaction)) {
            transactions.push_back(transaction);
        }
    }
    return transactions;
}

bool saveTransactions(const vector<TransactionRecord>& transactions,
                      const string& filename) {
    ofstream outputFile(filename, ios::trunc);
    if (!outputFile.is_open()) {
        return false;
    }

    outputFile << "timestamp,type,fromAccount,toAccount,amount\n";
    for (const TransactionRecord& transaction : transactions) {
        outputFile << escapeCsvField(transaction.timestamp) << ','
                   << escapeCsvField(transaction.type) << ','
                   << escapeCsvField(transaction.fromAccount) << ','
                   << escapeCsvField(transaction.toAccount) << ','
                   << fixed << setprecision(2) << transaction.amount << '\n';
    }
    return static_cast<bool>(outputFile);
}

void addTransaction(vector<TransactionRecord>& transactions,
                    const string& type,
                    const string& fromAccount,
                    const string& toAccount,
                    double amount) {
    transactions.push_back(
        {currentTimestamp(), type, fromAccount, toAccount, amount});
}
