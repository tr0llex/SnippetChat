#include <Wt/WServer.h>

#include "ChatServer.hpp"


ChatServer::ChatServer(Wt::WServer &server, const MainDb &db)
        : server_(server), db_(db) {
}

bool ChatServer::connect(ChatServer::Client *client, const User &user, const ChatEventCallback &handleEvent) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    if (clients_.count(client) == 0) {
        ClientInfo clientInfo;

        clientInfo.sessionId = Wt::WApplication::instance()->sessionId();
        clientInfo.userLogin = user.getLogin();
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

bool ChatServer::createUser(User &user, time_t timeOfCreation) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    if (db_.writeUser(user)) {
        user.setToken(auth_.registerUser(user.getLogin(), user.getPassword()));

        std::vector<std::string> participantsList;
        participantsList.push_back(user.getLogin());
        Dialogue dialogue(participantsList, timeOfCreation);

        db_.createDialogue(dialogue);

        return true;
    }

    return false;
}

bool ChatServer::login(User &user) {
    User userFromDb = db_.getUser(user.getLogin());

    if (user.verification(userFromDb)) {
        user = userFromDb;

        LoginData loginData(user.getLogin(), user.getPassword());
        loginData.setType(1);
        user.setToken(auth_.loginUser(loginData));

        return true;
    }

    return false;
}

void ChatServer::logout(const User &user) {
    auth_.logoutUser(user.getLogin());
}

DialogueList ChatServer::getDialogueList(const User &user, int count) const {
    return db_.getLastNDialoguesWithLastMessage(user, count);
}

std::vector<Message> ChatServer::getMessagesFromDialogue(const std::string &dialogueId, int count) const {
    return db_.getNLastMessagesFromDialogue(dialogueId, count);
}

User ChatServer::getUserByLogin(const std::string &findUser) const {
    if (db_.searchUserLogin(findUser)) {
        return User(findUser);
    }

    return User();
}

void ChatServer::createDialogue(Dialogue &dialogue) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    db_.createDialogue(dialogue);

    for (const auto &participant : dialogue.getParticipantsList()) {
        notifyUser(ChatEvent(ChatEvent::kNewDialogue, participant, dialogue));
    }
}

void ChatServer::sendMessage(Dialogue &dialogue, Message &message) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    db_.writeMessage(message);
    dialogue.pushNewMessage(message);

    for (const auto &participant : dialogue.getParticipantsList()) {
        notifyUser(ChatEvent(ChatEvent::kNewMessage, participant, dialogue));
    }
}

std::string ChatServer::verifyToken(const std::string &token) {
    return auth_.verifyToken(token);
}

void ChatServer::runCompilation(ChatServer &this_serv, const User &user, const Message &message, const std::string input) {
    Compilation compilationResult = manager_.runCompilation(message.getSnippet(), input);

    this_serv.notifyUser(ChatEvent(ChatEvent::kCompilationCode,
                                   user.getLogin(),
                                   message.getDialogueParentId(),
                                   message.getId(),
                                   compilationResult));
    std::this_thread::yield();  // TODO
}

void ChatServer::notifyUser(const ChatEvent& event) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    for (const auto &i : clients_) {
        if (i.second.userLogin == event.userLogin_) {
            auto callback = std::bind(i.second.eventCallback, event);
            server_.post(i.second.sessionId, callback);
        }
    }
}
