#ifndef PROJECT_INCLUDE_CHATEVENT_HPP_
#define PROJECT_INCLUDE_CHATEVENT_HPP_


#include <functional>

#include "Models.hpp"
#include "Compilation.hpp"


class ChatEvent {
public:
    enum Type {
        kLogin,
        kLogout,
        kNewDialogue,
        kNewMessage,
        kCompilationCode,
    };

    [[nodiscard]] Type type() const { return type_; }
    [[nodiscard]] const std::string& userLogin() const { return userLogin_; }
    [[nodiscard]] const Dialogue& dialogue() const { return dialogue_; }
    [[nodiscard]] const Message& message() const { return message_; }
    [[nodiscard]] const Compilation& resultCompilation() const { return resultCompilation_; }

    [[nodiscard]] std::string getSenderLogin() const;
    [[nodiscard]] std::string getDialogueId() const;
    [[nodiscard]] const std::string& getMessageId() const;

private:
    ChatEvent(Type type, const std::string &userLogin, const Dialogue& dialogue);

    ChatEvent(Type type, const std::string &userLogin, const std::string &dialogueId,
              const std::string &messageId, const Compilation &resultCompilation);

    friend class ChatServer;

private:
    Type         type_;
    std::string  userLogin_;
    Dialogue     dialogue_;
    Message      message_;
    std::string  messageId_;
    Compilation  resultCompilation_;
};

typedef std::function<void (const ChatEvent&)> ChatEventCallback;


#endif  // PROJECT_INCLUDE_CHATEVENT_HPP_
