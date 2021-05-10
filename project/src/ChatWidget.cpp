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

#include "ChatServer.hpp"

#include "ChatWidget.hpp"

ChatWidget::ChatWidget(ChatServer &server)
        : WContainerWidget(),
          server_(server),
          loggedIn_(false),
          dialogues_(nullptr) {
    soundLogin_ = std::make_unique<Wt::WSound>("resources/sounds/login.mp3");

    letLogin();
}

ChatWidget::~ChatWidget() {
    soundLogin_.reset();
    soundMessageReceived_.reset();
    logout();
}

void ChatWidget::connect(const User &user) {
    auto handler = std::bind(&ChatWidget::processChatEvent, this, std::placeholders::_1);
    if (server_.connect(this, user, handler)) {
        Wt::WApplication::instance()->enableUpdates(true);
    }
}

void ChatWidget::disconnect() {
    if (server_.disconnect(this)) {
        Wt::WApplication::instance()->enableUpdates(false);
    }
}

void ChatWidget::letSignUp() {
    clear();

    auto vLayout = setLayout(std::make_unique<Wt::WVBoxLayout>());

    auto hLayout_(std::make_unique<Wt::WHBoxLayout>());
    auto hLayout = hLayout_.get();
    vLayout->addLayout(std::move(hLayout_), 0,
                       Wt::AlignmentFlag::Top | Wt::AlignmentFlag::Left);

    hLayout->addWidget(std::make_unique<Wt::WLabel>("User name:"),
                       0, Wt::AlignmentFlag::Middle);

    userNameEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>("My name"),
                                       0, Wt::AlignmentFlag::Middle);
    userNameEdit_->setFocus();

    auto button = hLayout->addWidget(std::make_unique<Wt::WPushButton>("Create account"),
                                     0, Wt::AlignmentFlag::Middle);

    button->clicked().connect(this, &ChatWidget::signUp);
    userNameEdit_->enterPressed().connect(this, &ChatWidget::signUp);

    auto signIn = hLayout->addWidget(std::make_unique<Wt::WPushButton>("Sign in"),
                                     0, Wt::AlignmentFlag::Middle);
    signIn->clicked().connect(this, &ChatWidget::letLogin);

    statusMsg_ = vLayout->addWidget(std::make_unique<Wt::WText>());
    statusMsg_->setTextFormat(Wt::TextFormat::Plain);
}

void ChatWidget::letLogin() {
    clear();

    auto vLayout = setLayout(std::make_unique<Wt::WVBoxLayout>());

    auto hLayout_(std::make_unique<Wt::WHBoxLayout>());
    auto hLayout = hLayout_.get();
    vLayout->addLayout(std::move(hLayout_), 0,
                       Wt::AlignmentFlag::Top | Wt::AlignmentFlag::Left);

    hLayout->addWidget(std::make_unique<Wt::WLabel>("User name:"),
                       0, Wt::AlignmentFlag::Middle);

    userNameEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>("My name"),
                                       0, Wt::AlignmentFlag::Middle);
    userNameEdit_->setFocus();

    auto button = hLayout->addWidget(std::make_unique<Wt::WPushButton>("Sign in"),
                                     0, Wt::AlignmentFlag::Middle);

    button->clicked().connect(this, &ChatWidget::login);
    userNameEdit_->enterPressed().connect(this, &ChatWidget::login);

    auto signUp = hLayout->addWidget(std::make_unique<Wt::WPushButton>("Sign up"),
                                     0, Wt::AlignmentFlag::Middle);
    signUp->clicked().connect(this, &ChatWidget::letSignUp);

    statusMsg_ = vLayout->addWidget(std::make_unique<Wt::WText>());
    statusMsg_->setTextFormat(Wt::TextFormat::Plain);
}

void ChatWidget::letSetting() {
    /// TODO
}

