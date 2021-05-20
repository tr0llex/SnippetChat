#ifndef PROJECT_INCLUDE_CHATSERVER_HPP_
#define PROJECT_INCLUDE_CHATSERVER_HPP_


#include <map>
#include <thread>
#include <mutex>

#include "Auth.hpp"
#include "ChatEvent.hpp"
#include "Models.hpp"
#include "MainDb.hpp"


class ChatServer {
public:
    class Client {
    };

    explicit ChatServer(Wt::WServer& server);

    ChatServer(const ChatServer &) = delete;
    ChatServer &operator=(const ChatServer &) = delete;

    bool connect(Client *client, const User &user, const ChatEventCallback& handleEvent);
    bool disconnect(Client *client);

    bool signUp(User& user, time_t timeOfCreation);
    bool login(User& user);
    void logout(const User &user);

    bool changeProfile(User& user, const User& newUser);

    DialogueList getDialogueList(const User &user) const;
    std::vector<Message> getMessagesFromDialogue(const std::string &dialogueId) const;

    std::vector<User> getUsersByUserName(const std::string &findUser) const;
    void createDialogue(Dialogue &dialogue);

    void sendMessage(Dialogue &dialogue, Message &message);

//    void runCompilation(const Message &message, const std::wstring &stdIn); TODO

private:
    void postChatEvent(const ChatEvent& event);
    void notifyUser(const ChatEvent& event);

private:
    struct ClientInfo {
        std::string       sessionId;
        std::string       userId;
        ChatEventCallback eventCallback;
    };
    typedef std::map<Client *, ClientInfo> ClientMap;

    Wt::WServer&            server_;
    std::recursive_mutex    mutex_;
    ClientMap               clients_;
    MainDb                  db_;

    Auth                    auth_;
};


#endif  // PROJECT_INCLUDE_CHATSERVER_HPP_
