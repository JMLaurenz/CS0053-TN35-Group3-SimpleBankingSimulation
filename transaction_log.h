#ifndef TRANSACTION_LOG_H
#define TRANSACTION_LOG_H

#include <string>
#include <vector>

using namespace std;

struct TransactionRecord {
    string type;
    string sourceAccount;
    string destinationAccount;
    double amount = 0.0;
};

vector<TransactionRecord> loadTransactionRecords(
    const string& filename = "transactions.csv");

bool saveTransactionRecords(const vector<TransactionRecord>& records,
                            const string& filename = "transactions.csv");

bool appendTransactionRecord(vector<TransactionRecord>& records,
                             const string& type,
                             const string& sourceAccount,
                             const string& destinationAccount,
                             double amount);

#endif
