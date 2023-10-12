#ifndef VECTOR_HPP_
#define VECTOR_HPP_
#include <pthread.h>
#include <mutex>
class ScopedLocker {
public:
    explicit ScopedLocker(pthread_mutex_t& mutex) : mutex_(mutex) {
        pthread_mutex_lock(&mutex_);
    }
    ~ScopedLocker() {
        pthread_mutex_unlock(&mutex_);
    }
private:
    pthread_mutex_t& mutex_;
};
class Vector {
public:
    Vector(unsigned int size = 10000) : size_(size)
    {
        pthread_mutex_init(&mut_, NULL);
        vector_ = new int[size_];
        set(0);
    }
    ~Vector()
    {
        delete[] vector_;
        pthread_mutex_destroy(&mut_);
    }
    bool setAndTest(int n)
    {
        ScopedLocker lock(mut_);
        set(n);
        bool res = test(n);
        return res;
    }
private:
    void set(int n)
    {
        for (unsigned int i = 0; i < size_; i++)
            vector_[i] = n;
    }
    bool test(int n)
    {
        for (unsigned int i = 0; i < size_; i++)
            if (vector_[i] != n)
                return false;
        return true;
    }
    int* vector_;
    unsigned int size_;
    pthread_mutex_t mut_;
};

#endif
