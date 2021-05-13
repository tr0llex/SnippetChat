#ifndef PROJECT_INCLUDE_MODELS_HPP_
#define PROJECT_INCLUDE_MODELS_HPP_

#include <string>
#include <set>
#include <utility>
#include <vector>
#include <ctime>

class User {
private:
    std::string userLogin_;
    std::string userPassword_;
    std::vector<std::string> userDialogueList_;
    std::string userToken_;
    int userStatus_;

public:
    User(std::string userLogin = std::string(), std::string userPassword = std::string(), 
        std::vector<std::string> userDialogueList = {std::string()},
        std::string userToken = std::string(), int userStatus = 0);

    ~User() = default;

    User(uint32_t userId, std::string userLogin, std::string userPassword,
         std::vector<std::string> dialogueList,
         std::string userToken, int userStatus) :
            userLogin_(std::move(userLogin)), userPassword_(std::move(userPassword)),
            userDialogueList_(std::move(dialogueList)), userToken_(std::move(userToken)) {
        setUserId(userId);
        setStatus(userStatus);
    }

    uint32_t getUserId() const;

    std::string getLogin();

    std::string getPassword();

   std::vector<std::string> getDialogues();

    std::string getToken();

    int getStatus() const;

    void setUserId(uint32_t userId);

    void setLogin(const std::string &userLogin);

    void setPassword(const std::string &userPassword);

    void setDialogues(const std::vector<std::string> &dialogueList);

    void setToken(const std::string &userToken);

    void setStatus(int userStatus);
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
    Message(std::string messageId, std::string dialogueParentId, std::string senderId,
            std::string messageText, std::string messageCode, time_t timeSent, bool isRead) :
            messageId_(messageId), dialogueParentId_(dialogueParentId), senderId_(senderId),
            messageText_(std::move(messageText)), messageCode_(std::move(messageCode)), 
            timeSent_(timeSent), isRead_(isRead) {
    }

    ~Message() = default;

    std::string getMessageId() const;

    std::string getDialogueParentId() const;

    std::string getSenderId() const;

    std::string getMessageText();

    std::string getMessageCode();

    time_t getTimeSent() const;

    bool isRead();

private:
    std::string messageId_;
    std::string dialogueParentId_;
    std::string senderId_;
    std::string messageText_;
    std::string messageCode_;
    time_t timeSent_;
    bool isRead_;
};

class Dialogue {
public:
    explicit Dialogue(uint32_t dialogueId) : dialogueId_(dialogueId) {
    }

    ~Dialogue() = default;

    std::vector<uint32_t> getParticipantsList();

    std::vector<Message> getDialogueMessageList();

    uint32_t getDialogueId() const;

    void setDialogueId(uint32_t id);

    void pushNewMessage(const Message& newMessage);

    void pushNewParticipant(uint32_t newParticipantId);

private:
    uint32_t dialogueId_;
    std::vector<Message> dialogueMessageList_;
    std::vector<uint32_t> participantsList_;
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

#endif  // PROJECT_INCLUDE_MODELS_HPP_