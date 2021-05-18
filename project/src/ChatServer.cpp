#include <Wt/WServer.h>

#include "ChatServer.hpp"


ChatServer::ChatServer(Wt::WServer &server)
        : server_(server) {}

bool ChatServer::connect(ChatServer::Client *client, const User &user, const ChatEventCallback &handleEvent) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    if (clients_.count(client) == 0) {
        ClientInfo clientInfo;

        clientInfo.sessionId = Wt::WApplication::instance()->sessionId();
        clientInfo.userId = user.getLogin();
        clientInfo.eventCallback = handleEvent;

        clients_[client] = clientInfo;

        return true;
    }

    return false;
}

bool ChatServer::disconnect(ChatServer::Client *client) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    return clients_.erase(client) == 1;
}

bool ChatServer::signUp(User &user) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    if (db_.writeUser(user) == EXIT_SUCCESS) {
        db_.createDialogue(user.getLogin(), user.getLogin());

        return true;
    }

    return false;
}

bool ChatServer::login(User &user) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    user = db_.searchUserPassword(user.getLogin(), user.getPassword());

    if (!user.getLogin().empty()) {
        /// Оповестить только друзей TODO

        return true;
    }

    return false;
}

void ChatServer::logout(const User &user) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    /// Оповестить только друзей TODO
}

bool ChatServer::changeProfile(User &user, const User &newUser) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    db_.changePassword(newUser);

    return false;
}

DialogueList ChatServer::getDialogueList(const User &user) const {
    return db_.getLastNDialoguesWithLastMessage(user, 20);
}

std::vector<Message> ChatServer::getMessagesFromDialogue(const std::string &dialogueId) const {
    return db_.getNLastMessagesFromDialogue(dialogueId, 100);
}

std::vector<User> ChatServer::getUsersByUserName(const std::string &findUser) const {
    std::vector<User> users;
    /// TODO кринж
    if (db_.searchUserLogin(findUser)) {
        users.emplace_back(findUser);
    }
    return users;
}

Dialogue ChatServer::createDialogue(const User &user, const User &otherUser) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    Dialogue dialogue = db_.createDialogue(user.getLogin(), otherUser.getLogin());

    notifyUser(ChatEvent(ChatEvent::NewDialogue, otherUser.getLogin(), dialogue));

    return dialogue;
}

void ChatServer::sendMessage(Dialogue &dialogue, Message &message) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    db_.writeMessage(message);
    dialogue.pushNewMessage(message);

    for (const auto &participant : dialogue.getParticipantsList()) {
        notifyUser(ChatEvent(ChatEvent::NewMessage, participant, dialogue));
    }
}

#include "unistd.h"

/*void handlerCompilation(std::list<Wt::WString> &resultList, const User &user, const Message &message, const std::wstring &stdIn) {
    sleep(5);
    const std::wstring& stdOut = stdIn;

    /// TODO
    DialogueInfo dialogueInfo(user.getUsername(), message);

    ChatEvent event(ChatEvent::CompilationCode, user.getId(), dialogueInfo, stdOut);

    auto callback = std::bind(client.eventCallback, event);
    server.post(client.sessionId, callback);
}*/
//void ChatServer::runCompilation(const User &user, const Message &message, const std::wstring &stdIn) {
//    std::thread compile(handlerCompilation, resultList_, user, message, stdIn);
//    compile.detach();
//}

void ChatServer::postChatEvent(const ChatEvent &event) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    Wt::WApplication *app = Wt::WApplication::instance();

    for (const auto &i : clients_) {
        if (app && app->sessionId() == i.second.sessionId) {
            i.second.eventCallback(event);
        }
        else {
            auto callback = std::bind(i.second.eventCallback, event);
            server_.post(i.second.sessionId, callback);
        }
    }
}

void ChatServer::notifyUser(const ChatEvent& event) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    for (const auto &i : clients_) {
        if (i.second.userId == event.userId_) {
            auto callback = std::bind(i.second.eventCallback, event);
            server_.post(i.second.sessionId, callback);
//            return; TODO оповещаем все сессии пользователя
        }
    }
}
