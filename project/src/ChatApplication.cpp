#include "ChatApplication.hpp"

#include <Wt/WPushButton.h>
#include <Wt/WText.h>

ChatApplication::ChatApplication(const Wt::WEnvironment& env,
                                 ChatServer& server)
        : WApplication(env),
          server_(server),
          env_(env) {
//    setTitle("SniCh");
//    useStyleSheet("chatapp.css");
//
//    messageResourceBundle().use(appRoot() + "simplechat");
//
//    root()->addWidget(std::make_unique<Wt::WText>(Wt::WString::tr("introduction")));
//
//    ChatWidget *chatWidget =
//            root()->addWidget(std::make_unique<ChatWidget>(server_));
//    chatWidget->setStyleClass("chat");
}
