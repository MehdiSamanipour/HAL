#include <pthread.h> // for POSIX threads
#include <iostream> // for console output
#include <unistd.h> // for sleep function
#include <semaphore.h> // for semaphore

static sem_t semaphr1; // Declare a semaphore

// Thread function that prints a message to the console
void *funcId(void *arg)
{
    // Cast the argument to an integer pointer
    int time =*static_cast<int*>(arg); 

    // Loop 10 times
    for (int i = 0; i <=10; i++)
    {
       sem_wait(&semaphr1); // Wait for the semaphore to be available
        // Print a message to the console
        std::cout << " Hello " <<  "#"<<i<<  " from thread : "  <<  time << std::endl;

       sem_post(&semaphr1); // Release the semaphore

        // Sleep for one second
        sleep(1);
    }

    // Exit the thread
    pthread_exit(NULL);
}

int main()
{
    sem_init(&semaphr1,0,1); // Initialize the semaphore

    // Declare two thread IDs
    pthread_t t1, t2;

    // Declare two integer variables to pass as arguments to the threads
    int num1=0;
    int num2=1;

    // Create the first thread, passing in the integer variable as the argument
    int ret = pthread_create(&t1, NULL, funcId, (void *)&num1);

    // Create the second thread, passing in the other integer variable as the argument
    ret = pthread_create(&t2, NULL, funcId, (void *)&num2);

    // Wait for both threads to complete
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Exit the program
    return 0;
}
