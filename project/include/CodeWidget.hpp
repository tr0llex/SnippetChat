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

#include "Models.hpp"
#include "ChatServer.hpp"


class CodeWidget : public Wt::WContainerWidget {
public:
    CodeWidget(const std::string &codeText, std::unique_ptr<WWidget> runButtonPtr);

    std::string getInput() const;

    void setResultCompilation(const std::string &result);

protected:
    void createLayout(
            std::unique_ptr<WWidget> programText,
            std::unique_ptr<WWidget> runButton,
            std::unique_ptr<WWidget> stdinEdit,
            std::unique_ptr<WWidget> executionResult);

private:
    void runCode();

private:
    std::string   codeText_;

    Wt::WPanel                               *programText_;
//    Wt::Core::observing_ptr<Wt::WPushButton>  runButton_;
    Wt::WTextArea                            *inputEdit_;
    Wt::WText                                *executionResult_;
    Wt::JSlot                                 clearInput_;

};


#endif //SNIPPETCHAT_CODEWIDGET_H
