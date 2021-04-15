#ifndef PROJECT_INCLUDE_MODELS_HPP_
#define PROJECT_INCLUDE_MODELS_HPP_

#include <string>
#include <utility>
#include <vector>
#include <ctime>

class User {
private:
    uint32_t userId_;
    std::string userLogin_;
    std::string userPassword_;
    std::vector<uint32_t> userDialogueList_;
    std::string userToken_;
    int userStatus_;

public:
    User() = default;

    ~User() = default;

    User(uint32_t userId, std::string userLogin, std::string userPassword,
         std::vector<uint32_t> dialogueList,
         std::string userToken, int userStatus) :
            userLogin_(std::move(userLogin)), userPassword_(std::move(userPassword)),
            userDialogueList_(std::move(dialogueList)), userToken_(std::move(userToken)) {
        setUserId(userId);
        setStatus(userStatus);
    }

    uint32_t getUserId() const {
        return userId_;
    }

    std::string getLogin() {
        return userLogin_;
    }

    std::string getPassword() {
        return userPassword_;
    }

    std::vector<uint32_t> getDialogues() {
        return userDialogueList_;
    }

    std::string getToken() {
        return userToken_;
    }

    int getStatus() const {
        return userStatus_;
    }

    void setUserId(uint32_t userId) {
        userId_ = userId;
    }

    void setLogin(const std::string &userLogin) {
        userLogin_ = userLogin;
    }

    void setPassword(const std::string &userPassword) {
        userPassword_ = userPassword;
    }

    void setDialogues(const std::vector<uint32_t> &dialogueList) {
        userDialogueList_ = dialogueList;
    }

    void setToken(const std::string &userToken) {
        userToken_ = userToken;
    }

    void setStatus(int userStatus) {
        userStatus_ = userStatus;
    }

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

    void setLogin(const std::string &login) {
        login_ = login;
    }

    void setPassword(const std::string &password) {
        password_ = password;
    }

    void setType(short type) {
        loginType_ = type;
    }

    std::string getLogin() const {
        return login_;
    }

    std::string getPassword() const {
        return password_;
    }

    short get_type() const {
        return loginType_;
    }
    bool operator== (const LoginData& ldt1) const {
        if (login_ == ldt1.getLogin())
            if (password_ == ldt1.getPassword())
                if (loginType_ == ldt1.get_type()) {
                    return true;
                }
        return false;
    }

private:
    std::string login_;
    std::string password_;
    short loginType_;

};

class Message {
public:
    Message(uint32_t messageId, uint32_t dialogueParentId, uint32_t senderId,
            std::string messageText, std::string messageCode, time_t timeSent) :
            messageId_(messageId), dialogueParentId_(dialogueParentId), senderId_(senderId),
            messageText_(std::move(messageText)), messageCode_(std::move(messageCode)), timeSent_(timeSent) {
    }

    ~Message() = default;

    uint32_t getMessageId() const {
        return messageId_;
    }

    uint32_t getDialogueParentId() const {
        return dialogueParentId_;
    }

    uint32_t getSenderId() const {
        return senderId_;
    }

    std::string getMessageText() {
        return messageText_;
    }

    std::string getMessageCode() {
        return messageCode_;
    }

    time_t getTimeSent() const {
        return timeSent_;
    }

private:
    uint32_t messageId_;
    uint32_t dialogueParentId_;
    uint32_t senderId_;
    std::string messageText_;
    std::string messageCode_;
    time_t timeSent_;
};

class Dialogue {
public:
    explicit Dialogue(uint32_t dialogueId) : dialogueId_(dialogueId) {
    }

    ~Dialogue() = default;

    std::vector<uint32_t> getParticipantsList() {
        return participantsList_;
    }

    std::vector<Message> getDialogueMessageList() {
        return dialogueMessageList_;
    }

    uint32_t getDialogueId() const {
        return dialogueId_;
    }

    void setDialogueId(uint32_t id) {
        dialogueId_ = id;
    }

    void pushNewMessage(const Message& newMessage) {
        dialogueMessageList_.push_back(newMessage);
    }

    void pushNewParticipant(uint32_t newParticipantId) {
        participantsList_.push_back(newParticipantId);
    }

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
