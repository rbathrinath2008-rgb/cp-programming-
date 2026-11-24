#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <chrono>

using namespace std;

queue<int> buffer;
mutex mtx;
condition_variable cv;

atomic<int> producedCount(0);
atomic<bool> finished(false);

// Producer function
void producer(int maxItems, int delayMs)
{
    for (int i = 1; i <= maxItems; i++)
    {
        this_thread::sleep_for(
            chrono::milliseconds(delayMs));

        {
            lock_guard<mutex> lock(mtx);

            buffer.push(i);
            producedCount++;

            cout << "Produced: " << i << endl;
        }

        cv.notify_one();
    }

    finished = true;
    cv.notify_all();
}

// Consumer function
void consumer()
{
    while (true)
    {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, []
        {
            return !buffer.empty() || finished;
        });

        if (buffer.empty() && finished)
            break;

        int item = buffer.front();
        buffer.pop();

        lock.unlock();

        cout << "Consumed: " << item << endl;
    }
}

int main()
{
    int maxItems, delayMs;

    cout << "Enter number of items to produce: ";
    cin >> maxItems;

    cout << "Enter delay between items in milliseconds: ";
    cin >> delayMs;

    if (cin.fail() || maxItems < 1 || delayMs < 0)
    {
        cout << "Invalid input. Items must be at least 1 and delay cannot be negative." << endl;
        return 1;
    }

    cout << "\nProducer-Consumer System Started\n"
         << endl;

    thread producerThread(producer, maxItems, delayMs);
    thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    cout << "\nTotal Items Produced: "
         << producedCount << endl;

    cout << "Producer-Consumer System Completed"
         << endl;

    return 0;
}