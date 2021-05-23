#ifndef PROJECT_INCLUDE_MODELS_HPP_
#define PROJECT_INCLUDE_MODELS_HPP_

#include <string>
#include <set>
#include <utility>
#include <vector>


std::string ws2s(const std::wstring &wstr);

class User {
private:
    std::string userLogin_;
    std::string userPassword_;
    std::string userToken_;
    int userStatus_;

public:
    User() = default;
    ~User() = default;

    User(const std::string &userLogin) : userLogin_(userLogin), userStatus_(0) {}
    User(const std::string &userLogin, const std::string &userPassword) : userLogin_(userLogin), userPassword_(userPassword), userStatus_(0) {}
    User(std::string userLogin, std::string userPassword,
         std::string userToken, int userStatus) :
            userLogin_(std::move(userLogin)), userPassword_(std::move(userPassword)),
            userToken_(std::move(userToken)), userStatus_(userStatus) {
    }

    std::string getLogin() const;

    std::string getPassword() const;

    std::string getToken() const;

    int getStatus() const;

    void setUserLogin(const std::string &userLogin);

    void setPassword(const std::string &userPassword);

    void setToken(const std::string &userToken);

    void setStatus(int userStatus);

    bool operator==(const User &user) const;
    bool operator!=(const User &user) const;
};

class LoginData {
public:
    LoginData() {
        login_ = std::string();
        password_ = std::string();
        loginType_ = -1;
    }

    LoginData(const std::string &login, const std::string &password) {
        login_ = login;
        password_ = password;
        loginType_ = 1;
    }

    void setLogin(const std::string &login);

    void setPassword(const std::string &password);

    void setType(short type);

    std::string getLogin() const;

    std::string getPassword() const;

    short getType() const;

    bool operator==(const LoginData& ldt1) const;

private:
    std::string login_;
    std::string password_;
    short loginType_;
};

class Message {
public:
    Message() = default;
    Message(const std::string &dialogueParentId, const std::string &senderId,
            const std::string &messageText, time_t timeSent, const std::string &messageCode = std::string());
    Message(const std::string &messageId, const std::string &dialogueParentId, const std::string &senderId,
            std::string messageText, std::string messageCode, time_t timeSent, bool isRead);

    ~Message() = default;

    std::string getId() const;

    void setId(std::string id);

    std::string getDialogueParentId() const;

    std::string getSenderLogin() const;

    std::string getMessageText() const;

    std::string getMessageCode() const;

    time_t getTimeSent() const;

    std::string getTimeSentStr() const;

    bool isRead();

    bool isHaveCode() const;

private:
    std::string id_;
    std::string dialogueParentId_;
    std::string senderLogin_;
    std::string messageText_;
    std::string messageCode_;
    time_t timeSent_;
    bool isRead_;
};

class Dialogue {
public:
    Dialogue() = default;
    Dialogue(std::vector<std::string> participantsList,
             time_t dateOfCreation) :
            participantsList_(std::move(participantsList)),
            dateOfCreation_(dateOfCreation) {
    }
    Dialogue(std::string dialogueId,
             std::vector<Message> dialogueMessageList,
             std::vector<std::string> participantsList,
             time_t dateOfCreation) :
            id_(std::move(dialogueId)),
            dialogueMessageList_(std::move(dialogueMessageList)),
            participantsList_(std::move(participantsList)),
            dateOfCreation_(dateOfCreation) {
    }

    ~Dialogue() = default;

    Message getLastMessage() const;

    std::string getLastMessageView() const;

    std::vector<std::string> getParticipantsList();

    std::vector<Message> getDialogueMessageList();

    std::string getId() const;

    void setId(std::string id);

    void pushNewMessage(const Message& newMessage);

    void updateLastMessage(const Message& newMessage);

    void pushMessages(const std::vector<Message> &dialogueMessageList);

    void pushNewParticipant(std::string newParticipantId);

    std::string getName(const User &requester) const;

    time_t getTimeOfLastUpdate() const;

    std::string getTimeOfLastUpdateStr() const;

    bool isEmpty() const;

    bool withYourself() const;

    bool operator==(const Dialogue &dialogue) const;
    bool operator!=(const Dialogue &dialogue) const;

private:
    std::string id_;
    std::vector<Message> dialogueMessageList_;
    std::vector<std::string> participantsList_;
    time_t dateOfCreation_;
};

struct ComparatorDialogue {
    bool operator()(const Dialogue &lhs, const Dialogue &rhs) const {
        return lhs.getTimeOfLastUpdate() > rhs.getTimeOfLastUpdate();
    }
};

typedef std::multiset<Dialogue, ComparatorDialogue> DialogueList;

#endif  // PROJECT_INCLUDE_MODELS_HPP_
