#ifndef PROJECT_INCLUDE_CHATWIDGET_HPP_
#define PROJECT_INCLUDE_CHATWIDGET_HPP_

#include <Wt/WContainerWidget.h>
#include <Wt/WJavaScript.h>
#include <Wt/WSound.h>

#include "ChatServer.hpp"


const int kCountLastDialogues = 20;
const int kCountLastMessages = 100;

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

    void startChat();
    void switchDialogue(const Dialogue& dialogue);
    void logout();

protected:
    virtual void createMessengerLayout(
            std::unique_ptr<WWidget> dialogueName,
            std::unique_ptr<WWidget> userNameSearch,
            std::unique_ptr<WWidget> searchButton,
            std::unique_ptr<WWidget> backButton,
            std::unique_ptr<WWidget> snippetButton,
            std::unique_ptr<WWidget> messages,
            std::unique_ptr<WWidget> dialogueList,
            std::unique_ptr<WWidget> messageEdit,
            std::unique_ptr<WWidget> sendButton,
            std::unique_ptr<WWidget> logoutButton);

    virtual void updateDialogueList();
    virtual void blankDialoguePage();
    virtual void showNewMessage(const Message &message);

    virtual void render(Wt::WFlags<Wt::RenderFlag> flags);

protected:
    bool loggedIn() const;

private:
    void signUp();
    void login();
    void searchUser();
    void endSearch();
    void send();
    void editSnippet();

    void processChatEvent(const ChatEvent& event);

private:
    ChatServer    &server_;

    bool                loggedIn_;
    User                user_;
    Dialogue            currentDialogue_;
    DialogueList        dialogueList_;
    Snippet             currentSnippet_;

    Wt::WLineEdit *userLoginEdit_;
    Wt::WLineEdit *passwordEdit_;
    Wt::WLineEdit *confirmPasswordEdit_;
    Wt::WText     *statusMsg_;

    typedef Wt::Core::observing_ptr<Wt::WPushButton> ButtonPtr;
    typedef Wt::Core::observing_ptr<Wt::WContainerWidget> ContainerPtr;

    Wt::WLineEdit             *userNameSearch_;
    Wt::JSlot                  clearSearchInput_;
    ButtonPtr                  searchButton_;
    ButtonPtr                  endSearchButton_;
    ContainerPtr               dialogues_;
    Wt::WText                 *dialogueName_;
    Wt::WContainerWidget      *messages_;
    ButtonPtr                  snippetButton_;
    Wt::WContainerWidget      *editContainer_;
    Wt::WTextArea             *messageEdit_;
    Wt::JSlot                  clearMessageInput_;
    ButtonPtr                  sendButton_;

    std::unique_ptr<Wt::WSound> soundLogin_;
    std::unique_ptr<Wt::WSound> soundMessageReceived_;
};


#endif  // PROJECT_INCLUDE_CHATWIDGET_HPP_
