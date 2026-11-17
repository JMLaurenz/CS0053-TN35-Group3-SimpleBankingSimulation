#include "validation.h"

#include <cmath>
#include <cctype>
#include <string>

using namespace std;

namespace {

bool containsOnlyDigits(const string& value) {
    if (value.empty()) {
        return false;
    }

    for (unsigned char character : value) {
        if (!isdigit(character)) {
            return false;
        }
    }

    return true;
}

} // namespace

bool isValidMenuChoice(int choice, int minimum, int maximum) {
    return minimum <= maximum && choice >= minimum && choice <= maximum;
}

bool isValidAccountNumber(const string& accountNumber) {
    return containsOnlyDigits(accountNumber);
}

bool isValidPin(const string& pin) {
    return pin.length() == 4 && containsOnlyDigits(pin);
}

bool isValidRequiredText(const string& text) {
    for (unsigned char character : text) {
        if (!isspace(character)) {
            return true;
        }
    }

    return false;
}

bool parsePositiveAmount(const string& input, double& amount) {
    try {
        size_t charactersRead = 0;
        double parsedAmount = stod(input, &charactersRead);

        if (charactersRead != input.length() ||
            !isfinite(parsedAmount) || parsedAmount <= 0.0) {
            return false;
        }

        amount = parsedAmount;
        return true;
    } catch (...) {
        return false;
    }
}
