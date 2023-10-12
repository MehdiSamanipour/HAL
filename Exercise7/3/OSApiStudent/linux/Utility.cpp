#include <osapi/Utility.hpp>
#include <unistd.h>

namespace osapi
{
  void sleep(unsigned long msecs)
  {
    usleep(msecs * 1000); // usleep uses microseconds, so we multiply by 1000 to convert milliseconds to microseconds
  }
}
