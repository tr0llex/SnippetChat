#ifndef PROJECT_INCLUDE_MAINDB_HPP_
#define PROJECT_INCLUDE_MAINDB_HPP_

#include <vector>
#include <cassandra.h>
#include "IMainDb.hpp"
#include "Models.hpp"

class MainDb : public IMainDb {
    public:
        MainDb();
        ~MainDb();

        User* searchUserLogin(std::string login, std::string password) override;
        void writeUser (User& user) override;
        int updateUser (User& user) override;

        std::string getCodeFromMessage(uint32_t messageId) override;
        void writeMessageToDialogue(Message message) override;
        std::vector <Message>* getNMessagesFromDialogue(uint32_t dialogueId, uint32_t senderId, 
                                                        uint32_t receiverId, long count) override;

        std::vector <Dialogue>* getDialoguessByUserId(int userId) override;
        uint32_t createDialogue(uint32_t senderId, uint32_t receiverId) override;  // return dialogId
        void deleteMessageFromDialogue(Message& message) override;
        void deleteDialogue(Dialogue& dialogue) override;

        void connectToDb(const char* contactPoints);
        void disconnectFromDb();

        void migrate();
    private:
        CassFuture* connect_future_;
        CassFuture* close_future_;
        CassCluster* cluster_;
        CassSession* session_;
};

#endif  // PROJECT_INCLUDE_MAINDB_HPP_
