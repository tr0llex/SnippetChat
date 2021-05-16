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
        void writeUser(User& user) override;
        int updateUser(User& user) override;

        std::string getCodeFromMessage(std::string messageId) override;
        void writeMessage(Message& message) override; // проставить айдишник если не проставлен
        std::vector <Message>* getNMessagesFromDialogue(std::string dialogueId, long count) override;
        DialogueList& getLastNDialoguesWithLastMessage(User user, long count) override;

        std::vector <std::string>* getDialogueListByLogin(std::string login) override;
        Dialogue createDialogue(std::string firstId, std::string secondId) override;
        // std::string createDialogue(std::string senderId, std::string receiverId, std::string dialogueName) override; with yourself
        void deleteMessage(Message& message) override;
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
