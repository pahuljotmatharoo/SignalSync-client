#include "pch.h"
#include "../ChatAppClient/Messages/User Messages/UserMessage.h"
#include <QString>

TEST(UserMessage, Constructor) {
    SignalSync::UserMessage m("Some_name");
    EXPECT_EQ(m.getName(), "Some_name");
    EXPECT_EQ(m.getMessages().size(), 0);
}

TEST(UserMessage, AddMessage) {
    SignalSync::UserMessage m("Some_name");
    auto message = std::make_pair(true, "message");
    m.addMessage(message);
    EXPECT_EQ(m.getMessages()[0], message);
}