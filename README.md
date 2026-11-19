# Simple Banking Simulation

A C++17 console-based banking simulation for the CS0053 TN35 Group 3 project. It contains separate administrator and customer applications, reusable account and transaction services, CSV persistence, validation, transaction history, and an executable integration test.

## Current implementation status

The user-facing menus are connected to the backend services. The project currently supports:

- Administrator and customer authentication.
- Account creation, listing, searching, updating, activation, and deactivation.
- Customer account details and balance inquiries.
- Deposits, withdrawals, and transfers with balance and account-status checks.
- Customer transaction history and administrator transaction auditing.
- Administrator bank reports showing account and balance totals.
- CSV persistence for accounts and transactions.
- Validation for menu choices, account numbers, four-digit PINs, required text, and finite monetary amounts.
- Rollback of in-memory changes when account or transaction persistence fails.

The administrator username and password are classroom test credentials. They should be replaced with a safer authentication system in a production application.

## Customer application

Run `customer_app` to access the customer interface. The customer must log in with an active account stored in `accounts.csv`.

Menu options:

1. View account details without exposing the PIN.
2. Check the current balance.
3. Deposit a positive amount.
4. Withdraw a positive amount when sufficient funds are available.
5. Transfer a positive amount to another active account.
6. View the logged-in account's transaction history.
7. Update the logged-in customer's name and PIN.
8. Log out.

The sample customer credentials are:

```text
Account number: 100001
PIN: 1234
```

An inactive account cannot log in or receive transactions. An administrator can reactivate it through **Update Account Information**.

## Administrator application

Run `admin_app` to access the administrator interface.

Test credentials:

```text
Username: admin
Password: admin123
```

Menu options:

1. Add a new bank account with validated account number, name, PIN, balance, and status.
2. View all accounts without displaying PINs.
3. Search for an account.
4. Update account information, including changing an inactive account to active.
5. Deactivate an account without permanently deleting its record.
6. View a bank report containing account counts and total recorded balance.
7. Audit all persisted transactions.
8. Log out.

## Data files

`accounts.csv` uses the following columns:

```text
accountNumber,name,pin,balance,active
```

The `active` field uses `1` for active and `0` for inactive. Account saves include the header row and quote fields so names containing commas can be preserved.

`transactions.csv` is created after the first successful customer transaction and uses:

```text
type,sourceAccount,destinationAccount,amount
```

Transaction types are `DEPOSIT`, `WITHDRAW`, and `TRANSFER`. The transaction log is used by customer history and administrator audit functions.

## Project files

| File | Responsibility |
|---|---|
| `admin_app.cpp` | Administrator login, account management, reports, audit, and menu flow. |
| `customer_app.cpp` | Customer login, account actions, transactions, history, and menu flow. |
| `storage.h/.cpp` | Account structure and validated CSV account loading/saving. |
| `account_service.h/.cpp` | Account lookup, creation, updating, activation, deactivation, and balance lookup. |
| `transaction_service.h/.cpp` | Deposit, withdrawal, and transfer rules. |
| `transaction_log.h/.cpp` | Transaction record persistence and history data. |
| `validation.h/.cpp` | Shared input and amount validation. |
| `member2_tests.cpp` | Business-logic tests for transaction rules. |
| `integration_test.sh` | End-to-end test for both applications and persistence. |
| `accounts.csv` | Sample account data. |

## Building and running

Use a C++17-compatible compiler such as `g++`.

Build the administrator application:

```bash
g++ -std=c++17 admin_app.cpp account_service.cpp storage.cpp transaction_log.cpp validation.cpp -o admin_app
```

Build the customer application:

```bash
g++ -std=c++17 customer_app.cpp account_service.cpp storage.cpp transaction_log.cpp transaction_service.cpp validation.cpp -o customer_app
```

Run both applications from the project directory so they can find `accounts.csv` and `transactions.csv`:

```bash
./admin_app
./customer_app
```

On Windows with MinGW, use `.exe` filenames when running the compiled programs.

## Testing

Run the business-logic tests:

```bash
g++ -std=c++17 member2_tests.cpp account_service.cpp storage.cpp transaction_service.cpp -o member2_tests
./member2_tests
```

Run the complete integration test:

```bash
./integration_test.sh
```

The integration test uses a temporary fixture and exercises the actual administrator and customer executables. It verifies:

- Transfer rejection when the recipient is inactive.
- Administrator account search, activation, creation, deactivation, reports, and persistence.
- Customer details, balance inquiry, deposit, withdrawal, transfer, history, profile update, and logout.
- Transaction audit output and persisted account/transaction data.
- Existing business-logic tests and the absence of failed test cases.

The test does not modify the repository's sample `accounts.csv` or leave temporary build files behind.
