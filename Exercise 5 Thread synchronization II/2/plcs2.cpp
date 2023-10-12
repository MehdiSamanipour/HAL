#include <iostream>
#include <vector>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib> // for rand() function
using namespace std;
pthread_mutex_t entry_gate;
pthread_mutex_t exit_gate;
pthread_cond_t entry_cond;
pthread_cond_t exit_cond;
bool entry_open = true;
bool exit_open = true;
queue<int> car_queue; // queue to keep track of car thread ids
pthread_mutex_t queue_mutex; // mutex lock for the queue
void *car_thread(void *arg) {
   int car_id = *(int*)arg;
    while (true) {
        pthread_mutex_lock(&entry_gate);  // Enter the parking lot
        while (!entry_open) {
        pthread_cond_wait(&entry_cond, &entry_gate);}
        entry_open = false;
        pthread_mutex_unlock(&entry_gate);
        pthread_mutex_lock(&queue_mutex);  // Add the car to the queue
        car_queue.push(car_id);
        pthread_mutex_unlock(&queue_mutex);
        int wait_time = rand() % 5 + 1;   // Wait in the parking lot for a random amount of time between 1 to 5 seconds
        cout << "car " << car_id << " enters the parking lot and waits for " << wait_time << " seconds" << endl;
        sleep(wait_time);
        pthread_mutex_lock(&exit_gate);    // Exit the parking lot
        while (!exit_open) {
        pthread_cond_wait(&exit_cond, &exit_gate); }
        exit_open = false;
        pthread_mutex_unlock(&exit_gate);
        pthread_mutex_lock(&queue_mutex);  // Remove the car from the queue
        car_queue.pop();
        pthread_mutex_unlock(&queue_mutex);
        cout << "car " << car_id << " exits the parking lot" << endl;
        wait_time = rand() % 5 + 1;   // Wait a random amount of time between 1 to 5 seconds before re-entering the parking lot
        sleep(wait_time);
        pthread_mutex_lock(&entry_gate);  // Signal that the entry gate is open
        entry_open = true;
        pthread_cond_signal(&entry_cond);
        pthread_mutex_unlock(&entry_gate);
        pthread_mutex_lock(&exit_gate);  // Signal that the exit gate is open
        exit_open = true;
        pthread_cond_signal(&exit_cond);
        pthread_mutex_unlock(&exit_gate);
        car_id++;  }} // Increment the car ID
int main() {
    pthread_mutex_init(&entry_gate, NULL);     // Initialize the mutexes and condition variables
    pthread_mutex_init(&exit_gate, NULL);
    pthread_cond_init(&entry_cond, NULL);
    pthread_cond_init(&exit_cond, NULL);
    pthread_mutex_init(&queue_mutex, NULL);
    vector<pthread_t> car_thread_ids;              // Create a vector to keep track of all the car thread ids
    for (size_t i = 1; i <= 10; i++) {       // Create 10 car threads with ids from 1 to 10
        pthread_t car_thread_id;
        int* car_id_ptr = new int(i);
        pthread_create(&car_thread_id, NULL, car_thread, (void*)car_id_ptr);
        car_thread_ids.push_back(car_thread_id);}
    for (int i = 1; i < car_thread_ids.size(); i++) {   // Wait for all cars to complete
        pthread_join(car_thread_ids[i+1], NULL); }
    pthread_mutex_destroy(&entry_gate);   // Destroy the mutexes and condition variables
    pthread_mutex_destroy(&exit_gate);
    pthread_cond_destroy(&entry_cond);
    pthread_cond_destroy(&exit_cond);
    pthread_mutex_destroy(&queue_mutex);
    return 0;}


