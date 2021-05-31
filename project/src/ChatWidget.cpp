#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WDialog.h>
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

#include <chrono>

#include "ChatServer.hpp"
#include "CodeWidget.hpp"
#include "DialogueWidget.hpp"
#include "MessageWidget.hpp"
#include "SnippetEditWidget.hpp"

#include "ChatWidget.hpp"


static inline int64_t getCurrentTimeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
}

ChatWidget::ChatWidget(ChatServer &server)
        : WContainerWidget(),
          server_(server),
          loggedIn_(false),
          dialogues_(nullptr) {
    soundLogin_ = std::make_unique<Wt::WSound>("resources/sounds/login.mp3");


    auto cookie = Wt::WApplication::instance()->environment().getCookie("userToken");

    if (cookie && !cookie->empty()) {
        std::string userLogin = server_.verifyToken(*cookie);
        if (!userLogin.empty()) {
            user_.setUserLogin(userLogin);
            user_.setToken(*cookie);

            connect(user_);

            loggedIn_ = true;
            dialogueList_ = server_.getDialogueList(user_, kCountLastDialogues);

            if (soundLogin_) {
                soundLogin_->play();
            }
            if (!soundMessageReceived_) {
                soundMessageReceived_ = std::make_unique<Wt::WSound>("resources/sounds/message_received.mp3");
            }

            startChat();
        } else {
            letLogin();
        }
    } else {
        letLogin();
    }
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

    auto vContainer = vLayout->addWidget(std::make_unique<Wt::WContainerWidget>());

    vContainer->setStyleClass("auth-block");

    auto vLayout2 = vContainer->setLayout(std::make_unique<Wt::WVBoxLayout>());

    auto image = vLayout2->addWidget(std::make_unique<Wt::WImage>(Wt::WLink("resources/LOGO.png")));
    image->setAlternateText("SNICH");
    image->setStyleClass("snich");
    image->setMaximumSize(400, 500);

    statusMsg_ = vLayout2->addWidget(std::make_unique<Wt::WText>(), 1, Wt::AlignmentFlag::Center);
    statusMsg_->setTextFormat(Wt::TextFormat::Plain);
    statusMsg_->setStyleClass("status-msg");

    auto hLayout = std::make_unique<Wt::WHBoxLayout>();

    hLayout->addWidget(std::make_unique<Wt::WLabel>("Login:"), 1);

    userLoginEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>(), 1);
    userLoginEdit_->setFocus();
    userLoginEdit_->setStyleClass("pass-log");

    vLayout2->addLayout(std::move(hLayout));


    hLayout = std::make_unique<Wt::WHBoxLayout>();

    hLayout->addWidget(std::make_unique<Wt::WLabel>("Password:"), 1);

    passwordEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>(), 1);
    passwordEdit_->setAttributeValue("type", "password");
    passwordEdit_->setStyleClass("pass-log");


    vLayout2->addLayout(std::move(hLayout));


    hLayout = std::make_unique<Wt::WHBoxLayout>();

    hLayout->addWidget(std::make_unique<Wt::WLabel>("Confirm password:"), 1);

    confirmPasswordEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>(), 1);
    confirmPasswordEdit_->setAttributeValue("type", "password");
    confirmPasswordEdit_->setStyleClass("pass-log");


    vLayout2->addLayout(std::move(hLayout));

    auto hLayout2 = std::make_unique<Wt::WHBoxLayout>();

    auto signUp = hLayout2->addWidget(std::make_unique<Wt::WPushButton>("Sign up"));

    signUp->setStyleClass("auth-buttons");

    signUp->clicked().connect(this, &ChatWidget::signUp);
    userLoginEdit_->enterPressed().connect(this, [&] {
        passwordEdit_->setFocus();
    });
    passwordEdit_->enterPressed().connect(this, [&] {
        confirmPasswordEdit_->setFocus();
    });
    confirmPasswordEdit_->enterPressed().connect(this, &ChatWidget::signUp);


    auto logIn = hLayout2->addWidget(std::make_unique<Wt::WPushButton>("Log in"));
    logIn->clicked().connect(this, &ChatWidget::letLogin);
    logIn->setStyleClass("auth-buttons");
    logIn->addStyleClass("signup-button");


    vLayout2->addLayout(std::move(hLayout2));

    this->setStyleClass("block-block");
}

