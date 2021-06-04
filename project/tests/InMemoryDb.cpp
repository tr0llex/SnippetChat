//
// Created by Борис Кожуро on 15.04.2021.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../include/InMemoryDb.hpp"
#include <string>

/*
 class InMemoryDbInterface {
public:
    virtual std::string getUserName (std::string token) = 0;
    virtual int writeInMemory (int userId, std::string token,
                               std::string userName, int status) = 0;
    virtual int updateUserStatus (int userId, int way) = 0;
    virtual int searchToken (std::string token) =0;
    virtual std::vector<int> getOnline (std::vector<uint32_t> userVector) =0;
};
 */
using ::testing::Return;
class MockInMemoryDb : public InMemoryDbInterface{
public:
    MOCK_METHOD(std::string, getUserName, (std::string token), (override));
    MOCK_METHOD(int, writeInMemory, (int userId, std::string token,
            std::string userName, int status), (override));
    MOCK_METHOD(int, updateUserStatus, (int userId, int way), (override));
    MOCK_METHOD(int, searchToken, (std::string token), (override));
    MOCK_METHOD(std::vector<int>, getOnline, (std::vector<uint32_t> userVector),
                (override));
};

TEST(writeInMemory, WrittenCorrectly) {
    MockInMemoryDb db;
    EXPECT_CALL(db, writeInMemory(101, "token1", "userA", 1)).WillOnce(Return(1));
    ASSERT_EQ(db.writeInMemory(101, "token1", "userA", 1), 1);
}
TEST(writeInMemory, DbError) {
    MockInMemoryDb db;
    EXPECT_CALL(db, writeInMemory(101, "token1", "userA", 1)).WillOnce(Return(-1));
    ASSERT_EQ(db.writeInMemory(101, "token1", "userA", 1), -1);
}
TEST(getUserName, UserNameExists) {
    MockInMemoryDb db;
    EXPECT_CALL(db, writeInMemory(101, "token1", "userA", 1)).WillOnce(Return(1));
    db.writeInMemory(101, "token1", "userA", 1);
    EXPECT_CALL(db, getUserName("token1")).WillOnce(Return("userA"));
    EXPECT_EQ(db.getUserName("token1"), "userA");
}
TEST(getUserName, UserNameNonexistant) {
    MockInMemoryDb db;
    EXPECT_CALL(db, writeInMemory(101, "token1", "userA", 1)).WillOnce(Return(1));
    db.writeInMemory(101, "token1", "userA", 1);
    EXPECT_CALL(db, getUserName("token2")).WillOnce(Return("0"));
    EXPECT_EQ(db.getUserName("token2"), "0");
}
TEST(updateUserStatus, updatesStates) {
    MockInMemoryDb db;
    EXPECT_CALL(db, writeInMemory(101, "token1", "userA", 1)).WillOnce(Return(1));
    db.writeInMemory(101, "token1", "userA", 1);
    EXPECT_CALL(db, updateUserStatus(101, 1)).WillOnce(Return(2));
    EXPECT_EQ(db.updateUserStatus(101, 1), 2);
    EXPECT_CALL(db, updateUserStatus(101, -1)).WillOnce(Return(1));
    EXPECT_EQ(db.updateUserStatus(101, -1), 1);
}
TEST(searchToken, TokenExists) {
    MockInMemoryDb db;
    EXPECT_CALL(db, writeInMemory(101, "token1", "userA", 1)).WillOnce(Return(1));
    db.writeInMemory(101, "token1", "userA", 1);
    EXPECT_CALL(db, searchToken("token1")).WillOnce(Return(101));
    EXPECT_EQ(db.searchToken("token1"), 101);
}
TEST(searchToken, TokenNonexistant) {
    MockInMemoryDb db;
    EXPECT_CALL(db, writeInMemory(101, "token1", "userA", 1)).WillOnce(Return(1));
    db.writeInMemory(101, "token1", "userA", 1);
    EXPECT_CALL(db, searchToken("token2")).WillOnce(Return(0));
    EXPECT_EQ(db.searchToken("token2"), 0);
}
TEST(getOnline, OnlineUsers) {
    MockInMemoryDb db;
    EXPECT_CALL(db, writeInMemory(101, "token1", "userA", 1)).WillOnce(Return(1));
    db.writeInMemory(101, "token1", "userA", 1);
    std::vector<uint32_t> users = {101};
    EXPECT_CALL(db, getOnline(users)).WillOnce(Return(std::vector<int>({1})));
    ASSERT_EQ(db.getOnline(users), std::vector<int>({1}));
}

