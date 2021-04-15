//
// Created by Борис Кожуро on 15.04.2021.
//

#include "InMemoryDb.hpp"
int InMemoryDb::dbStart() {
    return 0;
}

int InMemoryDb::dbConnect() {
    return 0;
}

std::string InMemoryDb::getUserName(std::string token) {
    return "";
}

int InMemoryDb::writeInMemory(int userId, std::string token, std::string userName, int status) {
    return 0;
}

int InMemoryDb::updateUserStatus(int userId, int way) {
    // way= 1 ++ way =-1 --
    userId_ = 0;
    status_  = 0;
    return 0;
}

int InMemoryDb::searchToken(std::string token) {
    return 0;
}

std::vector<int> InMemoryDb::getOnline(std::vector<uint32_t> userVector) {
    return std::vector<int>(0);
}
