#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "Vector.hpp"
using namespace std;

void* writer(void* arg)
{
    Vector& v = *(Vector*)arg;
    if (v.setAndTest(*(int*)pthread_self()) == true) {
        cout << "Thread [" << pthread_self() << "] " << "success" << endl;
    }
    else {
        cout << "Thread [" << pthread_self() << "] " << "Incosistency detected" << false << endl;
    }
    sleep(1);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int size ;
    Vector v1;
    pthread_mutex_t mut;
    pthread_mutex_init(&mut,NULL);
    cout << "enter a number between 1 and 100" << endl;
    cin >> size;
    pthread_t arr[size];
    for(int i=0;i<size;i++) {
        pthread_create(&arr[i], NULL, writer, &v1);
        sleep(1);
    }
    for(int i=0;i<size;i++) {
        pthread_join(arr[i], NULL);
    }
    pthread_mutex_destroy(&mut);
    return 0;
}
