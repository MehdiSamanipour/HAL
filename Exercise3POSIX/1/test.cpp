#include <iostream>
#include <thread>
#include <mutex>
std::mutex mutex;
// Shared variable to be incremented and read
int sharedVariable = 0;
// Function to be executed in the incrementer thread
void incrementer()
{
    for (int i = 0; i < 1000; i++)
    {
        // Lock the mutex to prevent concurrent access to the shared variable
        mutex.lock();

        // Increment the shared variable
        sharedVariable++;

        // Unlock the mutex
        mutex.unlock();
    }
}
// Function to be executed in the reader thread
void reader()
{
    // Read the shared variable 10 times
    for (int i = 0; i < 10; i++)
    {
        // Lock the mutex to prevent concurrent access to the shared variable
        mutex.lock();

        // Print the value of the shared variable
        std::cout << "Shared variable: " << sharedVariable << std::endl;

        // Unlock the mutex
        mutex.unlock();
    }
}
int main()
{
    // Create the incrementer thread and execute the incrementer function
    std::thread t1(incrementer);
    // Create the reader thread and execute the reader function
    std::thread t2(reader);
    // Wait for both threads to finish
    t1.join();
    t2.join();
    return 0;
}
