#include <osapi/Thread.hpp>  // Include the Thread header file
#include <osapi/ThreadFunctor.hpp>  // Include the ThreadFunctor header file
namespace osapi
{
  // Implementation of threadMapper function
  DWORD WINAPI ThreadFunctor::threadMapper(void* thread)
  {
    ThreadFunctor* tf = static_cast<ThreadFunctor*>(thread); // Cast the void pointer to ThreadFunctor*
    tf->run(); // Call the run() method on ThreadFunctor object
    tf->threadDone_.signal(); // Signal that the thread has finished
    return NULL;
  }
}
