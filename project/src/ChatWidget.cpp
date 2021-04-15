#include "ChatWidget.hpp"
#include "ChatServer.hpp"

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WEnvironment.h>
#include <Wt/WInPlaceEdit.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>
#include <Wt/WPushButton.h>
#include <Wt/WCheckBox.h>


ChatWidget::ChatWidget(ChatServer& server)
        : WContainerWidget(),
          server_(server),
          loggedIn_(false),
          userList_(0) {
    letLogin();
}

ChatWidget::~ChatWidget() {
    logout();
}

void ChatWidget::connect() {
}

void ChatWidget::disconnect() {
}

void ChatWidget::letLogin() {
}

void ChatWidget::letRegistration() {
}

void ChatWidget::changeProfile(const User &newUser) {
}

bool ChatWidget::startChat(const User &user) {
    return false;
}

void ChatWidget::logout() {
}

void ChatWidget::createLayout(std::unique_ptr<Wt::WWidget> messages, std::unique_ptr<Wt::WWidget> userList,
                              std::unique_ptr<Wt::WWidget> messageEdit, std::unique_ptr<Wt::WWidget> sendButton,
                              std::unique_ptr<Wt::WWidget> logoutButton) {
}

void ChatWidget::updateDialogueList() {
}

void ChatWidget::render(Wt::WFlags<Wt::RenderFlag> flags) {
    WWebWidget::render(flags);
}

bool ChatWidget::loggedIn() const {
    return false;
}

void ChatWidget::login() {
}

void ChatWidget::changeName(const Wt::WString &name) {
}

void ChatWidget::send() {
}

void ChatWidget::updateUser(Wt::WCheckBox *b) {
}

void ChatWidget::processChatEvent(const ChatEvent &event) {
}


