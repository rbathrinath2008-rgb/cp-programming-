#include <iostream>
#include <stdexcept>
#include <string>
#include <limits>
using namespace std;

// Base custom exception class
class BankException : public exception
{
protected:
    string message;

public:
    BankException(string msg)
    {
        message = msg;
    }

    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

// Derived exception class
class InsufficientFundsException : public BankException
{
public:
    InsufficientFundsException()
        : BankException("Error: Insufficient balance for withdrawal.")
    {
    }
};

// Another derived exception class
class InvalidAmountException : public BankException
{
public:
    InvalidAmountException()
        : BankException("Error: Amount must be greater than zero.")
    {
    }
};

class BankAccount
{
private:
    string name;
    double balance;

public:
    BankAccount(string n, double b)
    {
        name = n;
        balance = b;
    }

    void deposit(double amount)
    {
        if (amount <= 0)
            throw InvalidAmountException();

        balance += amount;
    }

    void withdraw(double amount)
    {
        // Validate first before modifying the balance
        if (amount <= 0)
            throw InvalidAmountException();

        if (amount > balance)
            throw InsufficientFundsException();

        // State changes only after all checks succeed
        balance -= amount;
    }

    void display()
    {
        cout << "Account Holder: " << name << endl;
        cout << "Balance: Rs. " << balance << endl;
    }
};

// Reads an amount from the user. If the input is not a number,
// it returns 0 so the account rejects it as an invalid amount.
double readAmount(const string &prompt)
{
    double amount;
    cout << prompt;
    cin >> amount;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return 0;
    }
    return amount;
}

int main()
{
    string name;
    double initialBalance;

    cout << "Enter account holder name: ";
    cin >> name;

    cout << "Enter initial balance: ";
    cin >> initialBalance;

    if (cin.fail() || initialBalance < 0)
    {
        cout << "Invalid input. Initial balance must be a number, 0 or more." << endl;
        return 1;
    }

    BankAccount account(name, initialBalance);

    cout << "\nInitial Account Details:" << endl;
    account.display();

    int choice;
    do
    {
        cout << "\n1. Deposit" << endl;
        cout << "2. Withdraw" << endl;
        cout << "3. Display details" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter 1 to 4." << endl;
            choice = 0;
            continue;
        }

        try
        {
            switch (choice)
            {
            case 1:
                account.deposit(readAmount("Enter amount to deposit: "));
                cout << "Deposit successful." << endl;
                break;

            case 2:
                account.withdraw(readAmount("Enter amount to withdraw: "));
                cout << "Withdrawal successful." << endl;
                break;

            case 3:
                account.display();
                break;

            case 4:
                break;

            default:
                cout << "Invalid choice. Please enter 1 to 4." << endl;
            }
        }
        catch (const InsufficientFundsException &e)
        {
            cout << e.what() << endl;
        }
        catch (const InvalidAmountException &e)
        {
            cout << e.what() << endl;
        }
        catch (const BankException &e)
        {
            cout << "Bank Exception: " << e.what() << endl;
        }
        catch (const exception &e)
        {
            cout << "General Exception: " << e.what() << endl;
        }

    } while (choice != 4);

    cout << "\nFinal Account Details:" << endl;
    account.display();

    return 0;
}