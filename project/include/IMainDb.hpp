#ifndef PROJECT_INCLUDE_IMAINDB_HPP_
#define PROJECT_INCLUDE_IMAINDB_HPP_

#include <string>
#include "Models.hpp"

class IMainDb {
    public:
        virtual ~IMainDb() = default;

        virtual User getUser(std::string login) = 0;
        virtual bool searchUserLogin(std::string login) const = 0;
        virtual bool writeUser(const User& user) = 0;
        virtual void changePassword(const User& user) = 0;


        virtual std::string getCodeFromMessage(std::string messageId) = 0;
        virtual void writeMessage(Message& message) = 0;

        virtual std::vector<Message> getNLastMessagesFromDialogue(std::string dialogueId, int count) const = 0;
        virtual std::vector<std::string> getParticipantsLoginsFromDialogue(std::string dialogueId) const = 0;
        virtual std::vector<std::string> getAllDialoguesIdByLogin(std::string login) = 0;
        virtual std::vector<std::string> getLastNDialoguesIdByLogin(std::string login, int count) const = 0;
        virtual DialogueList getLastNDialoguesWithLastMessage(const User& user, int count) const = 0;
        virtual std::string findDialogue(std::vector<std::string> participantsList) const = 0;
        virtual void createDialogue(Dialogue& dialogue) = 0;
        virtual time_t getTimeLastUpdateFromDialogue(std::string dialogueId, const User& user) const = 0;
        virtual void changePaginatedMessages(CassStatement*& statement, int amount, 
                                                          paginatedMessages& messages) = 0;

        virtual void deleteMessage(Message& message) = 0;
        virtual void deleteDialogue(Dialogue& dialogue) = 0;
};

#endif  // PROJECT_INCLUDE_IMAINDB_HPP_
