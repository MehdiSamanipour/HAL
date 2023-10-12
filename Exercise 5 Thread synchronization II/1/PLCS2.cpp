// Define mutexes and conditionals
mutex entry_mutex;
mutex exit_mutex;
condition_variable entry_cv;
condition_variable exit_cv;
bool entry_gate_open = true;
bool exit_gate_open = true;
// Entry guard thread function
void entry_guard() {
    while (true) {
        unique_lock<mutex> lock(entry_mutex);
        entry_cv.wait(lock, []{ return entry_gate_open; });
        entry_gate_open = false;
        lock.unlock();
        // Grant permission to enter
        // ...
        this_thread::sleep_for(chrono::milliseconds(100)); // Simulate time to enter
        lock.lock();
        entry_gate_open = true;
        entry_cv.notify_one();
    }
}
// Exit guard thread function
void exit_guard() {
    while (true) {
        unique_lock<mutex> lock(exit_mutex);
        exit_cv.wait(lock, []{ return exit_gate_open; });
        exit_gate_open = false;
        lock.unlock();
        // Grant permission to exit
        // ...
        this_thread::sleep_for(chrono::milliseconds(100)); // Simulate time to exit
        lock.lock();
        exit_gate_open = true;
        exit_cv.notify_one();
    }
}
// Car thread function
void car() {
    while (true) {
        unique_lock<mutex> entry_lock(entry_mutex);
        entry_cv.wait(entry_lock, []{ return entry_gate_open; });
        entry_gate_open = false;
        entry_lock.unlock();
        // Grant permission to enter
        // ...
        this_thread::sleep_for(chrono::milliseconds(100)); // Simulate time to enter
        unique_lock<mutex> exit_lock(exit_mutex);
        exit_cv.wait(exit_lock, []{ return exit_gate_open; });
        exit_gate_open = false;
        exit_lock.unlock();
        // Grant permission to exit
        // ...
        this_thread::sleep_for(chrono::milliseconds(100)); // Simulate time to exit
        entry_lock.lock();
        entry_gate_open = true;
        entry_cv.notify_one();
    }
}
