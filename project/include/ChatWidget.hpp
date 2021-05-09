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
    ~ChatWidget() override;

    void connect(const User &user);
    void disconnect();

    void letSignUp();
    void letLogin();
    void letSetting();

    void startChat();
    void switchDialogue(const DialogueInfo& dialogue);
    void changeProfile(const User& newUser);
    void logout();

protected:
    virtual void createLayout(
            std::unique_ptr<WWidget> dialogueName,
            std::unique_ptr<WWidget> messages,
            std::unique_ptr<WWidget> dialogueList,
            std::unique_ptr<WWidget> messageEdit,
            std::unique_ptr<WWidget> sendButton,
            std::unique_ptr<WWidget> settingButton,
            std::unique_ptr<WWidget> logoutButton);

    virtual void updateDialogueList();
    virtual void newMessage();

    virtual void render(Wt::WFlags<Wt::RenderFlag> flags);

protected:
    bool loggedIn() const;

private:
    void signUp();
    void login();
    void send();

    void processChatEvent(const ChatEvent& event);

private:
    ChatServer    &server_;

    bool           loggedIn_;
    User           user_;
    Dialogue       currentDialogue_;
    DialogueList   dialogueList_;

    Wt::WLineEdit *userNameEdit_;
    Wt::WText     *statusMsg_;

    Wt::WText                                    *dialogueName_;
    Wt::WContainerWidget                         *messages_;
    Wt::WTextArea                                *messageEdit_;
    Wt::JSlot                                     clearInput_;
    Wt::Core::observing_ptr<Wt::WPushButton>      sendButton_;
    Wt::Core::observing_ptr<Wt::WContainerWidget> dialogues_;

    std::unique_ptr<Wt::WSound> soundLogin_;
    std::unique_ptr<Wt::WSound> soundMessageReceived_;
};


#endif  // PROJECT_INCLUDE_CHATWIDGET_HPP_
