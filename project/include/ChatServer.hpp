#ifndef PROJECT_INCLUDE_CHATSERVER_HPP_
#define PROJECT_INCLUDE_CHATSERVER_HPP_


#include <map>
#include <thread>
#include <mutex>

#include "ChatEvent.hpp"
#include "VovaModels.hpp"


class ChatServer {
public:
    class Client {
    };

    explicit ChatServer(Wt::WServer& server);

    ChatServer(const ChatServer &) = delete;
    ChatServer &operator=(const ChatServer &) = delete;

    bool connect(Client *client, const User &user, const ChatEventCallback& handleEvent);
    bool disconnect(Client *client);

    bool signUp(User& user);
    bool login(User& user);
    void logout(const User &user);

    bool changeProfile(User& user, const User& newUser);

    DialogueList getDialogueList(const User &user) const;
    Dialogue getDialogue(uint32_t dialogueId) const;

    void sendMessage(const User &user, Dialogue &dialogue, const Message &message);

private:
    void postChatEvent(const ChatEvent& event);
    void notifyUser(const ChatEvent& event);

private:
    struct ClientInfo {
        std::string       sessionId;
        uint32_t          userId;
        ChatEventCallback eventCallback;
    };
    typedef std::map<Client *, ClientInfo> ClientMap;

    Wt::WServer&            server_;
    std::recursive_mutex    mutex_;
    ClientMap               clients_;
    DB                      db_;
};


#endif  // PROJECT_INCLUDE_CHATSERVER_HPP_
