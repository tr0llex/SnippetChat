#ifndef PROJECT_INCLUDE_CHATWIDGET_HPP_
#define PROJECT_INCLUDE_CHATWIDGET_HPP_

#include <Wt/WContainerWidget.h>
#include <Wt/WJavaScript.h>
#include <Wt/WSound.h>

#include "ChatServer.hpp"

class ChatEvent;

class ChatWidget : public Wt::WContainerWidget,
                   public ChatServer::Client {
public:
    explicit ChatWidget(ChatServer& server);
    ~ChatWidget();

    void connect();
    void disconnect();
    void letLogin();
    void letRegistration();
    void changeProfile(const User& newUser);
    bool startChat(const User& user);
    void logout();

protected:
    virtual void createLayout(std::unique_ptr<Wt::WWidget> messages, std::unique_ptr<Wt::WWidget> userList,
                              std::unique_ptr<Wt::WWidget> messageEdit,
                              std::unique_ptr<Wt::WWidget> sendButton, std::unique_ptr<Wt::WWidget> logoutButton);

    virtual void updateDialogueList();

    virtual void render(Wt::WFlags<Wt::RenderFlag> flags);

protected:
    bool loggedIn() const;

private:
    typedef std::map<Wt::WString, bool> UserMap;
    UserMap users_;

    ChatServer&   server_;
    bool          loggedIn_;

    User          user_;

    Wt::WLineEdit                *userNameEdit_;
    Wt::WText                    *statusMsg_;

    Wt::WContainerWidget         *messages_;
    Wt::WTextArea                *messageEdit_;
    Wt::Core::observing_ptr<Wt::WPushButton> sendButton_;
    Wt::Core::observing_ptr<Wt::WContainerWidget> userList_;

    void login();
    void changeName(const Wt::WString& name);
    void send();
    void updateUser(Wt::WCheckBox *b);

    void processChatEvent(const ChatEvent& event);
};


#endif  // PROJECT_INCLUDE_CHATWIDGET_HPP_
