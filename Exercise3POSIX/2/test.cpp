#include <pthread.h>
#include <iostream>
#include <unistd.h>

void *funcId(void *arg)
{


    for (int i = 1; i <=10; i++)
    {
       std::cout << "Thread ID:  "<<*(long *)arg << ", print " << i << " times\n";
        i++;
        sleep(1);
    }

    pthread_exit(NULL);
}
int main()
{
    pthread_t t1, t2;
    int ret;
    ret = pthread_create(&t1, NULL, funcId, (void *)&t1); 
    ret = pthread_create(&t2, NULL, funcId, (void *)&t2); 
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}

