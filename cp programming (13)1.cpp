#include <iostream>
#include <vector>
#include <concepts>
#include <ranges>

using namespace std;

// Concept to ensure type is numeric (integer or floating-point)
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

// Data processing function using C++20 ranges
template <Numeric T>
vector<T> processData(const vector<T>& data)
{
    vector<T> result;

    auto pipeline = data
        | views::filter([](T value) {
            return value > 0; // Filter out numbers <= 0
        })
        | views::transform([](T value) {
            return value * 2; // Double the remaining numbers
        });

    for (T value : pipeline)
    {
        result.push_back(value);
    }

    return result;
}

int main()
{
    int n;
    cout << "Enter the number of elements: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid number of elements." << endl;
        return 1;
    }

    vector<int> data(n);
    cout << "Enter " << n << " numbers (separated by spaces or newlines): ";
    for (int i = 0; i < n; ++i) {
        cin >> data[i];
    }

    // Process the user input data
    vector<int> result = processData(data);

    // Display the outputs
    cout << "\n----------------------------" << endl;
    cout << "Original Data: ";
    for (int value : data)
    {
        cout << value << " ";
    }
    
    cout << "\nProcessed Data (filtered > 0, doubled): ";
    for (int value : result)
    {
        cout << value << " ";
    }
    cout << "\n----------------------------" << endl;

    return 0;
}