#include "Models.hpp"

#include <locale>
#include <codecvt>

std::string ws2s(const std::wstring &wstr) {
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

std::string User::getLogin() const {
    return userLogin_;
}

std::string User::getPassword() const {
    return userPassword_;
}


std::string User::getToken() const {
    return userToken_;
}

bool User::operator==(const User &user) const {
    return userLogin_ == user.getLogin();
}
bool User::operator!=(const User &user) const {
    return userLogin_ != user.getLogin();
}

Message Dialogue::getLastMessage() const {
    return dialogueMessageList_.back();
}

int User::getStatus() const {
    return userStatus_;
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

short LoginData::getType() const {
    return loginType_;
}

bool LoginData::operator==(const LoginData &ldt1) const {
    if (login_ == ldt1.getLogin()) {
        if (password_ == ldt1.getPassword()) {
            if (loginType_ == ldt1.getType()) {
                return true;
            }
        }
    }
    return false;
}

Message::Message(const std::string &dialogueParentId, const std::string &senderId, const std::string &messageText,
                 time_t timeSent, const std::string &messageCode)
                 : dialogueParentId_(dialogueParentId), senderLogin_(senderId), messageText_(messageText),
                   messageCode_(messageCode), timeSent_(timeSent), isRead_(false) {

}

Message::Message(const std::string &messageId, const std::string &dialogueParentId, const std::string &senderId,
                 std::string messageText, std::string messageCode, time_t timeSent, bool isRead)
                 : id_(messageId), dialogueParentId_(dialogueParentId), senderLogin_(senderId),
                   messageText_(std::move(messageText)), messageCode_(std::move(messageCode)),
                   timeSent_(timeSent), isRead_(isRead) {
}

std::string Message::getId() const {
    return id_;
}

void Message::setId(std::string id) {
    id_ = id;
}


std::string Message::getDialogueParentId() const {
    return dialogueParentId_;
}

std::string Message::getSenderLogin() const {
    return senderLogin_;
}

std::string Message::getMessageText() const {
    return messageText_;
}

std::string Message::getMessageCode() const {
    return messageCode_;
}

time_t Message::getTimeSent() const {
    return timeSent_;
}

bool Message::isRead() {
    return isRead_;
}

std::string Message::getTimeSentStr() const {
    uint64_t minutes = timeSent_ / 60000;
    uint64_t hours = minutes / 60;

    uint8_t mm = minutes % 60;
    uint8_t hh = (3 + hours) % 24;

    std::string minutesStr;
    if (mm < 10) {
        minutesStr += "0";
    }
    minutesStr += std::to_string(mm);

    return std::to_string(hh) + ":" + minutesStr;
}

bool Message::isHaveCode() const {
    return !messageCode_.empty();
}

std::vector<Message> Dialogue::getDialogueMessageList() {
    return dialogueMessageList_;
}

std::vector<std::string> Dialogue::getParticipantsList() {
    return participantsList_;
}

std::string Dialogue::getId() const {
    return id_;
}

void Dialogue::setId(std::string id) {
    id_ = id;
}

std::string Dialogue::getName(const User &requester) const {
    if (withYourself()) {
        return requester.getLogin();
    }

    if (participantsList_[0] == requester.getLogin()) {
        return participantsList_[1];
    } else {
        return participantsList_[0];
    }
}

time_t Dialogue::getTimeOfLastUpdate() const {
    if (dialogueMessageList_.empty()) {
        return dateOfCreation_;
    }
    return getLastMessage().getTimeSent();
}

bool Dialogue::isEmpty() const {
    return participantsList_.empty();
}

void Dialogue::pushNewMessage(const Message &newMessage) {
    dialogueMessageList_.push_back(newMessage);
}

void Dialogue::updateLastMessage(const Message &newMessage) {
    dialogueMessageList_.clear();
    dialogueMessageList_.push_back(newMessage);
}

void Dialogue::pushNewParticipant(std::string newParticipantId) {
    participantsList_.push_back(newParticipantId);
}

void Dialogue::pushMessages(const std::vector<Message> &dialogueMessageList) {
    for (const auto &message : dialogueMessageList) {
        pushNewMessage(message);
    }
}

bool Dialogue::withYourself() const {
    return participantsList_.size() == 1;
}

bool Dialogue::operator==(const Dialogue &dialogue) const {
    return id_ == dialogue.getId();
}

bool Dialogue::operator!=(const Dialogue &dialogue) const {
    return id_ != dialogue.getId();
}
