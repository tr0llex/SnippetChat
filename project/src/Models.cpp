#include "Models.hpp"

std::string User::getLogin() const {
    return userLogin_;
}

std::string User::getPassword() const {
    return userPassword_;
}

std::string User::getToken() const {
    return userToken_;
}

int User::getStatus() const {
    return userStatus_;
}

std::vector<std::string> User::getDialoguesList() const {
    return dialoguesList_;
}

void User::addDialogueToList(std::string dialogueId) {
    dialoguesList_.push_back(dialogueId);
}

void User::setPassword(const std::string &userPassword) {
    userPassword_ = userPassword;
}

void User::setToken(const std::string &userToken) {
    userToken_ = userToken;
}

void User::setStatus(int userStatus) {
    userStatus_ = userStatus;
}

void LoginData::setLogin(const std::string &login) {
    login_ = login;
}

void LoginData::setPassword(const std::string &password) {
    password_ = password;
}

void LoginData::setType(short type) {
    loginType_ = type;
}

std::string LoginData::getLogin() const {
    return login_;
}

std::string LoginData::getPassword() const {
    return password_;
}

short LoginData::get_type() const {
    return loginType_;
}

bool LoginData::operator==(const LoginData &ldt1) const {
    if (login_ == ldt1.getLogin()) {
        if (password_ == ldt1.getPassword()) {
            if (loginType_ == ldt1.get_type()) {
                return true;
            }
        }
    }
    return false;
}

std::string Message::getMessageId() const {
    return messageId_;
}

std::string Message::getDialogueParentId() const {
    return dialogueParentId_;
}

std::string Message::getSenderId() const {
    return senderId_;
}

std::string Message::getMessageText() {
    return messageText_;
}

std::string Message::getMessageCode() {
    return messageCode_;
}

time_t Message::getTimeSent() const {
    return timeSent_;
}

bool Message::isRead() {
    return isRead_;
}

std::vector<Message> Dialogue::getDialogueMessageList() {
    return dialogueMessageList_;
}

std::vector<std::string> Dialogue::getParticipantsList() {
    return participantsList_;
}

std::string Dialogue::getDialogueId() const {
    return dialogueId_;
}

std::string Dialogue::getName(User& user) const {
    if (participantsList_[0] == user.getLogin()) {
        return participantsList_[1];
    } else {
        return participantsList_[0];
    }
}


void Dialogue::pushNewMessage(const Message &newMessage) {
    dialogueMessageList_.push_back(newMessage);
}

void Dialogue::pushNewParticipant(std::string newParticipantId) {
    participantsList_.push_back(newParticipantId);
}
