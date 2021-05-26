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


static inline int64_t getTimeMs() {
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
            dialogueList_ = server_.getDialogueList(user_);

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


    auto hLayout = std::make_unique<Wt::WHBoxLayout>();

    hLayout->addWidget(std::make_unique<Wt::WLabel>("Login:"), 1);

    userLoginEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>(), 1);
    userLoginEdit_->setFocus();

    vLayout->addLayout(std::move(hLayout), 0, Wt::AlignmentFlag::Center);


    hLayout = std::make_unique<Wt::WHBoxLayout>();

    hLayout->addWidget(std::make_unique<Wt::WLabel>("Password:"), 1);

    passwordEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>(), 1);
    passwordEdit_->setAttributeValue("type", "password");

    vLayout->addLayout(std::move(hLayout), 0, Wt::AlignmentFlag::Center);


    hLayout = std::make_unique<Wt::WHBoxLayout>();

    hLayout->addWidget(std::make_unique<Wt::WLabel>("Confirm password:"), 1);

    confirmPasswordEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>(), 1);
    confirmPasswordEdit_->setAttributeValue("type", "password");

    vLayout->addLayout(std::move(hLayout), 0, Wt::AlignmentFlag::Center);


    auto signUp = vLayout->addWidget(std::make_unique<Wt::WPushButton>("Sign up"), 0, Wt::AlignmentFlag::Center);

    signUp->clicked().connect(this, &ChatWidget::signUp);
    userLoginEdit_->enterPressed().connect(this, [&] {
        passwordEdit_->setFocus();
    });
    passwordEdit_->enterPressed().connect(this, [&] {
        confirmPasswordEdit_->setFocus();
    });
    confirmPasswordEdit_->enterPressed().connect(this, &ChatWidget::signUp);


    auto logIn = vLayout->addWidget(std::make_unique<Wt::WText>("Log in"), 0, Wt::AlignmentFlag::Center);
    logIn->setStyleClass("text-link");
    logIn->clicked().connect(this, &ChatWidget::letLogin);


    statusMsg_ = vLayout->addWidget(std::make_unique<Wt::WText>(), 1, Wt::AlignmentFlag::Center);
    statusMsg_->setTextFormat(Wt::TextFormat::Plain);
}

void ChatWidget::letLogin() {
    clear();

    auto vLayout = setLayout(std::make_unique<Wt::WVBoxLayout>());


    auto hLayout = std::make_unique<Wt::WHBoxLayout>();

    hLayout->addWidget(std::make_unique<Wt::WLabel>("Login:"), 1);

    userLoginEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>(), 1);
    userLoginEdit_->setFocus();

    vLayout->addLayout(std::move(hLayout), 0, Wt::AlignmentFlag::Center);


    hLayout = std::make_unique<Wt::WHBoxLayout>();

    hLayout->addWidget(std::make_unique<Wt::WLabel>("Password:"), 1);

    passwordEdit_ = hLayout->addWidget(std::make_unique<Wt::WLineEdit>(), 1);
    passwordEdit_->setAttributeValue("type", "password");

    vLayout->addLayout(std::move(hLayout), 0, Wt::AlignmentFlag::Center);


    auto logIn = vLayout->addWidget(std::make_unique<Wt::WPushButton>("Log in"), 0, Wt::AlignmentFlag::Center);

    logIn->clicked().connect(this, &ChatWidget::login);
    userLoginEdit_->enterPressed().connect(this, [&] {
        passwordEdit_->setFocus();
    });
    passwordEdit_->enterPressed().connect(this, &ChatWidget::login);


    auto signUp = vLayout->addWidget(std::make_unique<Wt::WText>("Sign up"), 0, Wt::AlignmentFlag::Center);
    signUp->setStyleClass("text-link");
    signUp->clicked().connect(this, &ChatWidget::letSignUp);


    statusMsg_ = vLayout->addWidget(std::make_unique<Wt::WText>(), 1, Wt::AlignmentFlag::Center);
    statusMsg_->setTextFormat(Wt::TextFormat::Plain);
}

