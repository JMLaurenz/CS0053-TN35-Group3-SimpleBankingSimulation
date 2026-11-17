#ifndef TRANSACTION_SERVICE_H
#define TRANSACTION_SERVICE_H

#include "storage.h"
#include <string>
#include <vector>

using namespace std;

bool deposit(vector<Account> &accounts, const string &accountNumber,
             double amount);

bool withdraw(vector<Account> &accounts, const string &accountNumber,
              double amount);

bool transfer(vector<Account> &accounts, const string &senderAccount,
              const string &receiverAccount, double amount);

#endif