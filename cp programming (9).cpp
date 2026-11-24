#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

/**
 * @brief Observer interface.
 */
class Observer
{
public:
    virtual void notify(const string& message) = 0;
    virtual ~Observer() {}
};

/**
 * @brief Email notification observer.
 */
class EmailNotification : public Observer
{
private:
    string email;

public:
    EmailNotification(string e)
    {
        email = e;
    }

    void notify(const string& message) override
    {
        cout << "Email sent to " << email
             << ": " << message << endl;
    }
};

/**
 * @brief SMS notification observer.
 */
class SMSNotification : public Observer
{
private:
    string phoneNumber;

public:
    SMSNotification(string number)
    {
        phoneNumber = number;
    }

    void notify(const string& message) override
    {
        cout << "SMS sent to " << phoneNumber
             << ": " << message << endl;
    }
};

/**
 * @brief Factory class for creating notifications.
 */
class NotificationFactory
{
public:
    static shared_ptr<Observer> createNotification(
        const string& type,
        const string& receiver)
    {
        if (type == "EMAIL")
        {
            return make_shared<EmailNotification>(receiver);
        }
        else if (type == "SMS")
        {
            return make_shared<SMSNotification>(receiver);
        }

        return nullptr;
    }
};

/**
 * @brief Singleton Event Manager.
 */
class EventManager
{
private:
    vector<shared_ptr<Observer>> observers;

    // Private constructor prevents external object creation
    EventManager()
    {
    }

public:
    // Delete copy constructor and assignment operator
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    // Singleton access method
    static EventManager& getInstance()
    {
        static EventManager instance;
        return instance;
    }

    void subscribe(shared_ptr<Observer> observer)
    {
        observers.push_back(observer);
    }

    void triggerEvent(const string& message)
    {
        cout << "\nEvent Triggered: "
             << message << endl;

        cout << "Sending Notifications...\n";

        for (auto observer : observers)
        {
            observer->notify(message);
        }
    }
};

int main()
{
    // Access Singleton EventManager
    EventManager& manager = EventManager::getInstance();

    int n;
    cout << "Enter number of subscribers: ";
    cin >> n;

    if (cin.fail() || n < 1)
    {
        cout << "Invalid input. Number of subscribers must be at least 1." << endl;
        return 1;
    }

    // Create observers using Factory Pattern
    int count = 0;
    while (count < n)
    {
        string type, receiver;

        cout << "\nSubscriber " << count + 1 << endl;

        cout << "Enter notification type (EMAIL or SMS): ";
        cin >> type;

        cout << "Enter receiver (email address or phone number): ";
        cin >> receiver;

        if (cin.fail())
        {
            cout << "Invalid input." << endl;
            return 1;
        }

        // Accept lowercase too (email -> EMAIL)
        transform(type.begin(), type.end(), type.begin(), ::toupper);

        shared_ptr<Observer> observer =
            NotificationFactory::createNotification(type, receiver);

        if (observer == nullptr)
        {
            cout << "Invalid type. Please enter EMAIL or SMS." << endl;
            continue;   // ask for this subscriber again
        }

        // Subscribe observer
        manager.subscribe(observer);
        count++;
    }

    // Read the event message (spaces allowed)
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string message;
    cout << "\nEnter event message: ";
    getline(cin, message);

    if (message.empty())
    {
        cout << "Message cannot be empty." << endl;
        return 1;
    }

    // Trigger event
    manager.triggerEvent(message);

    return 0;
}