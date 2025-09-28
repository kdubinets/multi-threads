#include <gtest/gtest.h>

#include "tas_lock.h"

namespace multi_threads {
namespace test {

class TasLockTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code for each test
    }

    void TearDown() override {
        // Cleanup code for each test
    }
};

/// Basic functionality tests
TEST_F(TasLockTest, MinimalTest) {
    TASLock lock;
    EXPECT_FALSE(lock.is_locked());
    
    lock.lock();
    EXPECT_TRUE(lock.is_locked());

    EXPECT_FALSE(lock.try_lock());

    lock.unlock();
    EXPECT_FALSE(lock.is_locked());

    EXPECT_TRUE(lock.try_lock());

    lock.unlock();
    EXPECT_FALSE(lock.is_locked());
}


} // namespace test
} // namespace multi_threads