//
// Created by Борис Кожуро on 13.04.2021.
//
#pragma once
#ifndef MESSENGER_AUTH_AUTH_H
#define MESSENGER_AUTH_AUTH_H

#include<string>
#include <vector>
#include "Models.hpp"

class AuthInterface {
public:
    virtual ~AuthInterface() = default;
    virtual std::string loginUser(LoginData loginData) = 0;
    virtual int logoutUser(uint32_t userId) = 0;
    virtual int verifyToken(std::string token) = 0;
};

class TokenManager {
private:
    uint32_t userId_;
    std::string token_;
    std::string user_name_;
    int status_;
public:
    int tryLogin(LoginData data);
    int tryToken(LoginData data);
    int validateToken(std::string token);
    int logoutUser(uint32_t user_id);
    std::string generateToken();
};

class Auth : public AuthInterface {
private:
    TokenManager tokenMan;
public:
    User currentUser;
    Auth() {
    }
};
#endif  //  MESSENGER_AUTH_AUTH_H
