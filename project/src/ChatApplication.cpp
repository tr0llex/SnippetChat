#include "ChatApplication.hpp"

ChatApplication::ChatApplication(const Wt::WEnvironment& env,
                                 ChatServer& server)
        : WApplication(env),
          server_(server),
          env_(env) {
    setTitle("SnippetChat");
    useStyleSheet("resources/chatapp.css");

    messageResourceBundle().use(appRoot() + "resources/SnippetChat");

    ChatWidget *chatWidget =
            root()->addWidget(std::make_unique<ChatWidget>(server_));
    chatWidget->setStyleClass("chat");
}
