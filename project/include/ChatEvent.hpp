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
    const Wt::WString& resultCompilation() const { return resultCompilation_; }

    std::string getSenderId() const {
        return dialogue_.getLastMessage().getSenderLogin();
    }

private:
    ChatEvent(Type type, std::string userId, const Dialogue& dialogueInfo)
            : type_(type), userId_(userId), dialogue_(dialogueInfo) {}

    ChatEvent(Type type, std::string userId, const Dialogue& dialogueInfo, const Wt::WString &resultCompilation)
            : type_(type), userId_(userId), dialogue_(dialogueInfo), resultCompilation_(resultCompilation) {}

    friend class ChatServer;

private:
    Type         type_;
    std::string  userId_;
    Dialogue     dialogue_;
    Wt::WString  resultCompilation_;
};

typedef std::function<void (const ChatEvent&)> ChatEventCallback;


#endif  // PROJECT_INCLUDE_CHATEVENT_HPP_
