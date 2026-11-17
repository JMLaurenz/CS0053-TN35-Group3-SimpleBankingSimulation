#include "account_service.h"

int findAccountIndex(const vector<Account> &accounts,
                     const string &accountNumber) {
  for (size_t i = 0; i < accounts.size(); i++) {
    if (accounts[i].accountNumber == accountNumber) {
      return static_cast<int>(i);
    }
  }

  return -1;
}

bool createAccount(vector<Account> &accounts, const Account &newAccount) {
  if (findAccountIndex(accounts, newAccount.accountNumber) != -1) {
    return false;
  }

  accounts.push_back(newAccount);
  return true;
}

bool updateAccount(vector<Account> &accounts, const string &accountNumber,
                   const string &newName, const string &newPin) {
  int index = findAccountIndex(accounts, accountNumber);

  if (index == -1 || !accounts[index].active) {
    return false;
  }

  accounts[index].name = newName;
  accounts[index].pin = newPin;

  return true;
}

bool deactivateAccount(vector<Account> &accounts, const string &accountNumber) {
  int index = findAccountIndex(accounts, accountNumber);

  if (index == -1) {
    return false;
  }

  accounts[index].active = false;
  return true;
}

double getAccountBalance(const vector<Account> &accounts,
                         const string &accountNumber) {
  int index = findAccountIndex(accounts, accountNumber);

  if (index == -1) {
    return -1.0;
  }

  return accounts[index].balance;
}