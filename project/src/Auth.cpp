//
// Created by Борис Кожуро on 13.04.2021.
//
#include "Auth.hpp"


std::string TokenManager::tryLogin(LoginData data) {
    // TODO - connect to Dudnik
    return "";
}

std::string TokenManager::tryToken(LoginData data) {
    std::string got_token = tarantool.searchToken(data.getPassword());
    return got_token;
}

int TokenManager::logoutUser(std::string userName) {
    int result = tarantool.updateUserStatus(userName, -1);
    return result;
}

std::string TokenManager::generateToken(std::string userName) {
    std::string new_hash = md5(userName);
    return new_hash;
}
 // returns Uid
std::string TokenManager::validateToken(std::string token) {
     std::string got_token = tarantool.searchToken(token);
     return got_token;
}
int TokenManager::writeInMemory(const std::string& userName, const std::string& token, int status) {
    return tarantool.writeInMemory(userName, token,status);
}


