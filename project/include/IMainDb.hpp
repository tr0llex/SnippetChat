#ifndef PROJECT_INCLUDE_IMAINDB_HPP_
#define PROJECT_INCLUDE_IMAINDB_HPP_

#include <string>
#include "Models.hpp"

class IMainDb {
    public:
        virtual ~IMainDb() = default;

        virtual User* searchUserLogin(std::string login, std::string password) = 0;
        virtual void writeUser (User& user) = 0;
        virtual int updateUser (User& user) = 0;

        virtual std::string getCodeFromMessage(std::string messageId) = 0;
        virtual void writeMessage(Message& message) = 0;
        virtual std::vector <Message>* getNMessagesFromDialogue(std::string dialogueId, long count) = 0;
        virtual DialogueList& getLastNDialoguesWithLastMessage(User user, long count) = 0;
    

        virtual std::vector <std::string>* getDialogueListByLogin(std::string login) = 0;
        virtual Dialogue createDialogue(std::string firstId, std::string secondId) = 0;
        virtual void deleteMessage(Message& message) = 0;
        virtual void deleteDialogue(Dialogue& dialogue) = 0;
        virtual std::vector<User> getUsersByUserName(const User &findUser) const = 0;
};

#endif  // PROJECT_INCLUDE_IMAINDB_HPP_
