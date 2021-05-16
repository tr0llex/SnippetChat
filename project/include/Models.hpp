/*
#ifndef PROJECT_INCLUDE_MODELS_HPP_
#define PROJECT_INCLUDE_MODELS_HPP_

#include <string>
#include <utility>
#include <vector>
#include <ctime>

class User {
private:
    uint32_t userId_;
    std::wstring userLogin_;
    std::wstring userPassword_;
    std::vector<uint32_t> userDialogueList_;
    std::wstring userToken_;
    int userStatus_;

public:
    User() = default;

    ~User() = default;

    User(uint32_t userId, std::wstring userLogin, std::wstring userPassword,
         std::vector<uint32_t> dialogueList,
         std::wstring userToken, int userStatus) :
            userLogin_(std::move(userLogin)), userPassword_(std::move(userPassword)),
            userDialogueList_(std::move(dialogueList)), userToken_(std::move(userToken)) {
        setUserId(userId);
        setStatus(userStatus);
    }

    uint32_t getUserId() const {
        return userId_;
    }

    std::wstring getLogin() {
        return userLogin_;
    }

    std::wstring getPassword() {
        return userPassword_;
    }

    std::vector<uint32_t> getDialogues() {
        return userDialogueList_;
    }

    std::wstring getToken() {
        return userToken_;
    }

    int getStatus() const {
        return userStatus_;
    }

    void setUserId(uint32_t userId) {
        userId_ = userId;
    }

    void setLogin(const std::wstring &userLogin) {
        userLogin_ = userLogin;
    }

    void setPassword(const std::wstring &userPassword) {
        userPassword_ = userPassword;
    }

    void setDialogues(const std::vector<uint32_t> &dialogueList) {
        userDialogueList_ = dialogueList;
    }

    void setToken(const std::wstring &userToken) {
        userToken_ = userToken;
    }

    void setStatus(int userStatus) {
        userStatus_ = userStatus;
    }
};

class LoginData {
public:
    LoginData() {
        login_ = std::wstring();
        password_ = std::wstring();
        loginType_ = -1;
    }

    LoginData(const std::wstring &login, const std::wstring &password) {
        login_ = login;
        password_ = password;
        loginType_ = 1;
    }

    void setLogin(const std::wstring &login) {
        login_ = login;
    }

    void setPassword(const std::wstring &password) {
        password_ = password;
    }

    void setType(short type) {
        loginType_ = type;
    }

    std::wstring getLogin() {
        return login_;
    }

    std::wstring getPassword() {
        return password_;
    }

    short get_type() const {
        return loginType_;
    }

private:
    std::wstring login_;
    std::wstring password_;
    short loginType_;
};

class Message {
public:
    Message(uint32_t messageId, uint32_t dialogueParentId, uint32_t senderId,
            std::wstring messageText, std::wstring messageCode, time_t timeSent) :
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

    std::wstring getMessageText() {
        return messageText_;
    }

    std::wstring getMessageCode() {
        return messageCode_;
    }

    time_t getTimeSent() const {
        return timeSent_;
    }

private:
    uint32_t messageId_;
    uint32_t dialogueParentId_;
    uint32_t senderId_;
    std::wstring messageText_;
    std::wstring messageCode_;
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

    std::wstring getMessageCode() {
        return messageCode_;
    }

    void setMessageCode(const std::wstring &messageCode) {
        messageCode_ = messageCode;
    }

    std::wstring getCompilerStderr() {
        return compilerStderr_;
    }

    void setCompilerStderr(const std::wstring &compilerStderr) {
        compilerStderr_ = compilerStderr;
    }

    std::wstring getCompilerStdout() {
        return compilerStdout_;
    }

    void setCompilerStdout(const std::wstring &compilerStdout) {
        compilerStdout_ = compilerStdout;
    }

    std::wstring getExecutionStderr() {
        return executionStderr_;
    }

    void setExecutionStderr(const std::wstring &executionStderr) {
        executionStderr_ = executionStderr;
    }

    std::wstring getExecutionStdin() {
        return executionStdin_;
    }

    void setExecutionStdin(const std::wstring &executionStdin) {
        executionStdin_ = executionStdin;
    }

    std::wstring getExecutionStdout() {
        return executionStdout_;
    }

    void setExecutionStdout(const std::wstring &executionStdout) {
        executionStdout_ = executionStdout;
    }

    std::wstring getExecutionUsedMemory() {
        return executionUsedMemory_;
    }

    void setExecutionUsedMemory(const std::wstring &executionUsedMemory) {
        executionUsedMemory_ = executionUsedMemory;
    }

    std::wstring getExecutionTime() {
        return executionTime_;
    }

    void setExecutionTime(const std::wstring &executionTime) {
        executionTime_ = executionTime;
    }

private:
    uint32_t compilationId_;
    uint32_t messageId_;
    std::wstring messageCode_;
    std::wstring compilerStderr_;
    std::wstring compilerStdout_;
    std::wstring executionStderr_;
    std::wstring executionStdin_;
    std::wstring executionStdout_;
    std::wstring executionUsedMemory_;
    std::wstring executionTime_;
};

#endif  // PROJECT_INCLUDE_MODELS_HPP_
*/
