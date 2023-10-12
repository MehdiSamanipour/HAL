#ifndef OSAPI_SEMAPHORE_ERROR_HPP_  // Header guard to prevent multiple inclusion
#define OSAPI_SEMAPHORE_ERROR_HPP_

#include <osapi/Exceptions.hpp>  // Include the Exceptions.hpp header

namespace osapi
{  
  class SemaphoreError : public SystemError  // Declaration of SemaphoreError class which inherits from SystemError
  {
  public:
    SemaphoreError()  // Constructor for SemaphoreError class
      : SystemError("Some semaphore error occurred!")  // Initialize the base class SystemError with an error message
    {}
    
  };
}

#endif  // End of header guard
