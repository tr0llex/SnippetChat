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
#include <pthread.h>

#include "Models.hpp"


class CodeWidget : public Wt::WContainerWidget {
public:
    CodeWidget(const Snippet &snippet, std::unique_ptr<Wt::WPushButton> runButtonPtr);

    std::string getInput() const;

    void setResultCompilation(const std::string &result);

protected:
    void createLayout(
            std::unique_ptr<WWidget> programText,
            std::unique_ptr<WWidget> runButton,
            std::unique_ptr<WWidget> stdinEdit,
            std::unique_ptr<WWidget> executionResult);

private:
    Snippet   snippet_;

    Wt::WPanel                               *programText_;
    Wt::WTextArea                            *inputEdit_;
    Wt::WText                                *executionResult_;
    Wt::WPushButton                          *runButton_;
};


#endif //SNIPPETCHAT_CODEWIDGET_H
