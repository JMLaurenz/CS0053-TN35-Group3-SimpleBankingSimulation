#include "transaction_service.h"
#include "account_service.h"

bool deposit(vector<Account>& accounts,
             const string& accountNumber,
             double amount) {
    if (amount <= 0) {
        return false;
    }

    int index = findAccountIndex(accounts, accountNumber);

    if (index == -1 || !accounts[index].active) {
        return false;
    }

    accounts[index].balance += amount;
    return true;
}

bool withdraw(vector<Account>& accounts,
              const string& accountNumber,
              double amount) {
    if (amount <= 0) {
        return false;
    }

    int index = findAccountIndex(accounts, accountNumber);

    if (index == -1 || !accounts[index].active) {
        return false;
    }

    if (accounts[index].balance < amount) {
        return false;
    }

    accounts[index].balance -= amount;
    return true;
}

bool transfer(vector<Account>& accounts,
              const string& senderAccount,
              const string& receiverAccount,
              double amount) {
    if (amount <= 0 || senderAccount == receiverAccount) {
        return false;
    }

    int senderIndex = findAccountIndex(accounts, senderAccount);
    int receiverIndex = findAccountIndex(accounts, receiverAccount);

    if (senderIndex == -1 || receiverIndex == -1) {
        return false;
    }

    if (!accounts[senderIndex].active ||
        !accounts[receiverIndex].active) {
        return false;
    }

    if (accounts[senderIndex].balance < amount) {
        return false;
    }

    accounts[senderIndex].balance -= amount;
    accounts[receiverIndex].balance += amount;

    return true;
}