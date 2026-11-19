#ifndef TRANSACTION_HISTORY_H
#define TRANSACTION_HISTORY_H

#include <string>
#include <vector>

using namespace std;

struct TransactionRecord {
    string timestamp;
    string type;
    string fromAccount;
    string toAccount;
    double amount = 0.0;
};

vector<TransactionRecord> loadTransactions(
    const string& filename = "transactions.csv");

bool saveTransactions(const vector<TransactionRecord>& transactions,
                      const string& filename = "transactions.csv");

void addTransaction(vector<TransactionRecord>& transactions,
                    const string& type,
                    const string& fromAccount,
                    const string& toAccount,
                    double amount);

string currentTimestamp();

#endif
