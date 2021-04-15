//
// Created by Борис Кожуро on 15.04.2021.
//

#ifndef PROJECT_INCLUDE_INMEMORYDB_HPP_
#define PROJECT_INCLUDE_INMEMORYDB_HPP_
#include<string>
#include <utility>
#include <vector>

class InMemoryDbInterface {
public:
    virtual std::string getUserName(std::string token) = 0;
    virtual int writeInMemory(int userId, std::string token,
                               std::string userName, int status) = 0;
    virtual int updateUserStatus(int userId, int way) = 0;
    virtual int searchToken(std::string token) = 0;
    virtual std::vector<int> getOnline(std::vector<uint32_t> userVector) = 0;
};
class InMemoryDb  : public InMemoryDbInterface {
private:
    uint32_t userId_{};
    std::string token_;
    std::string userName_;
    int status_{};


public:
    InMemoryDb() = default;
    InMemoryDb(uint32_t userId, std::string  token,
               std::string userName, int status) :
            userId_(userId), token_(std::move(token)),
            userName_(std::move(userName)), status_(status) {
    }
    ~InMemoryDb() = default;
    int dbStart();
    int dbConnect();
    std::string getUserName(std::string token) override;
    int writeInMemory(int userId, std::string token,
                      std::string userName, int status) override;
    int updateUserStatus(int userId, int way) override;
    int searchToken(std::string token) override;
    std::vector<int> getOnline(std::vector<uint32_t> userVector) override;
};

#endif //  PROJECT_INCLUDE_INMEMORYDB_HPP_
