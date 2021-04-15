#ifndef PROJECT_INCLUDE_IMAINDB_HPP_
#define PROJECT_INCLUDE_IMAINDB_HPP_

#include <string>
#include "Models.hpp"

class IMainDb {
    public:
        virtual ~IMainDb() = default;

        virtual User* searchUserLogin(std::string login, std::string password) = 0;
        virtual uint32_t writeUser (User& user) = 0;
        virtual int updateUser (User& user) = 0;

        virtual std::string getCodeFromMessage(uint32_t messageId) = 0;
        virtual void writeMessageToDialogue(Message message) = 0;
        virtual std::vector <Message>* getNMessagesFromDialogue(uint32_t dialogueId, uint32_t senderId, 
                                                                uint32_t receiverId, long count) = 0;

        virtual std::vector <Dialogue>* getDialoguessByUserId(int userId) = 0;
        virtual uint32_t createDialogue(uint32_t senderId, uint32_t receiverId) = 0;  // return dialogId
        virtual void deleteMessageFromDialogue(Message& message) = 0;
        virtual void deleteDialogue(Dialogue& dialogue) = 0;
};

#endif  // PROJECT_INCLUDE_IMAINDB_HPP_