void ChatWidget::startChat() {
    clear();

    userNameEdit_ = nullptr;

    auto dialogueNamePtr = std::make_unique<Wt::WText>(currentDialogue_.getName(user_));
    auto messagesPtr = std::make_unique<WContainerWidget>();
    auto userListPtr = std::make_unique<WContainerWidget>();
    auto messageEditPtr = std::make_unique<Wt::WTextArea>();
    auto sendButtonPtr = std::make_unique<Wt::WPushButton>("Send");
    auto settingButtonPtr = std::make_unique<Wt::WPushButton>("Setting");
    auto logoutButtonPtr = std::make_unique<Wt::WPushButton>("Logout");

    dialogueName_ = dialogueNamePtr.get();
    messages_ = messagesPtr.get();
    dialogues_ = userListPtr.get();
    messageEdit_ = messageEditPtr.get();
    sendButton_ = sendButtonPtr.get();
    Wt::Core::observing_ptr<Wt::WPushButton> settingButton = settingButtonPtr.get();
    Wt::Core::observing_ptr<Wt::WPushButton> logoutButton = logoutButtonPtr.get();

    dialogueName_->setStyleClass("chat-dialogue-name");

    messageEdit_->setRows(2);
    messageEdit_->setFocus();

    messages_->setOverflow(Wt::Overflow::Auto);
    dialogues_->setOverflow(Wt::Overflow::Auto);


    createLayout(std::move(dialogueNamePtr),
                 std::move(messagesPtr),
                 std::move(userListPtr),
                 std::move(messageEditPtr),
                 std::move(sendButtonPtr),
                 std::move(settingButtonPtr),
                 std::move(logoutButtonPtr));

    clearInput_.setJavaScript
            ("function(o, e) { setTimeout(function() {"
             "" + messageEdit_->jsRef() + ".value='';"
                                          "}, 0); }");

    Wt::WApplication::instance()->setConnectionMonitor(
            "window.monitor={ "
            "'onChange':function(type, newV) {"
            "var connected = window.monitor.status.connectionStatus != 0;"
            "if(connected) {"
            + messageEdit_->jsRef() + ".disabled=false;"
            + messageEdit_->jsRef() + ".placeholder='';"
                                      "} else { "
            + messageEdit_->jsRef() + ".disabled=true;"
            + messageEdit_->jsRef() + ".placeholder='connection lost';"
                                      "}"
                                      "}"
                                      "}");

    if (sendButton_) {
        sendButton_->clicked().connect(this, &ChatWidget::send);
        sendButton_->clicked().connect(clearInput_);
        sendButton_->clicked().connect((WWidget *) messageEdit_,
                                       &WWidget::setFocus);
    }
    messageEdit_->enterPressed().connect(this, &ChatWidget::send);
    messageEdit_->enterPressed().connect(clearInput_);
    messageEdit_->enterPressed().connect((WWidget *) messageEdit_,
                                         &WWidget::setFocus);

    messageEdit_->enterPressed().preventDefaultAction();

    if (settingButton) {
//        settingButton->clicked().connect(this, &ChatWidget::changeProfile);
    }

    if (logoutButton) {
        logoutButton->clicked().connect(this, &ChatWidget::logout);
    }

    /// TODO
    auto nameEdit = std::make_unique<Wt::WInPlaceEdit>();
    nameEdit->addStyleClass("name-edit");
    nameEdit->setButtonsEnabled(false);
    nameEdit->setText(user_.getUsername());
//    nameEdit->valueChanged().connect(this, &SimpleChatWidget::changeName);

    Wt::WTemplate *joinMsg = messages_->addWidget(std::make_unique<Wt::WTemplate>(tr("join-msg.template")));
    joinMsg->bindWidget("name", std::move(nameEdit));
    joinMsg->setStyleClass("chat-msg");

    switchDialogue(*dialogueList_.begin());
    updateDialogueList();
}

/// TODO
void ChatWidget::switchDialogue(const DialogueInfo &dialogue) {
    messages_->clear();

    currentDialogue_ = server_.getDialogue(dialogue.getId());
    dialogueName_->setText(currentDialogue_.getName(user_));

    for (const auto& message : currentDialogue_.getMessages()) {
        showNewMessage(message);
    }
}

void ChatWidget::changeProfile(const User &newUser) {
    if (loggedIn()) {
        letSetting();
    }
}

void ChatWidget::logout() {
    if (loggedIn()) {
        loggedIn_ = false;
        server_.logout(user_);
        disconnect();

        letLogin();
    }
}

void ChatWidget::createLayout(std::unique_ptr<WWidget> dialogueName, std::unique_ptr<WWidget> messages,
                              std::unique_ptr<WWidget> dialogueList, std::unique_ptr<WWidget> messageEdit,
                              std::unique_ptr<WWidget> sendButton, std::unique_ptr<WWidget> settingButton,
                              std::unique_ptr<WWidget> logoutButton) {
    auto vLayout = std::make_unique<Wt::WVBoxLayout>();

    // Create a horizontal layout for the messages | userslist.
    auto hLayout = std::make_unique<Wt::WHBoxLayout>();

    // хз
    hLayout->setPreferredImplementation(Wt::LayoutImplementation::JavaScript);

    // кнопки
    hLayout->addWidget(std::move(dialogueName), 1);
    hLayout->addWidget(std::move(settingButton));
    hLayout->addWidget(std::move(logoutButton));
    vLayout->addLayout(std::move(hLayout), 0);

    // Обновили горизонтальный слой
    hLayout = std::make_unique<Wt::WHBoxLayout>();

    // список диалогов
    dialogueList->setStyleClass("chat-getDialoguesView");
    hLayout->addWidget(std::move(dialogueList));
//    hLayout->setResizable(0, true);

    // диалог+эдит
    auto v2Layout = std::make_unique<Wt::WVBoxLayout>();
    messages->setStyleClass("chat-msgs");
    v2Layout->addWidget(std::move(messages), 1);

    auto hEditLayout = std::make_unique<Wt::WHBoxLayout>();
    messageEdit->setStyleClass("chat-noedit");
    hEditLayout->addWidget(std::move(messageEdit), 1);
    hEditLayout->addWidget(std::move(sendButton));
    v2Layout->addLayout(std::move(hEditLayout));

    hLayout->addLayout(std::move(v2Layout), 1);


    vLayout->addLayout(std::move(hLayout), 1);


    this->setLayout(std::move(vLayout));
}

