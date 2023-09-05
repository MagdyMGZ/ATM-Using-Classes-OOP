#include <iostream>
#include <cstring>
#include <limits>

using namespace std;

void removeFailure();
bool isNumeric(char[]);
bool containsOnlyAlphaCharacters(char[]);
void removeUnwantedSpaces(char[]);

// Use this function to input numbers
// This is my version of the cin function for inputting numbers of any generic numeric data type and avoid failure and input problems
// This generic numeric data type "T" is defined by the following template function
template <typename T>
void inputNumber(const char promptMessage[], T& num, const char errorMessage[] = "") {
    // The array is of length 310 since this is the maximum number of digits that can be held by long double numeric type
    char str[310];
	while (true) {
		cout << promptMessage;
		cin.getline(str, 310);
		if (!str[0]) {
            cout << errorMessage;
            continue;
		}

		if (cin) {
		    bool valid = true;
            for (int i = 0; str[i]; i++)
                if (str[i] != '.' && (str[i] < 48 || str[i] > 57)) valid = false;

            if (!valid) {
                cout << errorMessage;
                continue;
            }

            // Convert the string into a double data type then type cast it into the generic data type T
            num = static_cast<T>(atof(str));
            break;
		}

        cout << errorMessage;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

// This function is used to check if all the characters of the string are the same or not
// An extra optional parameter can be provided to test if all characters are equal to a certain character or not
bool allSame(const char* str, char checkChar = '\0') {
    for (int i = 1; str[i]; i++)
        if (str[i] != str[0]) return false;

    // If the extra optional character is given, test if the repeated sequence is equal to this parameter
    return (checkChar ? (str[0] == checkChar) : true);
}

class ATM {
private:
    char accountName[101];  // The string is 100 characters maximum with 1 extra character for the null terminator character
    char cardNumber[11]; // The string is 10 characters with 1 extra character for the null terminator character
    long double amount;

public:
    static ATM accounts[100]; // A static member array variable to hold the declared account objects with a maximum of 100 accounts for the class
    static int numAccounts; // A static member variable to hold the number of declared account objects

    static ATM* findAccountByCardNumber(char cardNumber[]) {
        // return a pointer to the account object with the given card number or else return a null pointer if the given card number does not exist
        for (int i = 0; i < ATM::numAccounts; i++) {
            if (strcmp(ATM::accounts[i].getCardNumber(), cardNumber) == 0)
                return &ATM::accounts[i];
        }

        return NULL;
    }

    // This function is used to check if all accounts are empty or not
    // This is important for transfer and withdrawal processes
    // Since if the user asks to make a withdrawal process when all available accounts are empty
    // These two processes will be forbidden
    static bool allEmpty() {
        for (int i = 0; i < ATM::numAccounts; i++)
            if (!ATM::accounts[i].isEmpty()) return false;

        return true;
    }

    // A getter function to access the private cardNumber variable
    char* getCardNumber() {
        return cardNumber;
    }

    ATM *create_new_acc(char accName[], char cardNum[]) {
        // The following exceptions are thrown if this class was to be used for any purposes other than this program
        // This program has no need to handle the following exceptions
        // They are only here for documentation if this class was used outside this program
        if (ATM::numAccounts == 100)
            // Throw an error if the class was used to create more than 100 accounts
            throw "Accounts Limit Reached. Cannot create more than 100 accounts.";
        if (strlen(accName) > 100)
            // Throw an error if the account name was more than 100 letters
            throw "Account Name cannot be greater than 100 letters.";
        if (!containsOnlyAlphaCharacters(accName))
            // Throw an error if the account name contains things other than alphabetic characters and spaces
            throw "Account Number must start with a letter.";
        if (!isNumeric(cardNum))
            // Throw an error if the card number contained any other characters other than numbers
            throw "Card Number string must be a numeric string.";
        if (strlen(cardNum) != 10)
            // Throw an error if the card number did not consist of 10 digits
            throw "Card Number must be 10 numeric digits.";
        if (findAccountByCardNumber(cardNum))
            // Throw an error if an account with the given card number already exists
            throw "An account already exists with this card number";

        // Create the new account with the given valid data
        strcpy_s(accountName, accName);
        strcpy_s(cardNumber, cardNum);
        amount = 0;

        // Add this account to the accounts array
        ATM::accounts[ATM::numAccounts] = *this;
        ATM::numAccounts++;

        return &ATM::accounts[ATM::numAccounts - 1];
    }

    void deposit(long double addedMoney) {
        amount += addedMoney * 1.01;
    }

    bool withdrawal(long double withdrawnMoney) {
        // If the amount in the account is less than the amount needed for withdrawal
        // return false to signal a failed withdrawal operation
        if (withdrawnMoney * 1.03 > amount)
            return false;

        // Otherwise return true to signal a successful withdrawal operation
        amount -= withdrawnMoney * 1.03;
        return true;
    }

    bool transfer(ATM *transferAcc, long double transferMoney) {
        // If the amount in the sender account is less than the amount needed for transfer
        // return false to signal a failed transfer operation
        ATM *acc1 = this, *acc2 = transferAcc;
        if (transferMoney * 1.015 > acc1->amount)
            return false;

        // Otherwise return true to signal a successful transfer operation
        acc1->amount -= transferMoney * 1.015;
        acc2->amount += transferMoney * 0.985;
        return true;
    }

    void print_acc_data() {
        cout << "Name: " << accountName << endl;
        cout << "Card Number: " << cardNumber << endl;
        cout << "Amount: " << amount << endl;
    }

    // A boolean function that checks if a certain account is empty or not
    bool isEmpty() {
        if (amount == 0) return true;
        return false;
    }
};

ATM ATM::accounts[100];
int ATM::numAccounts = 0;

int main() {
    int operation;
    bool exit = false;
    while (!exit) {
        // Program Menu Options
        cout << "1----Create New Account" << endl;
        cout << "2----Deposit" << endl;
        cout << "3----Transfer to Another Account" << endl;
        cout << "4----Withdrawal" << endl;
        cout << "5----Exit" << endl;

        inputNumber("Please Enter the Number of Operation: ", operation, "Invalid Input for Operation Number. Only a number from 1 to 5 is allowed.\n");

        switch (operation) {
        case 1:
            // If the number of accounts is 100
            // Forbid the user from creating more accounts
            if (ATM::numAccounts == 100)
                cout << "Maximum number of accounts reached. You cannot create more than 100 accounts." << endl;
            else {
                ATM newAccount;
                char accountName[101];
                char cardNumber[11];

                bool valid = false;

                while (!valid) {
                    cout << "Please Enter the Account Name: ";
                    cin.getline(accountName, 101);
                    // If the user inputs leading spaces or repeated spaces between words this function is used to remove them
                    removeUnwantedSpaces(accountName);
                    // Check if the user entered an empty string
                    if (strlen(accountName) == 0) {
                        cout << "You must Enter a Name." << endl;
                        continue;
                    }
                    if (!cin) {
                        // Failure may happen if the user enters more than 100 characters
                        // If failure occurred remove this failure and try again
                        removeFailure();
                        cout << "Invalid Name. The maximum number of allowed characters is 100." << endl;
                        continue;
                    }
                    else if (!containsOnlyAlphaCharacters(accountName))
                        // Check if the user entered only alphabetic characters and spaces
                        cout << "Invalid Name. Account Name must contain only alphabetic characters." << endl;
                    else
                        valid = true;
                }

                valid = false;

                while (!valid) {
                    cout << "Please Enter the Account Number (10 numbers): ";
                    cin.getline(cardNumber, 11);
                    if (isNumeric(cardNumber) && strlen(cardNumber) == 10 && cin)
                        if (ATM::findAccountByCardNumber(cardNumber))
                            cout << "An account with this card number already exists." << endl;
                        else if (allSame(cardNumber))
                            cout << "Card Number is too easy. Please try a much harder card number to guess." << endl;
                        else
                            valid = true;
                    else {
                        if (!cin) removeFailure();
                        cout << "Invalid Card Number. Card Number must be a sequence of 10 numeric characters." << endl;
                    }
                }

                newAccount.create_new_acc(accountName, cardNumber)->print_acc_data();
                cout << endl;
            }
            break;
        case 2: {
            // To deposit in an account you need at least to have .... uhmmm guess what? An account first? :)
            if (ATM::numAccounts < 1) {
                cout << "You need to create at least 1 account." << endl << endl;
                continue;
            }
            char cardNumber[11];
            ATM* account = NULL;
            bool valid = false;
            while (!valid) {
                cout << "Please Enter the Account Number (10 numbers): ";
                cin.getline(cardNumber, 11);
                if (!cin) {
                    removeFailure();
                    cout << "The card number you entered does not exist" << endl;
                    continue;
                }
                account = ATM::findAccountByCardNumber(cardNumber);
                if (!account)
                    cout << "The card number you entered does not exist." << endl;
                else
                    valid = true;
            }

            long double amount;
            while (true) {
                inputNumber("Please Enter the Amount to be deposited: ", amount, "Invalid Input. The amount must be a valid positive number\n");

                if (amount <= 0)
                    cout << "Invalid Input. The amount to be deposited must be a valid positive number." << endl;
                else
                    break;
            }

            account->deposit(amount);
            account->print_acc_data();
            cout << endl;
        } break;
        case 3: {
            // To transfer to another account you need to have at least two different accounts
            if (ATM::numAccounts < 2) {
                cout << "You need to create at least 2 accounts." << endl << endl;
                continue;
            }
            if (ATM::allEmpty()) {
                cout << "You need to deposit in at least one account." << endl << endl;
                continue;
            }
            ATM *senderAccount = NULL, *recieverAccount = NULL;
            ATM* account = NULL;
            char cardNumber[11];
            bool valid = false;
            while (!valid) {
                cout << "Please Enter the Number of the Sending Account (10 numbers): ";
                cin.getline(cardNumber, 11);
                if (!cin) {
                    removeFailure();
                    cout << "The card number you entered does not exist" << endl;
                    continue;
                }
                account = ATM::findAccountByCardNumber(cardNumber);
                if (!account)
                    cout << "The card number you entered does not exist." << endl;
                else if (account->isEmpty())
                    cout << "You cannot transfer from an empty account." << endl;
                else {
                    valid = true;
                    senderAccount = account;
                }
            }

            account = NULL;
            valid = false;
            while (!valid) {
                cout << "Please Enter the Number of the Receiving Account (10 numbers): ";
                cin.getline(cardNumber, 11);
                if (!cin) {
                    removeFailure();
                    cout << "The card number you entered does not exist" << endl;
                    continue;
                }
                account = ATM::findAccountByCardNumber(cardNumber);
                if (!account)
                    cout << "The card number you entered does not exist" << endl;
                else if (senderAccount != account) {
                    valid = true;
                    recieverAccount = account;
                } else
                    cout << "The sender account cannot be the receiver account" << endl;
            }

            long double amount;
            while (true) {
                inputNumber("Please Enter the Amount to be transferred: ", amount, "Invalid Input. The amount must be a valid positive number\n");

                if (amount <= 0)
                    cout << "Invalid Input. The amount must be a valid positive number" << endl;
                else if (senderAccount->transfer(recieverAccount, amount))
                    break;
                else
                    cout << "This amount cannot be transferred." << endl;
            }

            cout << "Sending Account:" << endl;
            senderAccount->print_acc_data();
            cout << "Receiving Account:" << endl;
            recieverAccount->print_acc_data();
            cout << endl;
        } break;

        case 4: {
            // To withdraw from an account you need at least to have .... uhmmm guess what? An account first? :)
            if (ATM::numAccounts < 1) {
                cout << "You need to create at least 1 account." << endl << endl;
                continue;
            }

            if (ATM::allEmpty()) {
                cout << "You need to deposit in at least one account." << endl << endl;
                continue;
            }
            char cardNumber[11];
            ATM* account = NULL;
            bool valid = false;
            while (!valid) {
                cout << "Please Enter the Account Number (10 numbers): ";
                cin.getline(cardNumber, 11);
                if (!cin) {
                    removeFailure();
                    cout << "The card number you entered does not exist" << endl;
                    continue;
                }
                account = ATM::findAccountByCardNumber(cardNumber);
                if (!account)
                    cout << "The card number you entered does not exist." << endl;
                else if (account->isEmpty())
                    cout << "You cannot withdraw from an empty account." << endl;
                else
                    valid = true;
            }

            long double amount;
            while (true) {
                inputNumber("Please Enter the Amount to be withdrawn: ", amount, "Invalid Input. The amount must be a valid positive number\n");

                if (amount <= 0)
                    cout << "Invalid Input. The amount must be a valid positive number" << endl;
                else if (account->withdrawal(amount))
                    break;
                else
                    cout << "This amount cannot be withdrawn." << endl;
            }
            account->print_acc_data();
            cout << endl;
        } break;

        case 5:
            // Exit the program
            exit = true;
            break;

        default:
            // If the user enters an invalid number show him the menu again
            cout << "Invalid Operation Number." << endl << endl;
            break;
        }
    }

    return 0;
}

// This function is used to remove cin from the failure mode
void removeFailure() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// This function is used to check whether a certain string contains only spaces and alphabetic characters or not
bool containsOnlyAlphaCharacters(char str[]) {
    for (int i = 0; str[i]; i++)
        if (str[i] != 32 && !(str[i] >= 65 && str[i] <= 91) && !(str[i] >= 97 && str[i] <= 123))
            return false;

    return true;
}

// This function is used to check whether a certain string contains only digits or not
bool isNumeric(char str[]) {
    for (int i = 0; str[i]; i++)
        if (str[i] < 48 || str[i] > 57)
            return false;

    return true;
}

// This function is used on strings to remove leading whitespace and repeated spaces between words in Account Names
void removeUnwantedSpaces(char* str) {
	bool inWord = false;
	for (int i = 0; str[i]; i++) {
		if (!inWord) {
			if (str[i] == 32) {
				for (int j = i; str[j]; j++)
					str[j] = str[j + 1];
				i--;
			} else
				inWord = true;
			continue;
		}
		if (str[i] == 32) inWord = false;
	}
}
