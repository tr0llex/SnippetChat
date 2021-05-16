#ifndef PROJECT_INCLUDE_CHATEVENT_HPP_
#define PROJECT_INCLUDE_CHATEVENT_HPP_

#include <Wt/WSignal.h>
#include <Wt/WString.h>

#include "VovaModels.hpp"


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
    uint32_t userId() const { return userId_; }
    const DialogueInfo& dialogue() const { return dialogueInfo_; }
    const Wt::WString& resultCompilation() const { return resultCompilation_; }

    uint32_t getSenderId() const {
        return dialogueInfo_.getMessage().getSenderId();
    }

private:
    ChatEvent(Type type, uint32_t userId, const DialogueInfo& dialogueInfo)
            : type_(type), userId_(userId), dialogueInfo_(dialogueInfo) {}

    ChatEvent(Type type, uint32_t userId, const DialogueInfo& dialogueInfo, const Wt::WString &resultCompilation)
            : type_(type), userId_(userId), dialogueInfo_(dialogueInfo), resultCompilation_(resultCompilation) {}

    friend class ChatServer;

private:
    Type         type_;
    uint32_t     userId_;
    DialogueInfo dialogueInfo_;
    Wt::WString  resultCompilation_;
};

typedef std::function<void (const ChatEvent&)> ChatEventCallback;


#endif  // PROJECT_INCLUDE_CHATEVENT_HPP_
