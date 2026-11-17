#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>

using namespace std;

struct Account {
    string accountNumber;
    string name;
    string pin;
    double balance = 0.0;
    bool active = true;
};

// Reads accounts from a CSV file. Missing files produce an empty list.
vector<Account> loadAccounts(const string& filename = "accounts.csv");

// Replaces the CSV file contents with the supplied accounts.
bool saveAccounts(const vector<Account>& accounts,
                  const string& filename = "accounts.csv");

#endif
