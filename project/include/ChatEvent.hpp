#ifndef PROJECT_INCLUDE_CHATEVENT_HPP_
#define PROJECT_INCLUDE_CHATEVENT_HPP_

#include <Wt/WSignal.h>
#include <Wt/WString.h>

#include "VovaModels.hpp"


class ChatEvent {
public:
    enum Type {
        SignUp, /// TODO потом удалить
        Login,
        Logout,
        UpdateProfile,
        NewDialogue,
        NewMessage,
        RunCode, /// пока не понятно
    };

    Type type() const { return type_; }
    const User& user() const { return user_; }
    const DialogueInfo& dialogue() const { return dialogueInfo_; }

    const Wt::WString formattedHTML(const User &user,
                                    Wt::TextFormat format) const;

private:
    ChatEvent(Type type, const User& user, const DialogueInfo& dialogueInfo)
            : type_(type), user_(user), dialogueInfo_(dialogueInfo) {}

    friend class ChatServer;

private:
    Type         type_;
    User         user_;
    DialogueInfo dialogueInfo_;
};

typedef std::function<void (const ChatEvent&)> ChatEventCallback;


#endif  // PROJECT_INCLUDE_CHATEVENT_HPP_
