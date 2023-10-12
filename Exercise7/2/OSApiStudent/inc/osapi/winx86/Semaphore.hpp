#ifndef OSAPI_WINX86_SEMAPHORE_HPP  // Header guard to prevent multiple inclusion
#define OSAPI_WINX86_SEMAPHORE_HPP

#include <osapi/Utility.hpp>  // Include the Utility.hpp header

namespace osapi
{
  namespace details
  {
    class Semaphore;  // Forward declaration of the details::Semaphore class
  }

  class Semaphore : Notcopyable  // Declaration of the Semaphore class which is not copyable
  {
  public:
    Semaphore(unsigned int initCount);  // Constructor to create a Semaphore object with an initial count
    void wait();  // Member function to wait on the semaphore
    void signal();  // Member function to signal the semaphore
    ~Semaphore();  // Destructor to clean up the Semaphore object
  private:
    details::Semaphore* sem_;  // Pointer to the details::Semaphore object
  };
}

#endif  // End of header guard
