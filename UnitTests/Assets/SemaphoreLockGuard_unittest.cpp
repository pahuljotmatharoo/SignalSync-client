#include "../../ChatAppClient/Assets/SemaphoreLockGuard.h"
#include "gtest/gtest.h"

namespace SignalSync {
    TEST(SemaphoreLockGuard, MakeSureLockReleases) {
        std::counting_semaphore m_semaphore(INT_MAX);
        {
            LockGuard(m_semaphore);
            m_semphore.aquire(INT_MAX - 1); // grab the rest, as in theory should grab only one
            EXPECT_EQ(false, m_semaphore.try_acquire()); // should be fully locked
        }
        EXPECT_EQ(true, m_semaphore.try_acquire()); // should have unlocked once
    }
}