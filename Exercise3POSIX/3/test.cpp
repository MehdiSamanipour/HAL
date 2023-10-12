#include <iostream>
#include <pthread.h>
#include<unistd.h>
// Shared variable to be incremented and read
int sharedVariable = 0;
// Function to be executed in the incrementer thread
void *func_incrementer(void *arg)
{
    for (int i = 0; i < 1000; i++)
    {
        // Increment the shared variable
        sharedVariable++;
        sleep(1);   
    }
    pthread_exit(NULL);
}
// Function to be executed in the reader thread
void *func_reader(void*arg)
{
    // Read the shared variable 10 times
    for (int i = 0; i < 10; i++)
    {
        // Print the value of the shared variable
        std::cout << "Shared variable: " << sharedVariable << std::endl;
        sleep(1);
    }
    pthread_exit(NULL);
}
int main()
{
 pthread_t incrementer, reader;
    int ret;
    ret = pthread_create(&incrementer, NULL,func_incrementer, NULL);
    ret = pthread_create(&reader, NULL,func_reader, NULL);
    // Wait for both threads to finish
    pthread_join(incrementer, NULL);
    pthread_join(reader, NULL);
    return 0;
}
