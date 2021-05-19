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

    Type type() const { return type_; }
    std::string userId() const { return userId_; }
    const Dialogue& dialogue() const { return dialogue_; }
    const std::string& messageId() const { return messageId_; }
    const std::string& resultCompilation() const { return resultCompilation_; }

    std::string getSenderLogin() const {
        return dialogue_.getLastMessage().getSenderLogin();
    }

private:
    ChatEvent(Type type, std::string userId, const Dialogue& dialogue)
            : type_(type), userId_(userId), dialogue_(dialogue) {}

    ChatEvent(Type type, std::string userId, const Dialogue& dialogue, const std::string &messageId, const std::string &resultCompilation)
            : type_(type), userId_(userId), dialogue_(dialogue), messageId_(messageId), resultCompilation_(resultCompilation) {}

    friend class ChatServer;

private:
    Type         type_;
    std::string  userId_;
    Dialogue     dialogue_;
    std::string  messageId_;
    std::string  resultCompilation_;
};

typedef std::function<void (const ChatEvent&)> ChatEventCallback;


#endif  // PROJECT_INCLUDE_CHATEVENT_HPP_
