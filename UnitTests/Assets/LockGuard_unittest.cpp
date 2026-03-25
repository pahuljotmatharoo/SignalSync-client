#include "../../ChatAppClient/Assets/LockGuard.h"
#include "gtest/gtest.h"

namespace SignalSync {
    TEST(LockGuard, MakeSureTheLockReleases) {
        std::binary_semaphore m_semaphore(1);
        {
            LockGuard(m_semaphore);
            EXPECT_EQ(false, m_semaphore.try_acquire());
        }
        EXPECT_EQ(true, m_semaphore.try_acquire());
    }
}