#include "pch.h"
#include "../ChatAppClient/Assets/UnqiuePtr.h"
#include "../ChatAppClient/Messages/User Messages/UserMessage.h"

    TEST(UniquePtr, DefaultConstructor) {
        SignalSync::UniquePtr<int> ptr;
        EXPECT_NE(ptr.operator->(), nullptr); // ptr should not be null, and have some memory address
    }

    TEST(UniquePtr, ConstructorWithInitalValue) {
        SignalSync::UniquePtr<int> ptr(5);
        EXPECT_NE(ptr.operator->(), nullptr);
        EXPECT_EQ(*ptr, 5); 
    }

    TEST(UniquePtr, NotCopyConstructible) {
        EXPECT_FALSE(std::is_copy_constructible_v<SignalSync::UniquePtr<int>>);
    }

    TEST(UniquePtr, MoveConstructor) {
        SignalSync::UniquePtr<double> ptr(5.234);
        SignalSync::UniquePtr<double> ptr_2(std::move(ptr));
        EXPECT_EQ(ptr.operator->(), nullptr);
        EXPECT_NE(ptr_2.operator->(), nullptr);
        EXPECT_EQ(*ptr_2, 5.234);
    }

    TEST(UniquePtr, MoveOperator) {
        SignalSync::UniquePtr<int> ptr(43);
        SignalSync::UniquePtr<int> ptr_2 = std::move(ptr);
        EXPECT_EQ(ptr.operator->(), nullptr);
        EXPECT_NE(ptr_2.operator->(), nullptr);
        EXPECT_EQ(*ptr_2, 43);
    }