void ChatWidget::letLogin() {
    clear();

    auto hPositionalLayout = std::make_unique<Wt::WHBoxLayout>();

    auto vContainer = hPositionalLayout->addWidget(std::make_unique<Wt::WContainerWidget>());

    vContainer->setStyleClass("auth-block");

    auto vLayout = vContainer->setLayout(std::make_unique<Wt::WVBoxLayout>());

    auto image = vLayout->addWidget(std::make_unique<Wt::WImage>(Wt::WLink("resources/LOGO.png")));
    image->setAlternateText("SNICH");
    image->setStyleClass("snich");
    image->setMaximumSize(400, 500);

    auto hLayout = std::make_unique<Wt::WHBoxLayout>();

    statusMsg_ = vLayout->addWidget(std::make_unique<Wt::WText>(), 1, Wt::AlignmentFlag::Center);
    statusMsg_->addStyleClass("status-msg");
    statusMsg_->setTextFormat(Wt::TextFormat::Plain);

    hLayout->addWidget(std::make_unique<Wt::WLabel>("Login:"), 1);

    userLoginEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>(), 1);
    userLoginEdit_->setFocus();

    userLoginEdit_->setStyleClass("pass-log");

    vLayout->addLayout(std::move(hLayout));

    hLayout = std::make_unique<Wt::WHBoxLayout>();

    hLayout->addWidget(std::make_unique<Wt::WLabel>("Password:"), 1);

    passwordEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>(), 1);
    passwordEdit_->setAttributeValue("type", "password");

    passwordEdit_->setStyleClass("pass-log");

    auto hLayout2 = std::make_unique<Wt::WHBoxLayout>();

    auto logIn = hLayout2->addWidget(std::make_unique<Wt::WPushButton>("Log in"), 0);
    logIn->setStyleClass("auth-buttons");

    logIn->clicked().connect(this, &ChatWidget::login);
    userLoginEdit_->enterPressed().connect(this, [&] {
        passwordEdit_->setFocus();
    });
    passwordEdit_->enterPressed().connect(this, &ChatWidget::login);


    auto signUp = hLayout2->addWidget(std::make_unique<Wt::WPushButton>("Sign up"), 0);
    signUp->setStyleClass("auth-buttons");
    signUp->addStyleClass("signup-button");
    signUp->clicked().connect(this, &ChatWidget::letSignUp);

    vLayout->addLayout(std::move(hLayout));

    vLayout->addLayout(std::move(hLayout2));

    this->setLayout(std::move(hPositionalLayout));

    this->setStyleClass("block-block");
}

