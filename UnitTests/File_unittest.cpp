#include "pch.h"
#include "../ChatAppClient/Assets/File.h"
#include <QString>

    TEST(File, ConstructorWithArguments) {
        QString user_from = "User";
        char* t_data = new char[10];
        uint32_t size = 10;
        SignalSync::File f(user_from, t_data, size);
        EXPECT_EQ(f.getUserFrom(), user_from);
        EXPECT_EQ(f.getData(), t_data);
        EXPECT_EQ(f.getSize(), size);
    }

    TEST(File, MoveConstructor) {
        QString user_from = "User";
        char* t_data = new char[10];
        uint32_t size = 10;
        SignalSync::File f(user_from, t_data, size);
        SignalSync::File f_2(std::move(f));
        EXPECT_EQ(f_2.getUserFrom(), user_from);
        EXPECT_EQ(f_2.getData(), t_data);
        EXPECT_EQ(f_2.getSize(), size);
        EXPECT_EQ(f.getUserFrom(), "");
        EXPECT_EQ(f.getData(), nullptr);
        EXPECT_EQ(f.getSize(), 0);
    }