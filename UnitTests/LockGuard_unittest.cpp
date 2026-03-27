#include "pch.h"
#include "../ChatAppClient/Assets/LockGuard.h"

    TEST(LockGuard, MakeSureTheLockReleases) {
        std::binary_semaphore m_semaphore(1);
        {
            SignalSync::LockGuard guard(m_semaphore);
            EXPECT_EQ(false, m_semaphore.try_acquire());
        }
        EXPECT_EQ(true, m_semaphore.try_acquire());
    }