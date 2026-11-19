#include "transaction_log.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

namespace {

bool parseRecord(const string& line, TransactionRecord& record) {
    istringstream row(line);
    string amountText;

    if (!getline(row, record.type, ',') ||
        !getline(row, record.sourceAccount, ',') ||
        !getline(row, record.destinationAccount, ',') ||
        !getline(row, amountText) || row.peek() != char_traits<char>::eof()) {
        return false;
    }

    try {
        size_t charactersRead = 0;
        record.amount = stod(amountText, &charactersRead);
        return charactersRead == amountText.length() &&
               isfinite(record.amount) && record.amount > 0.0;
    } catch (...) {
        return false;
    }
}

} // namespace

vector<TransactionRecord> loadTransactionRecords(const string& filename) {
    vector<TransactionRecord> records;
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        return records;
    }

    string line;
    while (getline(inputFile, line)) {
        if (line.empty() || line == "type,sourceAccount,destinationAccount,amount") {
            continue;
        }

        TransactionRecord record;
        if (parseRecord(line, record)) {
            records.push_back(record);
        }
    }

    return records;
}

bool saveTransactionRecords(const vector<TransactionRecord>& records,
                            const string& filename) {
    ofstream outputFile(filename, ios::trunc);

    if (!outputFile.is_open()) {
        return false;
    }

    outputFile << "type,sourceAccount,destinationAccount,amount\n";
    outputFile << fixed << setprecision(2);

    for (const TransactionRecord& record : records) {
        outputFile << record.type << ','
                   << record.sourceAccount << ','
                   << record.destinationAccount << ','
                   << record.amount << '\n';

        if (!outputFile) {
            return false;
        }
    }

    return true;
}

bool appendTransactionRecord(vector<TransactionRecord>& records,
                             const string& type,
                             const string& sourceAccount,
                             const string& destinationAccount,
                             double amount) {
    if (type.empty() || sourceAccount.empty() || amount <= 0.0) {
        return false;
    }

    records.push_back({type, sourceAccount, destinationAccount, amount});
    return true;
}
