#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <cctype>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

// BankAccount class definition 
class BankAccount {
private:
    string accountNumber;
    string accountHolderName;
    double balance;

public:
    BankAccount(string accNumber, string accHolderName, double initialBalance);
    void deposit(double amount);
    void withdraw(double amount);
    double getBalance() const;
    string getAccountNumber() const;
    string getAccountHolderName() const;
};

// Loan struct definition
struct Loan {
    string name;
    double amount;
    double interestRate;
    int duration;
    string durationUnit;
    string startDate;
};

// Function prototypes
BankAccount::BankAccount(string accNumber, string accHolderName, double initialBalance)
    : accountNumber(accNumber),
      accountHolderName(accHolderName),
      balance(initialBalance) {}

void BankAccount::deposit(double amount) {                                      // Deposit amount to the account
    if (amount > 0) {
        balance += amount;
        cout << "Deposited: " << amount << endl;
    } else {
        cout << "Deposit amount must be positive." << endl;
    }
}

void BankAccount::withdraw(double amount) {                                     // Withdraw amount from the account
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        cout << "Withdrew: " << amount << endl;
    } else {
        cout << "Invalid withdrawal amount." << endl;
    }
}

double BankAccount::getBalance() const {
    return balance;
}

string BankAccount::getAccountNumber() const {
    return accountNumber;
}

string BankAccount::getAccountHolderName() const {
    return accountHolderName;
}

void displayAccountDetails(const BankAccount& account) {                        // Display account details
    cout << "Account Number: " << account.getAccountNumber() << endl;
    cout << "Account Holder Name: " << account.getAccountHolderName() << endl;
    cout << "Balance: " << account.getBalance() << endl;
}

bool isValidStartDate(const string& date) {                                     // Validate the start date format (MM/YYYY)
    if (date.size() != 7 || date[2] != '/') {
        return false;
    }

    for (size_t i = 0; i < date.size(); ++i) {
        if (i != 2 && !isdigit(static_cast<unsigned char>(date[i]))) {
            return false;
        }
    }

    int month = stoi(date.substr(0, 2));
    return month >= 1 && month <= 12;
}

int getDurationInMonths(const Loan& loan) {                                     // Convert loan duration to months
    if (loan.durationUnit == "month" || loan.durationUnit == "months") {
        return loan.duration;
    }
    return loan.duration * 12;
}

double calculateMonthlyEmi(const Loan& loan) {                                  // Calculate the monthly EMI
    int numberOfPayments = getDurationInMonths(loan);
    double monthlyInterestRate = loan.interestRate / 1200.0;

    if (monthlyInterestRate == 0) {
        return loan.amount / numberOfPayments;
    }

    double rateFactor = pow(1 + monthlyInterestRate, numberOfPayments);
    return loan.amount * monthlyInterestRate * rateFactor / (rateFactor - 1);
}

int getRemainingPayments(const Loan& loan) {                                    // Calculate the remaining payments
    time_t currentTime = time(nullptr);
    tm* currentDate = localtime(&currentTime);
    int currentYear = currentDate->tm_year + 1900;
    int currentMonth = currentDate->tm_mon + 1;

    int startMonth = stoi(loan.startDate.substr(0, 2));
    int startYear = stoi(loan.startDate.substr(3, 4));
    int paymentsDue = (currentYear - startYear) * 12 + (currentMonth - startMonth);
    int totalPayments = getDurationInMonths(loan);

    if (paymentsDue < 0) {
        paymentsDue = 0;
    }

    int remainingPayments = totalPayments - paymentsDue;
    return remainingPayments > 0 ? remainingPayments : 0;
}

void addLoanInformation(vector<Loan>& loans) {                                  // Add or update loan information
    int totalLoans;
    cout << "Enter total number of loans (0 if none): ";
    cin >> totalLoans;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (totalLoans < 0) {
        cout << "Number of loans cannot be negative." << endl;
        return;
    }

    loans.clear();                                                              // Clear all existing loan information before adding new loans
    for (int i = 0; i < totalLoans; ++i) {
        while (true) {
            Loan loan;
            cout << "\nLoan " << (i + 1) << " name: ";
            getline(cin, loan.name);
            cout << "Loan amount: ";
            cin >> loan.amount;
            cout << "Annual interest rate (percent): ";
            cin >> loan.interestRate;
            cout << "Loan duration (whole number): ";
            cin >> loan.duration;
            cout << "Duration unit (months/years): ";
            cin >> loan.durationUnit;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            for (char& character : loan.durationUnit) {
                character = static_cast<char>(tolower(static_cast<unsigned char>(character)));
            }

            if (loan.amount <= 0 || loan.interestRate < 0 || loan.duration <= 0 ||
                (loan.durationUnit != "month" && loan.durationUnit != "months" &&
                 loan.durationUnit != "year" && loan.durationUnit != "years")) {
                cout << "Enter a positive amount and duration, a nonnegative interest rate, "
                     << "and a duration unit of months or years. Please try again."
                     << endl;
                continue;
            }

            cout << "Loan start date (MM/YYYY): ";
            getline(cin, loan.startDate);
            while (!isValidStartDate(loan.startDate)) {
                cout << "Enter the start date in MM/YYYY format (month 01-12): ";
                getline(cin, loan.startDate);
            }

            loans.push_back(loan);
            break;
        }
    }

    cout << "\n Loan information saved for this run." << endl;
}

