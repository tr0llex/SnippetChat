#ifndef PROJECT_INCLUDE_CHATEVENT_HPP_
#define PROJECT_INCLUDE_CHATEVENT_HPP_

#include "Models.hpp"

#include <Wt/WSignal.h>
#include <Wt/WString.h>


class ChatEvent {
public:
    enum Type {
        SignUp,
        Login,
        Logout,
        NewMessage,
        UpdateProfile,
        RunCode,
    };

    Type type() const { return type_; }
    const User& user() const { return user_; }
    const User& newUser() const { return user_; }
    const Wt::WString& message() const { return message_; }

private:
    Type        type_;
    User        user_;
    User        newUser_;
    Wt::WString message_;


    ChatEvent(const User& user, const Wt::WString& message)
            : type_(NewMessage), user_(user), message_(message)
    { }

    ChatEvent(Type type, User& user, const User& newUser)
            : type_(type), user_(user), newUser_(newUser)
    { }

    friend class ChatServer;
};

    typedef std::function<void (const ChatEvent&)> ChatEventCallback;


#endif  // PROJECT_INCLUDE_CHATEVENT_HPP_
