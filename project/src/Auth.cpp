//
// Created by Борис Кожуро on 13.04.2021.
//
#include "Auth.hpp"

int TokenManager::tryLogin(LoginData data) {
    return 0;
}

int TokenManager::tryToken(LoginData data) {
    return 0;
}

int TokenManager::logoutUser(uint32_t user_id) {
    userId_ = user_id;
    status_ = -1;
    return 0;
}

std::string TokenManager::generateToken() {
    return "0";
}

int TokenManager::validateToken(std::string token) {
    return 0;
}