void ChatWidget::startChat() {
    clear();

    userLoginEdit_ = nullptr;

    auto dialogueNamePtr = std::make_unique<Wt::WText>();
    auto userNameSearchPtr = std::make_unique<Wt::WLineEdit>();
    auto searchButtonPtr = std::make_unique<Wt::WPushButton>("find");
    auto backButtonPtr = std::make_unique<Wt::WPushButton>("back");
    auto snippetButtonPtr = std::make_unique<Wt::WPushButton>("</>");
    auto messagesPtr = std::make_unique<WContainerWidget>();
    auto userListPtr = std::make_unique<WContainerWidget>();
    auto messageEditPtr = std::make_unique<Wt::WTextArea>();
    auto sendButtonPtr = std::make_unique<Wt::WPushButton>("Send");
    auto logoutButtonPtr = std::make_unique<Wt::WPushButton>("Logout");

    dialogueName_ = dialogueNamePtr.get();
    userNameSearch_ = userNameSearchPtr.get();
    searchButton_ = searchButtonPtr.get();
    backButton_ = backButtonPtr.get();
    snippetButton_ = snippetButtonPtr.get();
    messages_ = messagesPtr.get();
    dialogues_ = userListPtr.get();
    messageEdit_ = messageEditPtr.get();
    sendButton_ = sendButtonPtr.get();
    Wt::Core::observing_ptr<Wt::WPushButton> logoutButton = logoutButtonPtr.get();

    dialogueName_->setStyleClass("chat-dialogue-name");

    messageEdit_->setRows(2);

    messages_->setOverflow(Wt::Overflow::Auto);
    dialogues_->setOverflow(Wt::Overflow::Auto);

    createMessengerLayout(std::move(dialogueNamePtr),
                          std::move(userNameSearchPtr),
                          std::move(searchButtonPtr),
                          std::move(backButtonPtr),
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

    searchButton_->clicked().connect(this, &ChatWidget::searchUser);
    searchButton_->clicked().connect(clearSearchInput_);

    userNameSearch_->enterPressed().connect(this, &ChatWidget::searchUser);
    userNameSearch_->enterPressed().connect(clearSearchInput_);

    backButton_->clicked().connect(this, &ChatWidget::back);
    backButton_->clicked().connect(clearSearchInput_);

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

    std::vector<Message> messages = server_.getMessagesFromDialogue(dialogue.getId());
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

    hLayout->setPreferredImplementation(Wt::LayoutImplementation::JavaScript);

    /// <Шапка>
    hLayout->addWidget(std::make_unique<Wt::WText>(tr("projectName")), 1);
    hLayout->addWidget(std::move(logoutButton));
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
    messageEdit->setStyleClass("chat-noedit");
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

    auto runButtonPtr = std::make_unique<Wt::WPushButton>("Run");
    ButtonPtr runButton = runButtonPtr.get();
//    if (message.isHaveCode()) {
//        runCodeButtons_.push_back(runButton);
//    }

    auto messageWidget = std::make_unique<MessageWidget>(message, std::move(runButtonPtr));
    MessageWidget *w = messages_->addWidget(std::move(messageWidget));

    if (message.isHaveCode()) {
        runButton->clicked().connect([=] {
//            runButton->disable();

            server_.runCompilation(user_, message, w->getInput());
        });
    }

    w->setInline(false);
    w->setStyleClass("chat-msg");

    if (message.getSenderLogin() == user_.getLogin()) {
        w->addStyleClass("msg-right");
    }

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

    if (server_.signUp(user_, getTimeMs())) {
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
        dialogueList_ = server_.getDialogueList(user_);

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

        foundUsers_ = server_.getUsersByUserName(findUser);

        for (const auto& foundUser : foundUsers_) {
            Wt::WText *w = dialogues_->addWidget(std::make_unique<Wt::WText>(foundUser.getLogin()));
            w->setStyleClass("dialogue-block");
            w->addStyleClass("dialogue-name");
            if (user_ == foundUser) {
                w->addStyleClass("dialogue-name-self");
            }

            w->setInline(false);

            w->clicked().connect([&] {
                for (const auto &dialogue : dialogueList_) {
                    if (dialogue.getName(user_) == foundUser.getLogin()) {
                        switchDialogue(dialogue);
                        updateDialogueList();
                        return;
                    }
                }

                std::vector<std::string> participantsList;
                participantsList.push_back(user_.getLogin());
                participantsList.push_back(foundUser.getLogin());

                Dialogue dialogue(participantsList, getTimeMs());

                server_.createDialogue(dialogue);
            });
        }

        foundUsers_.clear(); // TODO no test
    }
}

void ChatWidget::back() {
    updateDialogueList();
}

void ChatWidget::send() {
    if (currentDialogue_.isEmpty()) {
        return;
    }

    if (!messageEdit_->text().empty() || !currentSnippet_.empty()) {
        Message message(currentDialogue_.getId(),
                        user_.getLogin(),
                        ws2s(messageEdit_->text()),
                        getTimeMs(),
                        currentSnippet_);

        currentSnippet_.clear();
        snippetButton_->removeStyleClass("attached-code");

        server_.sendMessage(currentDialogue_, message);
    }
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
                snippetButton_->addStyleClass("attached-code");
            }
        } else {
            currentSnippet_.clear();
            snippetButton_->removeStyleClass("attached-code");
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
        case ChatEvent::Login: {
            updateDialogueList();
            break;
        }
        case ChatEvent::Logout: {
            updateDialogueList();
            break;
        }
        case ChatEvent::NewDialogue: {
            dialogueList_.insert(event.dialogue());
            updateDialogueList();

            if (soundMessageReceived_) {
                soundMessageReceived_->play();
            }
            break;
        }
        case ChatEvent::NewMessage: {
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
        case ChatEvent::CompilationCode: {
            /// TODO не протестировано
            if (event.getDialogueId() != currentDialogue_.getId()) {
                break;
            }

            for (int i = 0; i < messages_->count(); ++i) {
                auto messageWidget = dynamic_cast<MessageWidget*>(messages_->widget(i));
                if (messageWidget->getMessageId() == event.getMessageId() && messageWidget->isHaveCode()) {
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
