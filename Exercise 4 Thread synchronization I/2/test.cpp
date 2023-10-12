#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "Vector.hpp" // Include the header file for the Vector class
using namespace std;
pthread_mutex_t mut; // Define a mutex variable for thread synchronization
void* writer(void* arg)
{
    pthread_mutex_lock(&mut); // Lock the mutex to ensure mutual exclusion
    if ((*(Vector*)arg).setAndTest(*(int*)pthread_self()) == true) // Call setAndTest() method of Vector class to set the value and check consistency
    {
        cout << "Thread [" << pthread_self() << "] " << "success" << endl; // Print success message
    }
    else 
    {
        cout << "Thread [" << pthread_self() << "] " << "Incosistency detected" <<false << endl; // Print failure message
    }
    sleep(1); // Sleep for one second
    pthread_mutex_unlock(&mut); // Unlock the mutex
    pthread_exit(NULL); // Exit the thread
}
int main(int argc, char *argv[])
{
    int size ;
    Vector v1; // Declare an instance of Vector class
    pthread_mutex_init(&mut,NULL); // Initialize the mutex variable
    cout << "enter a number between 1 and 100" << endl;
    cin >> size;
    pthread_t arr[size]; // Declare an array of threads
    for(int i=0;i<=size;i++){
        pthread_create(&arr[i],NULL,writer,&v1); // Create a thread and pass the Vector object as argument
        sleep(1); // Sleep for one second
    }
    for(int i=0;i<size;i++){
        pthread_join(arr[i],NULL); // Wait for each thread to complete
    }
    return 0;
}
