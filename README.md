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
- Customer transaction history and administrator transaction audit views.
- Bank summary reports for account and transaction totals.
- Shared validation for account numbers, PINs, required text, and positive amounts.
- Business-logic tests for valid transactions, insufficient funds, inactive accounts, and balance changes.

The customer and administrator menus are connected to the account, transaction, validation, and storage services. Account changes and transaction records are saved locally so they can be loaded again after restarting the application.

## User-facing applications

### Customer side: `customer_app.cpp`

The customer application represents the banking kiosk or client interface. After a successful login, the customer menu provides the following pages/options:

1. **View Account Details** – displays the logged-in customer's account information and balance.
2. **Check Balance** – displays the current balance.
3. **Deposit Money** – validates the amount, deposits money, records the transaction, and saves the accounts.
4. **Withdraw Money** – validates the amount, checks available funds, records the transaction, and saves the accounts.
5. **Transfer Money** – validates the recipient and amount, transfers funds, records the transaction, and saves the accounts.
6. **View Transaction History** – displays transactions involving the logged-in customer's account.
7. **Update Personal Information** – updates the customer's name and PIN after validation.
8. **Logout** – ends the customer menu loop.

Current customer test credentials:

- **Account number:** `100001`
- **PIN:** `1234`

Customer authentication now uses `accounts.csv` and rejects missing, incorrect, or inactive accounts.

### Administrator side: `admin_app.cpp`

The administrator application represents the bank staff interface. After a successful login, the administrator menu provides the following pages/options:

1. **Add Bank Account** – validates the account number, name, and PIN before creating an account.
2. **View All Accounts** – displays all accounts and their active/inactive status.
3. **Search Account** – finds and displays a specific account.
4. **Update Account Information** – updates an active account's name and PIN.
5. **Deactivate Account** – marks an account inactive without deleting its record.
6. **View Bank Reports** – displays account totals, active/inactive counts, balances, and transaction totals.
7. **Audit All Transactions** – displays every recorded transaction.
8. **Logout** – ends the administrator menu loop.

Current administrator test credentials:

- **Username:** `admin`
- **Password:** `admin123`

The administrator credentials are hard-coded for classroom testing and should be replaced with a safer authentication method in a production-quality system.

## Project files

| File | Responsibility |
|---|---|
| `admin_app.cpp` | Administrator login, menu display, menu validation, and logout loop. |
| `customer_app.cpp` | Customer login, menu display, menu validation, and logout loop. |
| `storage.h/.cpp` | `Account` structure and CSV loading/saving. |
| `account_service.h/.cpp` | Account lookup, creation, updating, deactivation, and balance lookup. |
| `transaction_service.h/.cpp` | Deposit, withdrawal, and transfer operations. |
| `transaction_history.h/.cpp` | Transaction recording, CSV persistence, timestamps, customer history, and administrator audit data. |
| `validation.h/.cpp` | Shared validation for menu choices, account numbers, PINs, required text, and positive amounts. |
| `member2_tests.cpp` | Console tests for account and transaction business logic. |
| `accounts.csv` | Runtime account data used by the storage functions. |
| `transactions.csv` | Persisted transaction history. |

## Account data format

`accounts.csv` uses the following column order:

```text
accountNumber,name,pin,balance,active
```

The `active` field uses `1` for an active account and `0` for an inactive account. `loadAccounts()` skips the header, blank lines, and invalid rows. `saveAccounts()` rewrites the target CSV file with the current vector of accounts.

The default filename is `accounts.csv`, so the executable must be run from the directory containing that file, or a different path must be passed to `loadAccounts()` and `saveAccounts()`.

`transactions.csv` stores `timestamp`, `type`, `fromAccount`, `toAccount`, and `amount`. It is created with a header and updated after successful deposits, withdrawals, and transfers.

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
- Successful deposits, withdrawals, and transfers create transaction-history records.

## Building and running

Use a C++17-compatible compiler such as `g++`.

Build the administrator application:

```bash
g++ -std=c++17 admin_app.cpp account_service.cpp storage.cpp transaction_service.cpp transaction_history.cpp validation.cpp -o admin_app
```

Build the customer application:

```bash
g++ -std=c++17 customer_app.cpp account_service.cpp storage.cpp transaction_service.cpp transaction_history.cpp validation.cpp -o customer_app
```

Build and run the business-logic tests:

```bash
g++ -std=c++17 member2_tests.cpp account_service.cpp storage.cpp transaction_service.cpp validation.cpp -o member2_tests
./member2_tests
```

Run an application from the project directory so it can find `accounts.csv`:

```bash
./admin_app
./customer_app
```

On Windows with MinGW, use `.exe` filenames when running the compiled programs.

## Further recommended improvements

For a stronger production-quality version, programmers should:

1. Replace hard-coded administrator credentials with a safer authentication method.
2. Add account restoration or permanent deletion if required by the final project scope.
3. Add stronger automated tests for malformed CSV rows and persistence failures.
4. Move repeated input and display helpers into reusable modules.
5. Avoid storing PINs as plain text in a production system.

## Testing notes

`member2_tests.cpp` uses an in-memory set of accounts, so it does not modify `accounts.csv`. The tests currently cover:

- A valid deposit and updated balance.
- A valid withdrawal and updated balance.
- A withdrawal greater than the available balance.
- A valid transfer and both resulting balances.
- A transfer to an inactive account.

Additional tests should cover malformed CSV rows, duplicate account numbers, negative and zero amounts, missing accounts, inactive-account logins, persistence after saving, and customer permission boundaries.
