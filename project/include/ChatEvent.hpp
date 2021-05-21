#ifndef PROJECT_INCLUDE_CHATEVENT_HPP_
#define PROJECT_INCLUDE_CHATEVENT_HPP_

#include <Wt/WSignal.h>
#include <Wt/WString.h>

#include "Models.hpp"


class ChatEvent {
public:
    enum Type {
        Login,
        Logout,
        UpdateProfile,
        NewDialogue,
        NewMessage,
        CompilationCode, /// пока не понятно
    };

    [[nodiscard]] Type type() const { return type_; }
    [[nodiscard]] std::string userId() const { return userId_; }
    [[nodiscard]] const Dialogue& dialogue() const { return dialogue_; }
    [[nodiscard]] const Message& message() const { return message_; }
    [[nodiscard]] const std::string& resultCompilation() const { return resultCompilation_; }

    [[nodiscard]] std::string getSenderLogin() const {
        return dialogue_.getLastMessage().getSenderLogin();
    }
    [[nodiscard]] std::string getDialogueId() const {
        if (dialogue().isEmpty()) {
            return message_.getDialogueParentId();
        }
        return dialogue_.getId();
    }
    [[nodiscard]] std::string getMessageId() const {
        return message_.getId();
    }

private:
    ChatEvent(Type type, std::string userId, const Dialogue& dialogue)
            : type_(type), userId_(userId), dialogue_(dialogue) {}

    ChatEvent(Type type, std::string userId, const Message &message,  const std::string &resultCompilation)
            : type_(type), userId_(userId), message_(message), resultCompilation_(resultCompilation) {}

    ChatEvent(Type type, std::string userId, const Dialogue& dialogue, const std::string &messageId, const std::string &resultCompilation)
            : type_(type), userId_(userId), dialogue_(dialogue), messageId_(messageId), resultCompilation_(resultCompilation) {}

    friend class ChatServer;

private:
    Type         type_;
    std::string  userId_;
    Dialogue     dialogue_;
    Message      message_;
    std::string  messageId_;
    std::string  resultCompilation_;
};

typedef std::function<void (const ChatEvent&)> ChatEventCallback;


#endif  // PROJECT_INCLUDE_CHATEVENT_HPP_
