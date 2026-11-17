#include "account_service.h"
#include "transaction_service.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void showOperationTest(const string &testName, bool expected, bool actual) {
  cout << testName << '\n';
  cout << "Expected: " << (expected ? "Success" : "Failure") << '\n';
  cout << "Actual:   " << (actual ? "Success" : "Failure") << '\n';
  cout << "Result:   " << (expected == actual ? "PASSED" : "FAILED") << "\n\n";
}

void showBalanceTest(const string &testName, double expected, double actual) {
  cout << testName << '\n';
  cout << "Expected balance: " << expected << '\n';
  cout << "Actual balance:   " << actual << '\n';
  cout << "Result:            " << (expected == actual ? "PASSED" : "FAILED")
       << "\n\n";
}

int main() {
  vector<Account> accounts = {
      {"100001", "Alex Cruz", "1234", 1000.00, true},
      {"100002", "Sam Santos", "5678", 500.00, true},
      {"100003", "Inactive User", "9999", 300.00, false}};

  cout << fixed << setprecision(2);
  cout << "===== MEMBER 2 BUSINESS LOGIC TESTS =====\n\n";

  bool depositResult = deposit(accounts, "100001", 200.00);
  showOperationTest("Test 1: Valid deposit", true, depositResult);
  showBalanceTest("Test 1 Balance Check", 1200.00,
                  getAccountBalance(accounts, "100001"));

  bool withdrawalResult = withdraw(accounts, "100001", 150.00);
  showOperationTest("Test 2: Valid withdrawal", true, withdrawalResult);
  showBalanceTest("Test 2 Balance Check", 1050.00,
                  getAccountBalance(accounts, "100001"));

  bool insufficientResult = withdraw(accounts, "100001", 5000.00);
  showOperationTest("Test 3: Withdrawal greater than balance", false,
                    insufficientResult);
  showBalanceTest("Test 3 Balance Unchanged", 1050.00,
                  getAccountBalance(accounts, "100001"));

  bool transferResult = transfer(accounts, "100001", "100002", 300.00);

  showOperationTest("Test 4: Valid transfer", true, transferResult);
  showBalanceTest("Test 4 Sender Balance", 750.00,
                  getAccountBalance(accounts, "100001"));
  showBalanceTest("Test 4 Receiver Balance", 800.00,
                  getAccountBalance(accounts, "100002"));

  bool inactiveTransferResult = transfer(accounts, "100001", "100003", 100.00);

  showOperationTest("Test 5: Transfer to inactive account", false,
                    inactiveTransferResult);

  cout << "===== TESTING COMPLETE =====\n";

  return 0;
}