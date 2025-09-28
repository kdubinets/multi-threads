#pragma once

#include <atomic>

namespace multi_threads {

/**
 * @brief TAS Lock (Test-And-Set Lock) implementation
 * 
 */
class TTASLock {
public:
    TTASLock() = default;
    ~TTASLock() = default;

    void lock()
    {
        while (true) {
            // First check without setting the flag (optimistic check)
            while (flag_.test()) {
                // Spin-wait (busy-wait)
            }
            // Now try to set the flag
            if (!flag_.test_and_set()) {
                return; // Successfully acquired the lock
            }
        }
    }

    bool try_lock()
    {
        return !flag_.test_and_set();
    }

    void unlock()
    {
        flag_.clear();
    }

    bool is_locked() const
    {
        return flag_.test();
    }

private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};

} // namespace multi_threads
