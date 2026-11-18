# Simple Banking Simulation

A C++ console-based banking simulation for the CS0053 TN35 Group 3 project. The project is divided into two user-facing applications and reusable backend modules for account management, CSV storage, and banking transactions.

## Current implementation status

The repository currently contains:

- Separate administrator and customer login screens.
- Separate administrator and customer menu loops.
- Input validation for menu choices, including rejection of values such as `2abc`.
- Logout options for both applications.
- Account creation, searching, updating, deactivation, and balance lookup functions in the account service.
- CSV loading and saving with support for quoted fields, commas, escaped quotes, invalid-row handling, and Windows line endings.
- Deposit, withdrawal, and transfer business logic.
- Business-logic tests for valid transactions, insufficient funds, inactive accounts, and balance changes.

The application menus and backend services are not fully wired together yet. The banking options in `admin_app.cpp` and `customer_app.cpp` currently display connection placeholders. Programmers should connect these menu cases to the existing service functions before treating the applications as complete.

## User-facing applications

### Customer side: `customer_app.cpp`

The customer application represents the banking kiosk or client interface. After a successful login, the customer menu provides the following pages/options:

1. **View Account Details** – intended to display the logged-in customer's account number, name, status, and other account information.
2. **Check Balance** – intended to call `getAccountBalance()` and display the current balance.
3. **Deposit Money** – intended to validate an amount, call `deposit()`, and save the updated accounts.
4. **Withdraw Money** – intended to validate an amount, call `withdraw()`, and save the updated accounts.
5. **Transfer Money** – intended to collect a recipient account and amount, call `transfer()`, and save the updated accounts.
6. **View Transaction History** – reserved for transaction-history functionality, which is not implemented in the current repository.
7. **Update Personal Information** – intended to call `updateAccount()` for the logged-in customer.
8. **Logout** – ends the customer menu loop.

Current customer test credentials:

- **Account number:** `100001`
- **PIN:** `1234`

The login values are currently hard-coded for testing. The customer application must be connected to `loadAccounts()` and account-service functions so that authentication uses the CSV data and inactive accounts are rejected.

### Administrator side: `admin_app.cpp`

The administrator application represents the bank staff interface. After a successful login, the administrator menu provides the following pages/options:

1. **Add Bank Account** – intended to collect account details and call `createAccount()`.
2. **View All Accounts** – intended to load and display all accounts, including their active/inactive status.
3. **Search Account** – intended to call `findAccountIndex()` and display a matching account.
4. **Update Account Information** – intended to call `updateAccount()`.
5. **Deactivate or Delete Account** – the current backend supports deactivation through `deactivateAccount()`; permanent deletion is not implemented.
6. **View Bank Reports** – reserved for reporting functionality. A `reports.cpp` file is not currently included in the repository.
7. **Audit All Transactions** – reserved for transaction-audit functionality. A transaction-history or audit log is not currently persisted.
8. **Logout** – ends the administrator menu loop.

Current administrator test credentials:

- **Username:** `admin`
- **Password:** `admin123`

The administrator credentials are currently hard-coded for testing and should be replaced with a safer authentication method in a production-quality system.

## Project files

| File | Responsibility |
|---|---|
| `admin_app.cpp` | Administrator login, menu display, menu validation, and logout loop. |
| `customer_app.cpp` | Customer login, menu display, menu validation, and logout loop. |
| `storage.h/.cpp` | `Account` structure and CSV loading/saving. |
| `account_service.h/.cpp` | Account lookup, creation, updating, deactivation, and balance lookup. |
| `transaction_service.h/.cpp` | Deposit, withdrawal, and transfer operations. |
| `member2_tests.cpp` | Console tests for account and transaction business logic. |
| `accounts.csv` | Runtime account data used by the storage functions. |

## Account data format

`accounts.csv` uses the following column order:

```text
accountNumber,name,pin,balance,active
```

The `active` field uses `1` for an active account and `0` for an inactive account. `loadAccounts()` skips the header, blank lines, and invalid rows. `saveAccounts()` rewrites the target CSV file with the current vector of accounts.

The default filename is `accounts.csv`, so the executable must be run from the directory containing that file, or a different path must be passed to `loadAccounts()` and `saveAccounts()`.

## Backend rules programmers should know

- Account numbers must be unique when using `createAccount()`.
- `updateAccount()` refuses to update missing or inactive accounts.
- `deactivateAccount()` marks an existing account inactive instead of deleting it.
- Deposits and withdrawals must have an amount greater than zero.
- Deposits, withdrawals, and transfers reject missing or inactive accounts.
- A withdrawal fails when the account does not have enough money.
- A transfer fails when the sender and receiver are the same, either account is missing/inactive, or the sender has insufficient funds.
- Service functions modify the in-memory `vector<Account>`. Call `saveAccounts()` after a successful change to persist it.
- `getAccountBalance()` returns `-1.0` when the account number is not found; callers should distinguish this from a valid balance.
- PINs are currently stored as plain text in the CSV file. This is acceptable only for classroom testing and should not be used for real credentials.
- The current transaction service changes balances but does not create a transaction-history record.

## Building and running

Use a C++17-compatible compiler such as `g++`.

Build the administrator application:

```bash
g++ -std=c++17 admin_app.cpp account_service.cpp storage.cpp transaction_service.cpp -o admin_app
```

Build the customer application:

```bash
g++ -std=c++17 customer_app.cpp account_service.cpp storage.cpp transaction_service.cpp -o customer_app
```

Build and run the business-logic tests:

```bash
g++ -std=c++17 member2_tests.cpp account_service.cpp storage.cpp transaction_service.cpp -o member2_tests
./member2_tests
```

Run an application from the project directory so it can find `accounts.csv`:

```bash
./admin_app
./customer_app
```

On Windows with MinGW, use `.exe` filenames when running the compiled programs.

## Recommended integration work

To complete the application, programmers should:

1. Load the accounts once after a successful login or at application startup.
2. Replace each menu placeholder with input handling and calls to the appropriate service function.
3. Save the account vector after every successful create, update, deactivate, deposit, withdrawal, or transfer operation.
4. Use the logged-in account number to prevent a customer from accessing another customer's data.
5. Add transaction records and implement customer history and administrator audit reports.
6. Add stronger validation for account numbers, names, PINs, monetary values, and duplicate accounts.
7. Move shared input and authentication logic into reusable modules where appropriate.
8. Avoid exposing PINs when displaying account lists or reports.

## Testing notes

`member2_tests.cpp` uses an in-memory set of accounts, so it does not modify `accounts.csv`. The tests currently cover:

- A valid deposit and updated balance.
- A valid withdrawal and updated balance.
- A withdrawal greater than the available balance.
- A valid transfer and both resulting balances.
- A transfer to an inactive account.

Additional tests should cover malformed CSV rows, duplicate account numbers, negative and zero amounts, missing accounts, inactive-account logins, persistence after saving, and customer permission boundaries.
