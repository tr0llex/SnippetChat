#include <Wt/WPushButton.h>
#include <Wt/WText.h>

#include "ChatApplication.hpp"

ChatApplication::ChatApplication(const Wt::WEnvironment& env,
                                 ChatServer& server)
        : WApplication(env),
          server_(server),
          env_(env) {
    setTitle("SnippetChat");
    useStyleSheet("resources/chatapp.css");

    messageResourceBundle().use(appRoot() + "resources/SnippetChat");

    root()->addWidget(std::make_unique<Wt::WText>(Wt::WString::tr("introduction")));

    ChatWidget *chatWidget =
            root()->addWidget(std::make_unique<ChatWidget>(server_));
    chatWidget->setStyleClass("chat");
}
