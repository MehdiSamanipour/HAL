#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include<thread>
#include "Vector.hpp" 
using namespace std;
Vector vec1;
struct thread_ms
{
    int ID, delay;
    
};
void*writer(void*arg)
{
    thread_ms * args = static_cast<thread_ms*>(arg); 
    int ID = args->ID;
    int delay = args->delay;
    while (1){
    bool element = vec1.setAndTest(ID);
    if(element)
    cout << "Thread [" << ID << "] " << "Succes" << endl;
    else 
        cout << "Thread [" << ID << "] " << "Incosistency detected" <<false << endl;
        
    sleep(delay); 
    }
    pthread_exit(NULL);
        }

int main()
{

    int size, time;
    cout << "Type number of threads between 1-100: ";
    cin >> size;
    cout << "Type time in ms: ";
    cin >> time;
    time=time/1000;
    pthread_t arr[size];
    for(int i = 0; i < size; i++)
    {
        pthread_create(&arr[i], NULL, writer, new thread_ms{i,time});
    }
    for(unsigned int i = 0; i < size; i++)
    {
        pthread_join(arr[i], NULL);
    }
    return 0;
}