void ChatWidget::updateDialogueList() {
    if (!dialogues_) {
        return;
    }

    dialogues_->clear();

//    dialogueList_ = server_.getDialogueList(user_);

    for (const auto& dialogue : dialogueList_) {
        Wt::WText *w = dialogues_->addWidget(std::make_unique<Wt::WText>(escapeText(dialogue.getName())));
        w->setStyleClass("reactive");

        w->setInline(false);

        w->clicked().connect([&] {
            switchDialogue(dialogue);
        });

        if (dialogue.withYourself(user_)) {
            w->setStyleClass("chat-self");
        }
    }
}

static inline Wt::WString formattedMessage(const User &user, const Message& message) {
    Wt::WString result;

    result = Wt::WString("<span class='")
             + ((message.isMyMessage(user)) ?
                "chat-self" :
                "chat-user")
             + "'>" + Wt::WWebWidget::escapeText(std::to_wstring(message.getSenderId())) + ":</span>";

    return result + message.getMessageText() + " " + message.timeSent();
}
void ChatWidget::showNewMessage(const Message &message) {
    Wt::WApplication *app = Wt::WApplication::instance();

    Wt::WText *w = messages_->addWidget(std::make_unique<Wt::WText>());

    w->setText(formattedMessage(user_, message));

    w->setInline(false);
    w->setStyleClass("chat-msg");

    if (messages_->count() > 100) {
        messages_->removeChild(messages_->children()[0]);
    }

    app->doJavaScript(messages_->jsRef() + ".scrollTop += "
                      + messages_->jsRef() + ".scrollHeight;");
}

void ChatWidget::render(Wt::WFlags<Wt::RenderFlag> flags) {
    if (flags.test(Wt::RenderFlag::Full)) {
        if (loggedIn()) {
            /* Handle a page refresh correctly */
            messageEdit_->setText(Wt::WString::Empty);
            doJavaScript("setTimeout(function() { "
                         + messages_->jsRef() + ".scrollTop += "
                         + messages_->jsRef() + ".scrollHeight;}, 0);");
        }
    }

    WContainerWidget::render(flags);
}

bool ChatWidget::loggedIn() const {
    return loggedIn_;
}

void ChatWidget::signUp() {
    if (loggedIn()) {
        return;
    }

    Wt::WString username = userNameEdit_->text();

    user_ = User(username);

    if (server_.signUp(user_)) {
        login();
    } else {
        statusMsg_->setText("Sorry, name '" + escapeText(username) +
                            "' is already taken.");
    }
}

void ChatWidget::login() {
    if (loggedIn()) {
        return;
    }

    Wt::WString username = userNameEdit_->text();

    if (!soundMessageReceived_) {
        soundMessageReceived_ = std::make_unique<Wt::WSound>("resources/sounds/message_received.mp3");
    }

    user_ = User(username);

    if (server_.login(user_)) {
        loggedIn_ = true;
        dialogueList_ = server_.getDialogueList(user_);

        if (soundLogin_) {
            soundLogin_->play();
        }

        connect(user_);

        startChat();
    } else {
        statusMsg_->setText("Incorrect login or password");
    }
}

void ChatWidget::send() {
    if (!messageEdit_->text().empty()) {
        Message message(user_, currentDialogue_.getId(), messageEdit_->text());
        server_.sendMessage(user_, currentDialogue_, message);

        showNewMessage(message);
    }
}

void ChatWidget::processChatEvent(const ChatEvent &event) {
    Wt::WApplication *app = Wt::WApplication::instance();

    app->triggerUpdate();

    switch (event.type()) {
        case ChatEvent::Login: {
            updateDialogueList();
            break;
        }
        case ChatEvent::Logout: {
            updateDialogueList();
            break;
        }
        case ChatEvent::UpdateProfile: {
            updateDialogueList();
            break;
        }
        case ChatEvent::NewDialogue: {
            updateDialogueList();
            break;
        }
        case ChatEvent::NewMessage: {
            if (event.getSenderId() != user_.getId()) {
                if (currentDialogue_ == event.dialogue()) {
                    showNewMessage(event.dialogue().getMessage());

                    if (soundMessageReceived_) {
                        soundMessageReceived_->play();
                    }
                }
            } else {
                // заебись сообщение отправлено
            }

            break;
        }
        case ChatEvent::RunCode: {
            break;
        }
        default:
            break;
    }
}
