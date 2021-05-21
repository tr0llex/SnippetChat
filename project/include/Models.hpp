#ifndef PROJECT_INCLUDE_MODELS_HPP_
#define PROJECT_INCLUDE_MODELS_HPP_

#include <string>
#include <utility>
#include <vector>
#include <ctime>

class User {
private:
    uint32_t userId_{};
    std::string userLogin_;
    std::string userPassword_;
    std::vector<uint32_t> userDialogueList_;
    std::string userToken_;
    int userStatus_{};

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

    uint32_t getUserId() const;

    std::string getLogin();

    std::string getPassword();

    std::vector<uint32_t> getDialogues();

    std::string getToken();

    int getStatus() const;

    void setUserId(uint32_t userId);

    void setLogin(const std::string &userLogin);

    void setPassword(const std::string &userPassword);

    void setDialogues(const std::vector<uint32_t> &dialogueList);

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

    bool operator==(const LoginData &ldt1) const;

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

    uint32_t getMessageId() const;

    uint32_t getDialogueParentId() const;

    uint32_t getSenderId() const;

    std::string getMessageText();

    std::string getMessageCode();

    time_t getTimeSent() const;

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

    std::vector<uint32_t> getParticipantsList();

    std::vector<Message> getDialogueMessageList();

    uint32_t getDialogueId() const;

    void setDialogueId(uint32_t id);

    void pushNewMessage(const Message &newMessage);

    void pushNewParticipant(uint32_t newParticipantId);

private:
    uint32_t dialogueId_;
    std::vector<Message> dialogueMessageList_;
    std::vector<uint32_t> participantsList_;
};


class Compilation {
public:
    Compilation() {

    };

    ~Compilation() = default;

    uint32_t getCompilationId() const;

    void setCompilationId(const uint32_t &compilationId);

    uint32_t getMessageId() const;

    void setMessageId(const uint32_t &messageId);

    std::string getMessageCode();

    void setMessageCode(const std::string &messageCode);

    std::string getCompilerStderr();

    void setCompilerStderr(const std::string &compilerStderr);

    std::string getCompilerStdout();

    void setCompilerStdout(const std::string &compilerStdout);

    std::string getExecutionStderr();

    void setExecutionStderr(const std::string &executionStderr);

    std::string getExecutionStdin();

    void setExecutionStdin(const std::string &executionStdin);

    std::string getExecutionStdout();

    void setExecutionStdout(const std::string &executionStdout);

    std::string getExecutionUsedMemory();

    void setExecutionUsedMemory(const std::string &executionUsedMemory);

    std::string getExecutionTime();

    void setExecutionTime(const std::string &executionTime);

private:
    uint32_t compilationId_{};
    uint32_t messageId_{};
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
