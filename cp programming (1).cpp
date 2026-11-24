#include <iostream>
#include <string> // Recommended for using string
using namespace std;

class Student
{
private:
    int rollNo;
    string name;

public:
    // Constructor
    Student(int r, string n)
    {
        rollNo = r;
        name = n;
        cout << "Student object created" << endl;
    }

    // Member function
    void display()
    {
        cout << "\n--- Student Details ---" << endl;
        cout << "Roll Number: " << rollNo << endl;
        cout << "Name: " << name << endl;
    }

    // Destructor
    ~Student()
    {
        cout << "Student object destroyed" << endl;
    }
};

int main()
{
    int r;
    string n;

    // Taking input from the user
    cout << "Enter Roll Number: ";
    cin >> r;

    // cin.ignore() is used to clear the input buffer 
    // so getline works correctly right after cin >>
    cin.ignore(); 

    cout << "Enter Name: ";
    getline(cin, n);

    // Creating the Student object using user input
    Student s1(r, n);
    
    // Displaying the details
    s1.display();

    return 0;
}