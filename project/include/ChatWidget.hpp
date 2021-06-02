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

    bool cookieValidation(const std::string *cookie);

    void connect(const User &user);
    void disconnect();

    void letSignUp();
    void letLogin();

    void startChat();
    void switchDialogue(const Dialogue& dialogue);
    void logout();

protected:
    virtual void createMessengerLayout(
            std::unique_ptr<WWidget> dialogueName,
            std::unique_ptr<WWidget> userNameSearch,
            std::unique_ptr<WWidget> searchButton,
            std::unique_ptr<WWidget> endSearchButton,
            std::unique_ptr<WWidget> snippetButton,
            std::unique_ptr<WWidget> messages,
            std::unique_ptr<WWidget> dialogueList,
            std::unique_ptr<WWidget> messageEdit,
            std::unique_ptr<WWidget> sendButton,
            std::unique_ptr<WWidget> logoutButton);

    void updateDialogueList();
    void blankDialoguePage();
    void showNewMessage(const Message &message);
    bool loggedIn() const;

private:
    void signUp();
    void login();
    void searchUser();
    void endSearch();
    void sendMessage();
    void editSnippet();

    void processChatEvent(const ChatEvent& event);

private:
    ChatServer   &server_;

    bool          loggedIn_;
    User          user_;
    Dialogue      currentDialogue_;
    DialogueList  dialogueList_;
    Snippet       currentSnippet_;

    typedef Wt::Core::observing_ptr<Wt::WLineEdit>        LineEditPtr;
    typedef Wt::Core::observing_ptr<Wt::WText>            TextPtr;
    typedef Wt::Core::observing_ptr<Wt::WTextArea>        TextAreaPtr;
    typedef Wt::Core::observing_ptr<Wt::WPushButton>      ButtonPtr;
    typedef Wt::Core::observing_ptr<Wt::WContainerWidget> ContainerPtr;

    LineEditPtr    userLoginEdit_;
    LineEditPtr    passwordEdit_;
    LineEditPtr    confirmPasswordEdit_;
    TextPtr        statusMsg_;

    LineEditPtr    userNameSearch_;
    Wt::JSlot      clearSearchInput_;
    ButtonPtr      searchButton_;
    ButtonPtr      endSearchButton_;
    ContainerPtr   dialogues_;
    TextPtr        dialogueName_;
    ContainerPtr   messages_;
    ContainerPtr   editContainer_;
    ButtonPtr      snippetButton_;
    TextAreaPtr    messageEdit_;
    Wt::JSlot      clearMessageInput_;
    ButtonPtr      sendButton_;

    std::unique_ptr<Wt::WSound> soundMessageReceived_;
};


#endif  // PROJECT_INCLUDE_CHATWIDGET_HPP_
