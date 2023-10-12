#include <pthread.h>
#include <iostream>
#include <unistd.h>

// Initialize a static mutex variable
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

// Thread function that prints a message to the console
void *funcId(void *arg)
{
    // Cast the argument to an integer pointer
    int time = *static_cast<int *>(arg);

    // Loop 10 times
    for (int i = 0; i <= 10; i++)
    {
        // Acquire the mutex lock
        pthread_mutex_lock(&mut);

        // Print a message to the console
        std::cout << " Hello "
                  << "#" << i << " from thread : " << time << std::endl;

        // Release the mutex lock
        pthread_mutex_unlock(&mut);

        // Sleep for one second
        sleep(1);
    }

    // Exit the thread
    pthread_exit(NULL);
}

int main()
{
    // Declare two thread IDs
    pthread_t t1, t2;

    // Declare two integer variables to pass as arguments to the threads
    int num1 = 0;
    int num2 = 1;

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
