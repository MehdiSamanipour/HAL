#ifndef OSAPI_SEMAPHORE_HPP                    // Header guard to prevent multiple inclusion
#define OSAPI_SEMAPHORE_HPP

#include <osapi/Utility.hpp>                  // Include Utility.hpp header
#include <osapi/SemaphoreError.hpp>           // Include SemaphoreError.hpp header

#if defined(OS_WINX86)                       // Conditional compilation based on defined macros
#include "osapi/winx86/Semaphore.hpp"       // Include Semaphore.hpp for Windows x86
#elif defined(OS_LINUX)
#include "osapi/linux/Semaphore.hpp"        // Include Semaphore.hpp for Linux
#elif defined(OS_FREERTOS)
#include "osapi/freertos/Semaphore.hpp"     // Include Semaphore.hpp for FreeRTOS
#else
#error "No known OS defined"                // Error message for unknown operating system
#endif

#endif                                      // End of header guard
