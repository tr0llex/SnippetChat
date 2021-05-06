//
// Created by Борис Кожуро on 13.04.2021.
//
#pragma once
#ifndef MESSENGER_AUTH_AUTH_H
#define MESSENGER_AUTH_AUTH_H

#include<string>
#include <vector>
#include "Models.hpp"
#include "InMemoryDb.hpp"
#include "../../hash/hashids.h"

class AuthInterface {
public:
    virtual ~AuthInterface() = default;
    /// Функция принимает LoginData - 2 стринга: Логин + пароль/токен тип 1 = пароль Тип -1 = токен. Реализация типа 1 будет когда будет БД
    /// \param loginData
    /// \return  Отдает Токен для правильных данных, "" для пустого
    virtual std::string loginUser(LoginData loginData) = 0;
    /// Функция принимает  id
    /// \param userId
    /// \return Отдает 0 при удачном логауте -1 при ошибке
    virtual int logoutUser(uint32_t userId) = 0;
    /// Принимает токен
    /// \param token
    /// \return  отдает userId к которому тот привязан, -1 при отсутствии
    virtual int verifyToken(std::string token) = 0;
    ///  Регистрирует пользователя в основной БД + заносит сессию
    /// \param userLogin
    /// \param userPassword
    /// \return Возвращает токен для данного пользователя
    virtual std::string registerUser(std::string userLogin, std::string userPassword) = 0;
};

class TokenManager {
private:
    InMemoryDb tarantool;
    uint32_t userId_;
    std::string token_;
    std::string user_name_;
    int status_;
public:
    TokenManager() = default;
    ~TokenManager() = default;
    int tryLogin(LoginData data);
    int tryToken(LoginData data);
    int validateToken(std::string token);
    int logoutUser(uint32_t userId);
    std::string generateToken(uint32_t userId);
    int writeInMemory(int userId,  const std::string& userName, const std::string& token, int status);
};

class Auth : public AuthInterface {
private:
    TokenManager tokenMan;
public:
    User currentUser;
    Auth() = default;
    int logoutUser (uint32_t userId) override  {
        int result = tokenMan.logoutUser(userId);
        return result;
    }
    std::string loginUser (LoginData data) override {
        if (data.getType() == -1) {
            int result = tokenMan.tryToken(data);
            if (result >= 0) {
                return data.getPassword();
            }
            return "";
        }
        int result = tokenMan.tryLogin(data);
        if (result == -1) {
            return "";
        }
        return tokenMan.generateToken(result);
    }
    std::string registerUser(std::string userLogin, std::string userPassword) override {
        // TODO - write in cassandra
        int userId = 100; //override on cassandra
        std::string token = tokenMan.generateToken(userId);
        tokenMan.writeInMemory(userId, userLogin, token, 1);
        return token;
    }
    int verifyToken(std::string token) override {
        return tokenMan.validateToken(token);
    }
};
#endif  //  MESSENGER_AUTH_AUTH_H
