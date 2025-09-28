#pragma once

#include <atomic>

namespace multi_threads {

/**
 * @brief TAS Lock (Test-And-Set Lock) implementation
 * 
 */
class TASLock {
public:
    TASLock() = default;
    ~TASLock() = default;

    void lock()
    {
        while (flag_.test_and_set()) {
            // Spin-wait (busy-wait)
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
