#ifndef SNIPPETCHAT_CODEWIDGET_H
#define SNIPPETCHAT_CODEWIDGET_H

#include <Wt/WAnimation.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WPanel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>

#include "VovaModels.hpp"
#include "ChatServer.hpp"


class CodeWidget : public Wt::WContainerWidget {
public:
    CodeWidget(ChatServer& server, const Message &message);

    void setResultCompilation(const Wt::WString &result);

protected:
    void createLayout(
            std::unique_ptr<WWidget> programText,
            std::unique_ptr<WWidget> runButton,
            std::unique_ptr<WWidget> stdinEdit,
            std::unique_ptr<WWidget> executionResult);

private:
    void runCode();

private:
    ChatServer    &server_;
//    User          &user_; TODO

    Message    message_;

    Wt::WPanel                               *programText_;
    Wt::Core::observing_ptr<Wt::WPushButton>  runButton_;
    Wt::WTextArea                            *stdinEdit_;
    Wt::WText                                *executionResult_;
    Wt::JSlot                                 clearInput_;

};


#endif //SNIPPETCHAT_CODEWIDGET_H
