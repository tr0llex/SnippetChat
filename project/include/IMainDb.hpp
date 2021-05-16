#ifndef PROJECT_INCLUDE_IMAINDB_HPP_
#define PROJECT_INCLUDE_IMAINDB_HPP_

#include <string>
#include "Models.hpp"

class IMainDb {
    public:
        virtual ~IMainDb() = default;

        virtual User* searchUserLogin(std::string login, std::string password) = 0;
        virtual void writeUser(const User& user) = 0;
        virtual void changePassword(const User& user) = 0;


        virtual std::string getCodeFromMessage(std::string messageId) = 0;
        virtual void writeMessage(Message& message) = 0;

        virtual std::vector<Message> getNLastMessagesFromDialogue(std::string dialogueId, long count) = 0;
        virtual std::vector<std::string> getParticipantsLoginsFromDialogue(std::string dialogueId) = 0;
        virtual std::vector<std::string> getAllDialoguesIdByLogin(std::string login) = 0;
        virtual std::vector<std::string> getLastNDialoguesIdByLogin(std::string login, long count) = 0;
        virtual DialogueList getLastNDialoguesWithLastMessage(User user, long count) = 0;

        virtual Dialogue createDialogue(std::string firstId, std::string secondId) = 0;
        virtual void deleteMessage(Message& message) = 0;
        virtual void deleteDialogue(Dialogue& dialogue) = 0;
};

#endif  // PROJECT_INCLUDE_IMAINDB_HPP_
