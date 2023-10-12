#include <errno.h> // Include for error handling
#include <osapi/ConditionalError.hpp>
#include "osapi/linux/Mutex.hpp"
#include <osapi/linux/Conditional.hpp>
namespace osapi
{
  Conditional::Conditional()
  {
    if(pthread_condattr_init(&condattr_) != 0) throw ConditionalError(); // Initialize condition attribute and check for error
    if(pthread_condattr_setclock(&condattr_, CLOCK_MONOTONIC) != 0) throw ConditionalError(); // Set condition attribute clock type and check for error
    if(pthread_cond_init(&cond_, &condattr_) != 0) throw ConditionalError(); // Initialize condition variable with attribute and check for error
  }
  void Conditional::signal()
  {
    if(pthread_cond_signal(&cond_) != 0) throw ConditionalError(); // Signal condition variable and check for error
  }
  void Conditional::broadcast()
  {
    if(pthread_cond_broadcast(&cond_) != 0) throw ConditionalError(); // Broadcast condition variable and check for error
  }
  void Conditional::wait(Mutex& mut)
  {
    if(pthread_cond_wait(&cond_, &mut.mutex_) != 0) throw ConditionalError(); // Wait on condition variable with associated mutex and check for error
  }
  Conditional::Awoken Conditional::waitTimed(Mutex& mut, unsigned long timeout)
  {
    struct timespec ts;
      // Get monotonic clock - current time
    clock_gettime(CLOCK_MONOTONIC, &ts);
  // Calculate new absolute time by getting current monotonic time and offsetting it
    size_t secs = timeout/1000;
    size_t msecs = timeout - secs*1000;
    ts.tv_sec += secs;
    ts.tv_nsec += msecs*1000000;
    size_t overflow = ts.tv_nsec/1000000000;
      if(overflow)
    {
      ts.tv_sec += overflow;
      ts.tv_nsec -= overflow*1000000000;
    }  
  int res = pthread_cond_timedwait(&cond_, &mut.mutex_, &ts); // Wait on condition variable with associated mutex and absolute time, and check for error
        switch(res)
    {
      case ETIMEDOUT:
        return TIMEDOUT;
        break;
        
      case 0:
        return SIGNALED;
        break;

      default:
        throw ConditionalError();
    }
  }
  Conditional::~Conditional()
  {
    if(pthread_cond_destroy(&cond_) != 0) throw ConditionalError(); // Destroy condition variable and check for error
    if(pthread_condattr_destroy(&condattr_) != 0) throw ConditionalError(); // Destroy condition attribute and check for error
  }
}
