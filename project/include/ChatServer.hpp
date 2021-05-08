#ifndef PROJECT_INCLUDE_CHATSERVER_HPP_
#define PROJECT_INCLUDE_CHATSERVER_HPP_


#include "VovaModels.hpp"
#include "ChatEvent.hpp"

#include <mutex>


class ChatServer {
public:
    class Client {
    };

    explicit ChatServer(Wt::WServer& server);

    ChatServer(const ChatServer &) = delete;
    ChatServer &operator=(const ChatServer &) = delete;

    bool connect(Client *client, const ChatEventCallback& handleEvent);
    bool disconnect(Client *client);
    bool login(const User& user);
    void logout(const User& user);
    bool signUp(const User& user);
    void changeProfile(User& user, const User& newUser);
    void sendMessage(const User& user, const Wt::WString& message);
    void runCode(const User& user, const Wt::WString& dataStdin);

private:
    Wt::WServer&         server_;
    std::recursive_mutex mutex_;

    void postChatEvent(const ChatEvent& event);
};


#endif  // PROJECT_INCLUDE_CHATSERVER_HPP_
