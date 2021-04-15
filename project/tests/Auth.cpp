//
// Created by Борис Кожуро on 15.04.2021.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../include/Auth.hpp"
#include "../include/Models.hpp"
#include <string>
#include <locale>
#include <codecvt>
/*
class AuthInterface {
public:
    virtual ~AuthInterface() = 0;
    virtual std::string loginUser(LoginData loginData) = 0;
    virtual int logoutUser(uint32_t userId) = 0;
    virtual int verifyToken(std::string token) = 0;
};
  */
using ::testing::Return;
class MockAuth : public AuthInterface{
public:
    MOCK_METHOD(int, logoutUser, (uint32_t userId), (override));
    MOCK_METHOD(int, verifyToken, (std::string token), (override));
    MOCK_METHOD(std::string, loginUser, (LoginData loginData), (override));
};

TEST(VerifyToken, NonexistentToken) {
    MockAuth mock;
    EXPECT_CALL(mock, verifyToken(std::string("b"))).WillOnce(Return(-1));
    EXPECT_EQ(mock.verifyToken("b"), -1);
}
TEST(VerifyToken, CorrectToken) {
    MockAuth mock;
    EXPECT_CALL(mock, verifyToken(std::string("a"))).WillOnce(Return(101));
    EXPECT_EQ(mock.verifyToken("a"), 101);
}
TEST(LogoutUser, CorrectLogout) {
    MockAuth mock;
    EXPECT_CALL(mock, logoutUser(101)).WillOnce(Return(1));
    EXPECT_EQ(mock.logoutUser(101), 1);
}
TEST(LoginUser, CorrectLogin) {
    MockAuth mock;
    LoginData ldt, ldt2;
    ldt.setLogin("A");
    ldt.setPassword("b");
    ldt.setType(1);
    ldt == ldt2;
    EXPECT_CALL(mock, loginUser(ldt)).WillOnce(Return("correct_token"));
    EXPECT_EQ(mock.loginUser(ldt), "correct_token");
}

TEST(LoginUser, IncorrectLogin) {
    MockAuth mock;
    LoginData ldt;
    ldt.setLogin("A");
    ldt.setPassword("c");
    ldt.setType(1);
    EXPECT_CALL(mock, loginUser(ldt)).WillOnce(Return("incorrect_message"));
    EXPECT_EQ(mock.loginUser(ldt), "incorrect_message");
}
