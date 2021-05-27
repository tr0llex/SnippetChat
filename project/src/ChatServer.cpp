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

bool ChatServer::signUp(User &user, time_t timeOfCreation) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    if (db_.writeUser(user) == EXIT_SUCCESS) {
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
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    user = db_.searchUserPassword(user.getLogin(), user.getPassword());

    if (!user.getLogin().empty()) {
        LoginData loginData(user.getLogin(), user.getPassword());
        loginData.setType(1);
        user.setToken(auth_.loginUser(loginData));
        /// Оповестить только друзей TODO

        return true;
    }

    return false;
}

void ChatServer::logout(const User &user) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    auth_.logoutUser(user.getLogin());

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

void ChatServer::createDialogue(Dialogue &dialogue) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    db_.createDialogue(dialogue);

    for (const auto &participant : dialogue.getParticipantsList()) {
        notifyUser(ChatEvent(ChatEvent::NewDialogue, participant, dialogue));
    }
}

void ChatServer::sendMessage(Dialogue &dialogue, Message &message) {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    db_.writeMessage(message);
    dialogue.pushNewMessage(message);

    for (const auto &participant : dialogue.getParticipantsList()) {
        notifyUser(ChatEvent(ChatEvent::NewMessage, participant, dialogue));
    }
}

std::string ChatServer::verifyToken(const std::string &token) {
    return auth_.verifyToken(token);
}

int ChatServer::runCompilation(ChatServer &this_serv, const User &user, const Message &message, const std::string input) {  //
    Compilation compilation = manager_.runCompilation(message.getMessageCode(), input, 2);
    std::string outputToMessage = "Output: \n" + compilation.getExecutionStdout();

    if (!compilation.getExecutionStderr().empty()) {
        outputToMessage += "\n\nError: \n" + compilation.getExecutionStderr();
    }

    this_serv.notifyUser(ChatEvent(ChatEvent::CompilationCode, user.getLogin(), message, outputToMessage));
    std::this_thread::yield();

    return 0;
}

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
        }
    }
}
