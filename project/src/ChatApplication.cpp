#include "ChatApplication.hpp"

ChatApplication::ChatApplication(const Wt::WEnvironment& env,
                                 ChatServer& server)
        : WApplication(env),
          server_(server),
          env_(env) {
    setTitle("SnippetChat");
    useStyleSheet("resources/chatapp.css");
    useStyleSheet("resources/highlight/styles/default.css");
    require("resources/highlight/highlight.pack.js");

    messageResourceBundle().use(appRoot() + "resources/SnippetChat");

    ChatWidget *chatWidget =
            root()->addWidget(std::make_unique<ChatWidget>(server_));
    chatWidget->setStyleClass("chat");

//    doJavaScript("<script src=\"resources/highlight/highlight.pack.js\"></script>");
//    doJavaScript("<script src=\"resources/highlight/highlight.pack.js\"></script>");
//    appRoot().j
}
