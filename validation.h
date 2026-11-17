#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>

using namespace std;

// Menu choice must be within the displayed inclusive range.
bool isValidMenuChoice(int choice, int minimum, int maximum);

// Account number must contain at least one digit and no other characters.
bool isValidAccountNumber(const string& accountNumber);

// PIN must contain exactly four digits, matching the current sample accounts.
bool isValidPin(const string& pin);

// Text fields must contain at least one non-whitespace character.
bool isValidRequiredText(const string& text);

// Parses a positive, finite amount. On failure, amount is not changed.
bool parsePositiveAmount(const string& input, double& amount);

#endif
