//
// Created by Борис Кожуро on 13.04.2021.
//
#include "Auth.hpp"


int TokenManager::tryLogin(LoginData data) {
    // TODO - connect to Dudnik
    return 0;
}

int TokenManager::tryToken(LoginData data) {
    int got_token = tarantool.searchToken(data.getPassword());
    return got_token;
}

int TokenManager::logoutUser(uint32_t userId) {
    int result = tarantool.updateUserStatus(userId, -1);
    return result;
}

std::string TokenManager::generateToken(uint32_t userId) {
    hashidsxx::Hashids hash("this is my salt");
    std::string new_hash = hash.encode({userId});
    return new_hash;
}
 // returns Uid
int TokenManager::validateToken(std::string token) {
     int got_token = tarantool.searchToken(token);
     return got_token;
}
int TokenManager::writeInMemory(int userId,  const std::string& userName, const std::string& token, int status) {
    return tarantool.writeInMemory(userId, userName, token,status);
}


