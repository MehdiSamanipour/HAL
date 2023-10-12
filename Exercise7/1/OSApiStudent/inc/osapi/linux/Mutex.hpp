#ifndef OSAPI_LINUX_MUTEX_HPP
#define OSAPI_LINUX_MUTEX_HPP

#include <osapi/Utility.hpp>
#include <pthread.h>

namespace osapi
{
    class Conditional;

    class Mutex : Notcopyable 
    {
    public:
        Mutex();                    // Constructor
        ~Mutex();                   // Destructor

        void lock();                // Lock the mutex
        void unlock();              // Unlock the mutex
        bool tryLock();             // Try to lock the mutex, return false if it's already locked

    private:
        friend class Conditional;   // Allow Conditional class to access the private members of Mutex
        pthread_mutex_t mutex_;     // Mutex object
    };
}

#endif // OSAPI_LINUX_MUTEX_HPP
