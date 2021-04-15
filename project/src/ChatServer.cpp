#include "ChatServer.hpp"

#include <Wt/WServer.h>


ChatServer::ChatServer(Wt::WServer& server)
        : server_(server) {
}

bool ChatServer::connect(ChatServer::Client *client, const ChatEventCallback &handleEvent) {
    return false;
}

bool ChatServer::disconnect(ChatServer::Client *client) {
    return false;
}

bool ChatServer::login(const User &user) {
    return false;
}

void ChatServer::logout(const User &user) {
}

bool ChatServer::signUp(const User &user) {
    return false;
}

void ChatServer::changeProfile(User &user, const User &newUser) {
}

void ChatServer::sendMessage(const User &user, const Wt::WString &message) {
}

void ChatServer::runCode(const User &user, const Wt::WString &dataStdin) {
}

void ChatServer::postChatEvent(const ChatEvent &event) {
}



