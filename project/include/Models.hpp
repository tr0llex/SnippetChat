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

    short get_type() const;
    bool operator== (const LoginData& ldt1) const;

private:
    std::string login_;
    std::string password_;
    short loginType_;
};

class Message {
public:
    Message() = default;
    Message(const std::string &dialogueParentId, const std::string &senderId,
            const std::string &messageText, const std::string &messageCode = std::string());
    Message(const std::string &messageId, const std::string &dialogueParentId, const std::string &senderId,
            std::string messageText, std::string messageCode, time_t timeSent, bool isRead);

    ~Message() = default;

    std::string getId() const;

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
    std::string senderId_;
    std::string messageText_;
    std::string messageCode_;
    time_t timeSent_;
    bool isRead_;
};

class Dialogue {
public:
    Dialogue() = default;
    Dialogue(std::string dialogueId,
             std::vector<Message> dialogueMessageList,
             std::vector<std::string> participantsList) :
            id_(std::move(dialogueId)),
            dialogueMessageList_(std::move(dialogueMessageList)),
            participantsList_(std::move(participantsList)) {
    }

    ~Dialogue() = default;

    Message getLastMessage() const;

    std::vector<std::string> getParticipantsList();

    std::vector<Message> getDialogueMessageList();

    std::string getId() const;

    void pushNewMessage(const Message& newMessage);

    void pushMessages(const std::vector<Message> &dialogueMessageList);

    void pushNewParticipant(std::string newParticipantId);

    std::string getName(const User &requester) const;

    bool isEmpty() const;

    bool withYourself() const;

    bool operator==(const Dialogue &dialogue) const;

private:
    std::string id_;
    std::vector<Message> dialogueMessageList_;
    std::vector<std::string> participantsList_;
};


class Compilation {
public:
    Compilation() = default;

    ~Compilation() = default;

    uint32_t getCompilationId() const {
        return compilationId_;
    }

    void setCompilationId(const uint32_t &compilationId) {
        compilationId_ = compilationId;
    }

    uint32_t getMessageId() const {
        return messageId_;
    }

    void setMessageId(const uint32_t &messageId) {
        messageId_ = messageId;
    }

    std::string getMessageCode() {
        return messageCode_;
    }

    void setMessageCode(const std::string &messageCode) {
        messageCode_ = messageCode;
    }

    std::string getCompilerStderr() {
        return compilerStderr_;
    }

    void setCompilerStderr(const std::string &compilerStderr) {
        compilerStderr_ = compilerStderr;
    }

    std::string getCompilerStdout() {
        return compilerStdout_;
    }

    void setCompilerStdout(const std::string &compilerStdout) {
        compilerStdout_ = compilerStdout;
    }

    std::string getExecutionStderr() {
        return executionStderr_;
    }

    void setExecutionStderr(const std::string &executionStderr) {
        executionStderr_ = executionStderr;
    }

    std::string getExecutionStdin() {
        return executionStdin_;
    }

    void setExecutionStdin(const std::string &executionStdin) {
        executionStdin_ = executionStdin;
    }

    std::string getExecutionStdout() {
        return executionStdout_;
    }

    void setExecutionStdout(const std::string &executionStdout) {
        executionStdout_ = executionStdout;
    }

    std::string getExecutionUsedMemory() {
        return executionUsedMemory_;
    }

    void setExecutionUsedMemory(const std::string &executionUsedMemory) {
        executionUsedMemory_ = executionUsedMemory;
    }

    std::string getExecutionTime() {
        return executionTime_;
    }

    void setExecutionTime(const std::string &executionTime) {
        executionTime_ = executionTime;
    }

private:
    uint32_t compilationId_;
    uint32_t messageId_;
    std::string messageCode_;
    std::string compilerStderr_;
    std::string compilerStdout_;
    std::string executionStderr_;
    std::string executionStdin_;
    std::string executionStdout_;
    std::string executionUsedMemory_;
    std::string executionTime_;
};

struct ComparatorDialogue {
    bool operator()(const Dialogue &lhs, const Dialogue &rhs) {
        return lhs.getLastMessage().getTimeSent() > rhs.getLastMessage().getTimeSent();
    }
};

typedef std::multiset<Dialogue, ComparatorDialogue> DialogueList;

#endif  // PROJECT_INCLUDE_MODELS_HPP_
