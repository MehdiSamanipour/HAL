#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;
pthread_mutex_t entry_gate; // mutex for controlling entry to parking lot
pthread_mutex_t exit_gate;  // mutex for controlling exit from parking lot
pthread_cond_t entry_cond;  // condition variable for entry gate
pthread_cond_t exit_cond;   // condition variable for exit gate
bool entry_open = true;     // flag for indicating if entry gate is open
bool exit_open = true;      // flag for indicating if exit gate is open
void *car_thread(void *arg)
{
    int car_id = *(int *)arg; // get car ID from argument passed to thread
    while (true)
    {
        // Enter the parking lot
        pthread_mutex_lock(&entry_gate); // acquire lock on entry gate mutex
        while (!entry_open)
        { // if entry gate is closed, wait on condition variable
            pthread_cond_wait(&entry_cond, &entry_gate);
        }
        entry_open = false;                // set entry gate to closed
        pthread_mutex_unlock(&entry_gate); // release lock on entry gate mutex
        cout << "Car " << car_id << " enters the parking lot" << endl;
        sleep(3); // Car stays in the parking lot for 3 seconds
        // Exit the parking lot
        pthread_mutex_lock(&exit_gate); // acquire lock on exit gate mutex
        while (!exit_open)
        { // if exit gate is closed, wait on condition variable
            pthread_cond_wait(&exit_cond, &exit_gate);
        }
        exit_open = false;                // set exit gate to closed
        pthread_mutex_unlock(&exit_gate); // release lock on exit gate mutex
        cout << "Car " << car_id << " exits the parking lot" << endl;
        sleep(2); // Car waits 2 seconds before re-entering the parking lot
        // Signal that the entry gate is open
        pthread_mutex_lock(&entry_gate);   // acquire lock on entry gate mutex
        entry_open = true;                 // set entry gate to open
        pthread_cond_signal(&entry_cond);  // signal waiting threads on entry condition variable
        pthread_mutex_unlock(&entry_gate); // release lock on entry gate mutex
        // Signal that the exit gate is open
        pthread_mutex_lock(&exit_gate);   // acquire lock on exit gate mutex
        exit_open = true;                 // set exit gate to open
        pthread_cond_signal(&exit_cond);  // signal waiting threads on exit condition variable
        pthread_mutex_unlock(&exit_gate); // release lock on exit gate mutex
    }
}
int main()
{
    // Initialize the mutexes and condition variables
    pthread_mutex_init(&entry_gate, NULL); // initialize entry gate mutex with default attributes
    pthread_mutex_init(&exit_gate, NULL);  // initialize exit gate mutex with default attributes
    pthread_cond_init(&entry_cond, NULL);  // initialize entry condition variable with default attributes
    pthread_cond_init(&exit_cond, NULL);   // initialize exit condition variable with default attributes
   // Create a thread for a car
    pthread_t car_thread_id;                                   // declare car thread ID
    int car_id = 1;                                            // declare car ID
    pthread_create(&car_thread_id, NULL, car_thread, &car_id); // create car thread with ID passed as argument
    // Wait for the car thread to finish (this will never happen in this example)
    pthread_join(car_thread_id, NULL); // wait for car thread to exit
    // Destroy the mutexes and condition variables
    pthread_mutex_destroy(&entry_gate);
    pthread_mutex_destroy(&exit_gate);
    pthread_cond_destroy(&entry_cond);
    pthread_cond_destroy(&exit_cond);
    return 0;
}
