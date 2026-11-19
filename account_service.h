#ifndef ACCOUNT_SERVICE_H
#define ACCOUNT_SERVICE_H

#include "storage.h"
#include <string>
#include <vector>

using namespace std;

int findAccountIndex(const vector<Account> &accounts,
                     const string &accountNumber);

bool createAccount(vector<Account> &accounts, const Account &newAccount);

bool updateAccount(vector<Account> &accounts, const string &accountNumber,
                   const string &newName, const string &newPin);

// Administrator update: allows the account status to be changed as well.
bool updateAccount(vector<Account> &accounts, const string &accountNumber,
                   const string &newName, const string &newPin,
                   bool active);

bool deactivateAccount(vector<Account> &accounts, const string &accountNumber);

double getAccountBalance(const vector<Account> &accounts,
                         const string &accountNumber);

#endif
