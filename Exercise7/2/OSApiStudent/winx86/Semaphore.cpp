
#include <osapi/Semaphore.hpp> // Include the Semaphore header from OSApi library
#include <osapi/winx86/details/Semaphore.hpp> // Include the platform-specific Semaphore header for WinX86

namespace osapi
{  
  Semaphore::Semaphore(unsigned int initCount)
    : sem_(new details::Semaphore(initCount)) // Initialize the semaphore with the platform-specific Semaphore implementation
  {
  }


  void Semaphore::wait()
  {
    sem_->wait(); // Call the wait() function on the platform-specific Semaphore implementation
  }


  void Semaphore::signal()
  {
    sem_->signal(); // Call the signal() function on the platform-specific Semaphore implementation
  }


  Semaphore::~Semaphore()
  {
    // Needed for incomplete type - MUST be in the cpp file where the pimpl is known!!!
    delete sem_; // Delete the platform-specific Semaphore implementation to prevent memory leaks
  }

}
