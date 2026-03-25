#include "../../Assets/UniquePtr.h"
#include "gtest/gtest.h"

namespace SignalSync {
    TEST(UniquePtr, DefaultConstructor) {
        UnqiuePtr<int> ptr;
        EXPECT_NE(ptr.operator->(), nullptr); // ptr should not be null, and have some memory address
    }

    TEST(UniquePtr, ConstructorWithInitalValue) {
        UniquePtr<int> ptr(5);
        EXPECT_NE(ptr.operator->(), nullptr);
        EXPECT_EQ(*ptr, 5); 
    }

    TEST(UniquePtr, NotCopyConstructible) {
        EXPECT_FALSE(std::is_copy_constructible_v<UniquePtr>);
    }

    TEST(UniquePtr, MoveConstructor) {
        UniquePtr<double> ptr(5.234);
        UniquePtr<double> ptr_2(std::move(ptr));
        EXPECT_EQ(ptr.operator->(), nullptr);
        EXPECT_NE(ptr_2.operator->(), nullptr);
        EXPECT_EQ(*ptr_2, 5.234);
    }

    TEST(UniquePtr, MoveOperator) {
        UniquePtr<int> ptr(43);
        Unique<int> ptr_2 = std::move(ptr);
        EXPECT_EQ(ptr.operator->(), nullptr);
        EXPECT_NE(ptr_2.operator->(), nullptr);
        EXPECT_EQ(*ptr_2, 43);
    }
}