void ChatWidget::startChat() {
    clear();

    this->setStyleClass("chat");

    userLoginEdit_ = nullptr;

    auto dialogueNamePtr = std::make_unique<Wt::WText>();
    auto userNameSearchPtr = std::make_unique<Wt::WLineEdit>();
    auto searchButtonPtr = std::make_unique<Wt::WPushButton>();
    auto endSearchButtonPtr = std::make_unique<Wt::WPushButton>();
    auto snippetButtonPtr = std::make_unique<Wt::WPushButton>();
    auto messagesPtr = std::make_unique<WContainerWidget>();
    auto userListPtr = std::make_unique<WContainerWidget>();
    auto messageEditPtr = std::make_unique<Wt::WTextArea>();
    auto sendButtonPtr = std::make_unique<Wt::WPushButton>();
    auto logoutButtonPtr = std::make_unique<Wt::WPushButton>("Logout");

    dialogueName_ = dialogueNamePtr.get();
    userNameSearch_ = userNameSearchPtr.get();
    searchButton_ = searchButtonPtr.get();
    endSearchButton_ = endSearchButtonPtr.get();
    snippetButton_ = snippetButtonPtr.get();
    messages_ = messagesPtr.get();
    dialogues_ = userListPtr.get();
    messageEdit_ = messageEditPtr.get();
    sendButton_ = sendButtonPtr.get();
    Wt::Core::observing_ptr<Wt::WPushButton> logoutButton = logoutButtonPtr.get();

    dialogueName_->setStyleClass("chat-dialogue-name");
    messageEdit_->setStyleClass("message-edit");
    searchButton_->addStyleClass("chat-button bi-search");
    endSearchButton_->addStyleClass("chat-button bi-x-lg");
    snippetButton_->addStyleClass("chat-button bi bi-file-earmark-code");
    sendButton_->addStyleClass("chat-button bi bi-arrow-right");


    messageEdit_->setRows(2);

    messages_->setOverflow(Wt::Overflow::Auto);
    dialogues_->setOverflow(Wt::Overflow::Auto);

    createMessengerLayout(std::move(dialogueNamePtr),
                          std::move(userNameSearchPtr),
                          std::move(searchButtonPtr),
                          std::move(endSearchButtonPtr),
                          std::move(snippetButtonPtr),
                          std::move(messagesPtr),
                          std::move(userListPtr),
                          std::move(messageEditPtr),
                          std::move(sendButtonPtr),
                          std::move(logoutButtonPtr));

    clearMessageInput_.setJavaScript
            ("function(o, e) { setTimeout(function() {"
             "" + messageEdit_->jsRef() + ".value='';"
                                          "}, 0); }");
    clearSearchInput_.setJavaScript
            ("function(o, e) { setTimeout(function() {"
             "" + userNameSearch_->jsRef() + ".value='';"
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

    Wt::WApplication *app = Wt::WApplication::instance();
    app->addAutoJavaScript("hljs.highlightAll();");

    searchButton_->clicked().connect(this, &ChatWidget::searchUser);
    searchButton_->clicked().connect(clearSearchInput_);

    userNameSearch_->enterPressed().connect(this, &ChatWidget::searchUser);
    userNameSearch_->enterPressed().connect(clearSearchInput_);

    endSearchButton_->clicked().connect(this, &ChatWidget::endSearch);
    endSearchButton_->clicked().connect(clearSearchInput_);

    if (sendButton_) {
        sendButton_->clicked().connect(this, &ChatWidget::send);
        sendButton_->clicked().connect(clearMessageInput_);
        sendButton_->clicked().connect((WWidget *) messageEdit_,
                                       &WWidget::setFocus);
    }
    messageEdit_->enterPressed().connect(this, &ChatWidget::send);
    messageEdit_->enterPressed().connect(clearMessageInput_);
    messageEdit_->enterPressed().connect((WWidget *) messageEdit_,
                                         &WWidget::setFocus);

    messageEdit_->enterPressed().preventDefaultAction();

    snippetButton_->clicked().connect(this, &ChatWidget::editSnippet);

    if (logoutButton) {
        logoutButton->clicked().connect(this, &ChatWidget::logout);
    }

    updateDialogueList();
    blankDialoguePage();
}

void ChatWidget::switchDialogue(const Dialogue &dialogue) {
    messages_->clear();

    messageEdit_->enable();
    messageEdit_->setFocus();
    sendButton_->enable();
    snippetButton_->enable();

    currentDialogue_ = dialogue;

    std::vector<Message> messages = server_.getMessagesFromDialogue(dialogue.getId(), kCountLastMessages);
    currentDialogue_.pushMessages(messages);

    dialogueName_->setText(currentDialogue_.getName(user_));

    for (const auto &message : messages) {
        showNewMessage(message);
    }
}

void ChatWidget::logout() {
    if (loggedIn()) {
        loggedIn_ = false;
        currentDialogue_ = Dialogue();
        dialogueList_.clear();
        // TODO удалять все обьекты
        dialogueName_ = nullptr;

        server_.logout(user_);
        disconnect();

        Wt::WApplication::instance()->removeCookie("userToken");

        letLogin();
    }
}

void ChatWidget::createMessengerLayout(std::unique_ptr<WWidget> dialogueName, std::unique_ptr<WWidget> userNameSearch,
                                       std::unique_ptr<WWidget> searchButton, std::unique_ptr<WWidget> backButton,
                                       std::unique_ptr<WWidget> snippetButton, std::unique_ptr<WWidget> messages,
                                       std::unique_ptr<WWidget> dialogueList, std::unique_ptr<WWidget> messageEdit,
                                       std::unique_ptr<WWidget> sendButton, std::unique_ptr<WWidget> logoutButton) {
    auto vLayout = std::make_unique<Wt::WVBoxLayout>();

    auto hLayout = std::make_unique<Wt::WHBoxLayout>();

    /// <Шапка>
    hLayout->addWidget(std::make_unique<Wt::WText>(tr("projectName")), 0, Wt::AlignmentFlag::Left);
    hLayout->addWidget(std::move(logoutButton), 0, Wt::AlignmentFlag::Right);
    vLayout->addLayout(std::move(hLayout), 0);
    /// </Шапка>

    /// <Тело>
    hLayout = std::make_unique<Wt::WHBoxLayout>();

    /// <Левая часть>
    auto vLeftLayout = std::make_unique<Wt::WVBoxLayout>();

    /// <Поиск>
    auto hSearchLayout = std::make_unique<Wt::WHBoxLayout>();
    userNameSearch->setStyleClass("chat-search");
    hSearchLayout->addWidget(std::move(userNameSearch), 1);
    hSearchLayout->addWidget(std::move(searchButton));
    hSearchLayout->addWidget(std::move(backButton));
    vLeftLayout->addLayout(std::move(hSearchLayout));
    /// </Поиск>

    /// <Список диалогов>
    dialogueList->setStyleClass("chat-getDialoguesView");
    vLeftLayout->addWidget(std::move(dialogueList), 1);
    /// </Список диалогов>

    hLayout->addLayout(std::move(vLeftLayout), 0);
    /// </Левая часть>

    /// <Правая часть>
    auto vRightLayout = std::make_unique<Wt::WVBoxLayout>();

    /// <Шапка диалога>
    auto hRightLayout = std::make_unique<Wt::WHBoxLayout>();
    hRightLayout->addWidget(std::move(dialogueName));
    vRightLayout->addLayout(std::move(hRightLayout), 0);
    /// </Шапка диалога>

    /// <Сообщения>
    messages->setStyleClass("chat-msgs");
    vRightLayout->addWidget(std::move(messages), 1);
    /// </Сообщения>

    /// <Поле ввода>
    hRightLayout = std::make_unique<Wt::WHBoxLayout>();
    hRightLayout->addWidget(std::move(snippetButton));
    messageEdit->addStyleClass("chat-noedit");
    hRightLayout->addWidget(std::move(messageEdit), 1);
    hRightLayout->addWidget(std::move(sendButton));
    /// </Поле ввода>

    vRightLayout->addLayout(std::move(hRightLayout));
    /// </Правая часть>

    hLayout->addLayout(std::move(vRightLayout), 1);
    /// </Тело>

    vLayout->addLayout(std::move(hLayout), 1);

    this->setLayout(std::move(vLayout));
}

void ChatWidget::updateDialogueList() {
    if (!dialogues_) {
        return;
    }

    dialogues_->clear();

    for (const auto& dialogue : dialogueList_) {
        auto dialogueWidget = std::make_unique<DialogueWidget>(dialogue.getName(user_), dialogue);
        DialogueWidget *w = dialogues_->addWidget(std::move(dialogueWidget));
        w->setStyleClass("dialogue-block");

        w->setInline(false);

        w->clicked().connect([&] {
            switchDialogue(dialogue);
        });
    }
}

void ChatWidget::blankDialoguePage() {
    currentDialogue_ = Dialogue();

    dialogueName_->setText("");

    messages_->clear();

    messageEdit_->disable();
    sendButton_->disable();
    snippetButton_->disable();
}

void ChatWidget::showNewMessage(const Message &message) {
    Wt::WApplication *app = Wt::WApplication::instance();

    bool myMessage = message.getSenderLogin() == user_.getLogin();
    auto messageWidgetPtr = std::make_unique<MessageWidget>(message, myMessage);
    auto messageWidget = messageWidgetPtr.get();

    if (message.isHaveSnippet()) {
        auto clickedButton = ([=] {
            std::string msg = messageWidget->getInput();
            std::thread t(&ChatServer::runCompilation, &server_, std::ref(server_), std::ref(user_), std::ref(message), msg);
            t.detach();
        });

        messageWidget->setClickedRunButton(clickedButton);
    }

    messages_->addWidget(std::move(messageWidgetPtr));

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

    std::string username = ws2s(userLoginEdit_->text());
    std::string password = ws2s(passwordEdit_->text());
    std::string confirmPassword = ws2s(confirmPasswordEdit_->text());

    if (password != confirmPassword) {
        statusMsg_->setText("Password mismatch");
        return;
    }
    if (password.empty()) {
        statusMsg_->setText("Password field must not be empty");
        return;
    }

    user_ = User(username, password);

    if (server_.createUser(user_, getCurrentTimeMs())) {
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

    std::string username = ws2s(userLoginEdit_->text());
    std::string password = ws2s(passwordEdit_->text());

    user_ = User(username, password);

    if (server_.login(user_)) {
        Wt::WApplication::instance()->setCookie("userToken", user_.getToken(), 604800);

        connect(user_);

        loggedIn_ = true;
        dialogueList_ = server_.getDialogueList(user_, kCountLastDialogues);

        if (soundLogin_) {
            soundLogin_->play();
        }
        if (!soundMessageReceived_) {
            soundMessageReceived_ = std::make_unique<Wt::WSound>("resources/sounds/message_received.mp3");
        }

        startChat();
    } else {
        statusMsg_->setText("Incorrect login or password");
    }
}

void ChatWidget::searchUser() {
    blankDialoguePage();

    if (!userNameSearch_->text().empty()) {
        std::string findUser = ws2s(userNameSearch_->text());

        dialogues_->clear();

        User foundUser = server_.getUserByLogin(findUser);
        if (foundUser.empty()) {
            return;
        }

        Wt::WText *userWidget = dialogues_->addWidget(std::make_unique<Wt::WText>(foundUser.getLogin()));
        userWidget->setStyleClass("dialogue-block");
        userWidget->addStyleClass("dialogue-name");
        if (user_ == foundUser) {
            userWidget->addStyleClass("dialogue-name-self");
        }
        userWidget->setInline(false);

        userWidget->clicked().connect([&] {
            for (const auto &dialogue : dialogueList_) {
                if (dialogue.getName(user_) == foundUser.getLogin()) {
                    switchDialogue(dialogue);
                    endSearch();
                    return;
                }
            }

            std::vector<std::string> participantsList;
            participantsList.push_back(user_.getLogin());
            participantsList.push_back(foundUser.getLogin());

            Dialogue dialogue(participantsList, getCurrentTimeMs());

            server_.createDialogue(dialogue);
        });
     }
 }

 void ChatWidget::endSearch() {
     updateDialogueList();
 }

void ChatWidget::send() {
    if (currentDialogue_.isEmpty() ||
        (messageEdit_->text().empty() && currentSnippet_.empty())) {
        return;
    }

    Message message(currentDialogue_.getId(),
                    user_.getLogin(),
                    ws2s(messageEdit_->text()),
                    getCurrentTimeMs(),
                    currentSnippet_);

    currentSnippet_.clear();
    snippetButton_->removeStyleClass("bi-file-earmark-code-fill");
    snippetButton_->addStyleClass("bi-file-earmark-code");

    server_.sendMessage(currentDialogue_, message);
}

 void ChatWidget::editSnippet() {
     auto dialog = addChild(Wt::cpp14::make_unique<Wt::WDialog>("Write or copy the program"));

     auto snippetEdit = dialog->contents()->addNew<SnippetEditWidget>(currentSnippet_);

     auto save = dialog->footer()->addNew<Wt::WPushButton>("Save");
     save->setMargin(7, Wt::Side::Right);
     auto cancel = dialog->footer()->addNew<Wt::WPushButton>("Cancel");
     dialog->rejectWhenEscapePressed();

     save->clicked().connect(dialog, &Wt::WDialog::accept);
     cancel->clicked().connect(dialog, &Wt::WDialog::reject);

     dialog->finished().connect([=] {
         if (dialog->result() == Wt::DialogCode::Accepted) {
             currentSnippet_ = snippetEdit->getSnippet();
             if (!currentSnippet_.empty()) {
                 snippetButton_->removeStyleClass("bi-file-earmark-code");
                 snippetButton_->addStyleClass("bi-file-earmark-code-fill");
             }
         } else {
             currentSnippet_.clear();
             snippetButton_->removeStyleClass("bi-file-earmark-code-fill");
             snippetButton_->addStyleClass("bi-file-earmark-code");
         }

         removeChild(dialog);

         messageEdit_->setFocus();
     });

     dialog->show();
 }

 void ChatWidget::processChatEvent(const ChatEvent &event) {
     Wt::WApplication *app = Wt::WApplication::instance();

     app->triggerUpdate();

     switch (event.type()) {
         case ChatEvent::kNewDialogue: {
             dialogueList_.insert(event.dialogue());
             updateDialogueList();

             if (soundMessageReceived_) {
                 soundMessageReceived_->play();
             }
             break;
         }
         case ChatEvent::kNewMessage: {
             Dialogue updatedDialogue;
             for (const auto& dialogue : dialogueList_) {
                 if (dialogue == event.dialogue()) {
                     updatedDialogue = dialogue;
                     updatedDialogue.updateLastMessage(event.dialogue().getLastMessage());
                     dialogueList_.erase(dialogue); // TODO не уверен что удалится нужный
                     break;
                 }
             }
             dialogueList_.insert(updatedDialogue);
             updateDialogueList();

             if (currentDialogue_ == event.dialogue()) {
                 showNewMessage(event.dialogue().getLastMessage());
             }

             if (soundMessageReceived_) {
                 soundMessageReceived_->play();
             }

             break;
         }
         case ChatEvent::kCompilationCode: {
             /// TODO не протестировано
             if (event.getDialogueId() != currentDialogue_.getId()) {
                 break;
             }

             for (int i = 0; i < messages_->count(); ++i) {
                 auto messageWidget = dynamic_cast<MessageWidget*>(messages_->widget(i));
                 if (messageWidget->getMessageId() == event.getMessageId() && messageWidget->isHaveSnippet()) {
                     messageWidget->setResultCompilation(event.resultCompilation());
                     break;
                 }
             }

             break;
         }
         default:
             break;
     }
 }
