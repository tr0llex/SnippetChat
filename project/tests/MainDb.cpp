#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../include/MainDb.hpp"
#include "../include/IMainDb.hpp"
#include <string>
#include <cassandra.h>
#include <locale>
#include <codecvt>
using ::testing::Return;
class MockDb : public IMainDb {
public:
    MOCK_METHOD(User*, searchUserLogin, (std::string login, std::string password), (override));
    MOCK_METHOD(uint32_t, writeUser, (User& user), (override));
    MOCK_METHOD(int, updateUser, (User& user), (override));
    MOCK_METHOD(std::string, getCodeFromMessage, (uint32_t messageId), (override));
    MOCK_METHOD(void, writeMessageToDialogue, (Message message), (override));
    MOCK_METHOD(std::vector <Message>*, getNMessagesFromDialogue, (uint32_t dialogueId, uint32_t senderId, 
                                                                   uint32_t receiverId, long count), (override));
    MOCK_METHOD(std::vector <Dialogue>*, getDialoguessByUserId, (int userId), (override));
    MOCK_METHOD(uint32_t, createDialogue, (uint32_t senderId, uint32_t receiverId), (override));
    MOCK_METHOD(void, deleteMessageFromDialogue, (Message& message), (override));
    MOCK_METHOD(void, deleteDialogue, (Dialogue& dialogue), (override));
};

TEST(searchUserLogin, InCorrectData) {
    MockDb mock;
    EXPECT_CALL(mock, searchUserLogin(std::string("log"), std::string("pas"))).WillOnce(Return(nullptr));
    EXPECT_EQ(mock.searchUserLogin(std::string("log"), std::string("pas")), nullptr);
}

TEST(getCodeFromMessage, CorrectData) {
    MockDb mock;
    EXPECT_CALL(mock, getCodeFromMessage(23)).WillOnce(Return(""));
    EXPECT_EQ(mock.getCodeFromMessage(23), "");
}

// TEST(writeUser, CorrectData) {
//     MockDb mock;
//     User user;
//     EXPECT_CALL(mock, writeUser(user)).WillOnce(Return(921392));
//     EXPECT_EQ(mock.writeUser(user), 921392);
// }

// TEST(updateUser, CorrectData) {
//     MockDb mock;
//     User user;
//     EXPECT_CALL(mock, updateUser(user)).WillOnce(Return(0));
//     EXPECT_EQ(mock.updateUser(user), 0);
// }
