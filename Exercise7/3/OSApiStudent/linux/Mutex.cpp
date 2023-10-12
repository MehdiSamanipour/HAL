#include <osapi/Mutex.hpp>

namespace osapi
{
    Mutex::Mutex()
    {
        // Initialize mutex using pthread_mutex_init() function
        if (pthread_mutex_init(&mutex_, NULL) != 0)
            throw MutexError("Failed to initialize mutex.");
    }

    Mutex::~Mutex()
    {
        // Destroy mutex using pthread_mutex_destroy() function
        pthread_mutex_destroy(&mutex_);
    }

    void Mutex::lock()
    {
        // Lock mutex using pthread_mutex_lock() function
        if (pthread_mutex_lock(&mutex_) != 0)
            throw MutexError("Failed to lock mutex.");
    }

    void Mutex::unlock()
    {
        // Unlock mutex using pthread_mutex_unlock() function
        if (pthread_mutex_unlock(&mutex_) != 0)
            throw MutexError("Failed to unlock mutex.");
    }

    bool Mutex::tryLock()
    {
        // Try to lock mutex using pthread_mutex_trylock() function
        int result = pthread_mutex_trylock(&mutex_);
        if (result == 0)
            return true;
        else if (result == EBUSY)
            return false;
        else
            throw MutexError("Failed to try lock mutex.");
    }
}
