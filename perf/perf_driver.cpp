#include <assert.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#include "tas_lock.h"
#include "ttas_lock.h"

class LockBenchmark {
public:
    LockBenchmark(size_t thread_count, size_t iteration_count, bool verify = true)
        : m_thread_count(thread_count), m_iteration_count(iteration_count), m_verify(verify) {}

    virtual ~LockBenchmark() = default;

    std::chrono::microseconds run() 
    {
        std::vector<std::thread> threads;
        threads.reserve(m_thread_count);

        for (size_t i = 0; i < m_thread_count; ++i) {
            threads.emplace_back(&LockBenchmark::worker_thread, this, i);
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        m_start_flag.clear(); // Signal threads to start

        for (auto& thread : threads) {
            thread.join();
        }

        auto end_time = std::chrono::high_resolution_clock::now();

        if (m_verify &&!verify_correctness()) {
            std::cerr << "Correctness verification failed!" << std::endl;
            abort();
        }

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        return duration;
    }

    virtual void reset()
    {
        m_start_flag.test_and_set();
    }

protected:
    virtual void worker_thread(size_t thread_id) = 0;
    virtual bool verify_correctness() const = 0;

protected:
    static const size_t EXECUTION_STEP = 50;
    const size_t m_thread_count;
    const size_t m_iteration_count;
    const bool m_verify;
    std::atomic_flag m_start_flag{true};
};


template <typename LockType>
class GenericLockBenchmark : public LockBenchmark {
public:
    GenericLockBenchmark(size_t thread_count, size_t iteration_count, bool verify = true)
        : LockBenchmark(thread_count, iteration_count, verify), counter(0), risky_counter(0) {}

    virtual void reset() override
    {
        LockBenchmark::reset();
        counter.store(0);
        risky_counter.store(0);
    }
       
protected:
    virtual void worker_thread(size_t _) override 
    {
        while (m_start_flag.test()); // Wait for start signal

        while (true) {
            if (counter.load() >= m_iteration_count) {
                return;
            }

            for (size_t i = 0; i < EXECUTION_STEP; i++) {
                lock.lock();
                if (m_verify) {
                    risky_counter.store(risky_counter.load() + 1);
                }
                lock.unlock();
            }

            counter += EXECUTION_STEP;
        }
    }

    virtual bool verify_correctness() const override {
        return !m_verify || counter == risky_counter;
    }

private:
    std::atomic<size_t> counter;
    std::atomic<size_t> risky_counter;
    LockType lock;
};

class NoLock {
public:
    void lock() {}
    void unlock() {}
};

using TASLockBenchmark = GenericLockBenchmark<multi_threads::TASLock>;
using TTASLockBenchmark = GenericLockBenchmark<multi_threads::TTASLock>;
using MutexBenchmark = GenericLockBenchmark<std::mutex>;
using NoLockBenchmark = GenericLockBenchmark<NoLock>;

int main(int argc, char* argv[]) 
{
    // List of allowed benchmark names
    const std::vector<std::string> allowed_benchmarks = {
        "tas_lock",
        "ttas_lock",
        "mutex",
        "no_lock"
        // Add more benchmark names here as needed
    };

    auto print_usage = [&]() {
        std::cout << "Usage: " << argv[0] << " <benchmark_name> <number_of_threads> <number_of_iterations> [number_of_test_runs]\n";
        std::cout << "Allowed benchmark_name values:\n";
        for (const auto& name : allowed_benchmarks) {
            std::cout << "  - " << name << "\n";
        }
        std::cout << "Example: " << argv[0] << " insert 4 10000\n";
    };

    if (argc < 4 || std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
        print_usage();
        return 1;
    }

    std::string benchmark_name = argv[1];
    if (std::find(allowed_benchmarks.begin(), allowed_benchmarks.end(), benchmark_name) == allowed_benchmarks.end()) {
        std::cout << "Unknown benchmark_name: " << benchmark_name << "\n";
        print_usage();
        return 1;
    }

    size_t thread_count = 1;
    size_t iteration_count = 10000;
    size_t test_count = 1;

    try {
        thread_count = std::stoul(argv[2]);
    } catch (...) {
        std::cout << "Invalid number_of_threads argument.\n";
        print_usage();
        return 1;
    }

    try {
        iteration_count = std::stoul(argv[3]);
    } catch (...) {
        std::cout << "Invalid number_of_iterations argument.\n";
        print_usage();
        return 1;
    }

    if (argc >= 5) {
        try {
            test_count = std::stoul(argv[4]);
        } catch (...) {
            std::cout << "Invalid number_of_iterations argument.\n";
            print_usage();
            return 1;
        }
    }

    std::unique_ptr<LockBenchmark> benchmark;

    if (benchmark_name == "tas_lock") {
        benchmark = std::make_unique<TASLockBenchmark>(thread_count, iteration_count);
    } else if (benchmark_name == "ttas_lock") {
        benchmark = std::make_unique<TTASLockBenchmark>(thread_count, iteration_count);
    } else if (benchmark_name == "mutex") {
        benchmark = std::make_unique<MutexBenchmark>(thread_count, iteration_count);
    } else if (benchmark_name == "no_lock") {
        benchmark = std::make_unique<NoLockBenchmark>(thread_count, iteration_count, false);
    } else
    {
        assert(false && "Benchmark not implemented yet");
        return 1;
    }

    auto duration_micros = 0;

    for (size_t i = 0; i < test_count; i++) {
        benchmark->reset();
        duration_micros += benchmark->run().count();
    }

    duration_micros /= test_count;

    std::string micros_str = std::to_string(duration_micros);
    for (int insert_pos = static_cast<int>(micros_str.length()) - 3; insert_pos > 0; insert_pos -= 3) {
        micros_str.insert(insert_pos, ",");
    }

    std::cout << std::left
              << std::setw(20) << benchmark_name << "\t"
              << std::setw(3)  << thread_count     << "\t"
              << std::setw(12) << iteration_count  << "\t"
              << micros_str << " microseconds\n";
    return 0;
}