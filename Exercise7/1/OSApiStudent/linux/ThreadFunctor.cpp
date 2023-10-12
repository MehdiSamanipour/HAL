#include <osapi/Thread.hpp>

namespace osapi
{
  void* ThreadFunctor::threadMapper(void* thread)
  {
    ThreadFunctor* tf = static_cast<ThreadFunctor*>(thread); // Cast the void* to ThreadFunctor*
    tf->run(); // Call the run() method of ThreadFunctor
    tf->threadDone_.signal(); // Signal that the thread has completed
    return NULL;
  }

}
