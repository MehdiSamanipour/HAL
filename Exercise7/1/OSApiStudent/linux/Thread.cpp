#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <osapi/Thread.hpp>
namespace osapi
{
   void* Thread::threadFunction(void* param)
  {
    Thread* self = static_cast<Thread*>(param);
    if (self != nullptr)
    {
      // Call the ThreadFunctor's run function
      self->tf_->run();
    }
    return nullptr;
  }
  Thread::Thread(ThreadFunctor* tf,
                 Thread::ThreadPriority priority,
                 const std::string& name,
                 bool autoStart)
    : tf_(tf), priority_(priority), name_(name), attached_(true)
  {
    if (autoStart)
      start();
  }
  Thread::~Thread()
  {
    detach();
  }
  void Thread::start()
  {
    if (geteuid() == 0) // Check to see if we are root
    {
      pthread_attr_t attr;
      if (pthread_attr_init(&attr) != 0)
        throw ThreadError(); // Init attr
      sched_param sched_p;
      if (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0)
        throw ThreadError(); // Set RR scheduling (RT, timesliced)
      if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0)
        throw ThreadError(); // Create thread with explicit (non-inherited) scheduling - setting priority will not work otherwise!
      sched_p.sched_priority = static_cast<int>(priority_); // Set priority
      if (pthread_attr_setschedparam(&attr, &sched_p) != 0)
        throw ThreadError(); // Use the priority
      // Missing thread creation - Create the thread with specified attributes
      if (pthread_create(&threadId_, &attr, &osapi::Thread::threadFunction, this) != 0)
        throw ThreadError();
      pthread_attr_destroy(&attr);
    }
    else
  { 
  // Create the thread without any special attributes and threadFunction as the thread function
     if (pthread_create(&threadId_, NULL, &osapi::Thread::threadFunction, this) != 0)
        throw ThreadError();
  }
     attached_ = true;
  }
  void Thread::setPriority(Thread::ThreadPriority priority)
  {
    if (!attached_)
      throw ThreadError();
    if (geteuid() == 0) // Check to see if we are root
    {
      if (pthread_setschedprio(threadId_, static_cast<int>(priority)) != 0)
        throw ThreadError();
      priority_ = priority;
    }
    else
    {
      // Do nothing
    }
  }
  Thread::ThreadPriority Thread::getPriority() const
  {
    return priority_;
  }
   std::string Thread::getName() const
  {
    return name_;
  }
  void Thread::join()
  {
    if (!attached_)
      throw ThreadError();
    tf_->threadDone_.wait();
  }
  void Thread::detach()
  {
    if (!attached_)
      throw ThreadError();
    attached_ = false;
    tf_ = nullptr;
    pthread_detach(threadId_);
  }

} 