void displayLoanInformation(const vector<Loan>& loans) {                        // Display loan information
    if (loans.empty()) {
        cout << "\n No loan information has been entered." << endl;
        return;
    }

    cout << "\n--- Loan Information ---" << endl;
    for (size_t i = 0; i < loans.size(); ++i) {
        cout << "\nLoan " << (i + 1) << ": " << loans[i].name << endl;
        cout << "Amount: " << loans[i].amount << endl;
        cout << "Annual Interest Rate: " << loans[i].interestRate << "%" << endl;
        cout << "Duration: " << loans[i].duration << " "
             << loans[i].durationUnit << endl;
        cout << "Start Date: " << loans[i].startDate << endl;
        cout << "Monthly EMI: " << calculateMonthlyEmi(loans[i]) << endl;
        cout << "Estimated Payments Remaining: "
             << getRemainingPayments(loans[i]) << endl;
        cout << "Total amount to be paid over the loan duration: "
             << calculateMonthlyEmi(loans[i]) * getDurationInMonths(loans[i]) << endl;
        cout << "\n------------------------" << endl;
    }
}

void addNewLoan(vector<Loan>& loans) {                                          // Add a loan without replacing existing loans
    Loan loan;
    cout << "\nLoan name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, loan.name);
    cout << "Loan amount: ";
    cin >> loan.amount;
    cout << "Annual interest rate (percent): ";
    cin >> loan.interestRate;
    cout << "Loan duration (whole number): ";
    cin >> loan.duration;
    cout << "Duration unit (months/years): ";
    cin >> loan.durationUnit;

    for (char& character : loan.durationUnit) {
        character = static_cast<char>(tolower(static_cast<unsigned char>(character)));
    }

    if (loan.amount <= 0 || loan.interestRate < 0 || loan.duration <= 0 ||
        (loan.durationUnit != "month" && loan.durationUnit != "months" &&
         loan.durationUnit != "year" && loan.durationUnit != "years")) {
        cout << "Invalid loan amount, interest rate, duration, or duration unit."
             << endl;
        return;
    }

    cout << "Loan start date (MM/YYYY): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, loan.startDate);
    while (!isValidStartDate(loan.startDate)) {
        cout << "Enter the start date in MM/YYYY format (month 01-12): ";
        getline(cin, loan.startDate);
    }

    loans.push_back(loan);
    cout << "New loan information saved." << endl;
}

void performBankingOperations(BankAccount& account, vector<Loan>& loans) {      // Perform banking operations
    int choice;
    double amount;
    do {
        cout << "\nBanking Operations Menu:\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Check Balance\n";
        cout << "4. Add/Update Loan Information\n";
        cout << "5. View Loan Information\n";
        cout << "6. Add New Loan\n";
        cout << "7. Exit\n";
        cout << "\n Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "\n Enter amount to deposit: ";
                cin >> amount;
                account.deposit(amount);
                break;
            case 2:
                cout << "\n Enter amount to withdraw: ";
                cin >> amount;
                account.withdraw(amount);
                break;
            case 3:
                cout << "\n Current Balance: " << account.getBalance() << endl;
                break;
            case 4:
                addLoanInformation(loans);
                break;
            case 5:
                displayLoanInformation(loans);
                break;
            case 6:
                addNewLoan(loans);
                break;
            case 7:
                cout << "Exiting banking operations." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 7);
}

int main() {                                                                    // Main function to run the code
    string accNumber, accHolderName;
    double initialBalance;

    cout << fixed << setprecision(2);

    cout << "=== Welcome to the Banking System ===" << endl;
    cout << endl;

    cout << "Enter Account Number: ";
    getline(cin, accNumber);

    cout << "Enter Account Holder Name: ";
    getline(cin, accHolderName);

    cout << "Enter Initial Balance: ";
    cin >> initialBalance;

    BankAccount account(accNumber, accHolderName, initialBalance);
    vector<Loan> loans;

    cout << "\n--- Account Created Successfully! ---" << endl;
    displayAccountDetails(account);

    performBankingOperations(account, loans);

    cout << "\n--- Final Account Details ---" << endl;
    displayAccountDetails(account);

    cout << "\nThank you for using the Banking System!" << endl;
    return 0;
}
