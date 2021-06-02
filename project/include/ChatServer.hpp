#ifndef PROJECT_INCLUDE_CHATSERVER_HPP_
#define PROJECT_INCLUDE_CHATSERVER_HPP_


#include <map>
#include <mutex>
#include <thread>

#include "Auth.hpp"
#include "ChatEvent.hpp"
#include "CompilationManager.hpp"
#include "MainDb.hpp"
#include "Models.hpp"


class ChatServer {
public:
    class Client {
    };

    explicit ChatServer(Wt::WServer &server, const MainDb &db);

    ChatServer(const ChatServer &) = delete;

    ChatServer &operator=(const ChatServer &) = delete;

    bool connect(Client *client, const User &user, const ChatEventCallback &handleEvent);

    bool disconnect(Client *client);

    bool createUser(User &user, time_t timeOfCreation);

    bool login(User &user);

    void logout(const User &user);

    DialogueList getDialogueList(const User &user, int count) const;

    std::vector<Message> getMessagesFromDialogue(const std::string &dialogueId, int count) const;

    User getUserByLogin(const std::string &findUser) const;

    void createDialogue(Dialogue &dialogue);

    void sendMessage(Dialogue &dialogue, Message &message);

    std::string verifyToken(const std::string &token);

    void runCompilation(ChatServer &this_serv, User user, Message message, std::string input);

private:
    struct ClientInfo {
        std::string sessionId;
        std::string userLogin;
        ChatEventCallback eventCallback;
    };
    typedef std::map<Client *, ClientInfo> ClientMap;

    void notifyUser(const ChatEvent &event);

private:
    Wt::WServer &server_;
    std::recursive_mutex mutex_;
    ClientMap clients_;
    MainDb db_;

    Auth auth_;

    CompilationManager manager_;
};


#endif  // PROJECT_INCLUDE_CHATSERVER_HPP_
