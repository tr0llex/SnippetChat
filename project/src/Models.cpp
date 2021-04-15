#include "Models.hpp"

uint32_t User::getUserId() const {
    return userId_;
}

std::string User::getLogin() {
    return userLogin_;
}

std::string User::getPassword() {
    return userPassword_;
}

std::vector<uint32_t> User::getDialogues() {
    return userDialogueList_;
}

std::string User::getToken() {
    return userToken_;
}

int User::getStatus() const {
    return userStatus_;
}

void User::setUserId(uint32_t userId) {
    userId_ = userId;
}

void User::setLogin(const std::string &userLogin) {
    userLogin_ = userLogin;
}

void User::setPassword(const std::string &userPassword) {
    userPassword_ = userPassword;
}

void User::setDialogues(const std::vector<uint32_t> &dialogueList) {
    userDialogueList_ = dialogueList;
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

uint32_t Message::getMessageId() const {
    return messageId_;
}

uint32_t Message::getDialogueParentId() const {
    return dialogueParentId_;
}

uint32_t Message::getSenderId() const {
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

std::vector<Message> Dialogue::getDialogueMessageList() {
    return dialogueMessageList_;
}

std::vector<uint32_t> Dialogue::getParticipantsList() {
    return participantsList_;
}

uint32_t Dialogue::getDialogueId() const {
    return dialogueId_;
}

void Dialogue::setDialogueId(uint32_t id) {
    dialogueId_ = id;
}

void Dialogue::pushNewMessage(const Message &newMessage) {
    dialogueMessageList_.push_back(newMessage);
}

void Dialogue::pushNewParticipant(uint32_t newParticipantId) {
    participantsList_.push_back(newParticipantId);
}

uint32_t Compilation::getCompilationId() const {
    return compilationId_;
}

void Compilation::setCompilationId(const uint32_t &compilationId) {
    compilationId_ = compilationId;
}

uint32_t Compilation::getMessageId() const {
    return messageId_;
}

void Compilation::setMessageId(const uint32_t &messageId) {
    messageId_ = messageId;
}

std::string Compilation::getMessageCode() {
    return messageCode_;
}

void Compilation::setMessageCode(const std::string &messageCode) {
    messageCode_ = messageCode;
}

std::string Compilation::getCompilerStderr() {
    return compilerStderr_;
}

void Compilation::setCompilerStderr(const std::string &compilerStderr) {
    compilerStderr_ = compilerStderr;
}

std::string Compilation::getCompilerStdout() {
    return compilerStdout_;
}

void Compilation::setCompilerStdout(const std::string &compilerStdout) {
    compilerStdout_ = compilerStdout;
}

std::string Compilation::getExecutionStderr() {
    return executionStderr_;
}

void Compilation::setExecutionStderr(const std::string &executionStderr) {
    executionStderr_ = executionStderr;
}

std::string Compilation::getExecutionStdin() {
    return executionStdin_;
}

void Compilation::setExecutionStdin(const std::string &executionStdin) {
    executionStdin_ = executionStdin;
}

std::string Compilation::getExecutionStdout() {
    return executionStdout_;
}

void Compilation::setExecutionStdout(const std::string &executionStdout) {
    executionStdout_ = executionStdout;
}

std::string Compilation::getExecutionUsedMemory() {
    return executionUsedMemory_;
}

void Compilation::setExecutionUsedMemory(const std::string &executionUsedMemory) {
    executionUsedMemory_ = executionUsedMemory;
}

std::string Compilation::getExecutionTime() {
    return executionTime_;
}

void Compilation::setExecutionTime(const std::string &executionTime) {
    executionTime_ = executionTime;
}
