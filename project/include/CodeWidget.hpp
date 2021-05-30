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
#include "Compilation.hpp"


class CodeWidget : public Wt::WContainerWidget {
public:
    CodeWidget(const Snippet &snippet);

    void setClickedRunButton(const std::function<void()> &fn);

    std::string getInput() const;

    void setResultCompilation(const Compilation &result);

protected:
    void createLayout(
            std::unique_ptr<WWidget> programText,
            std::unique_ptr<WWidget> runButton,
            std::unique_ptr<WWidget> stdinEdit,
            std::unique_ptr<WWidget> resultContainer);

private:
    Snippet   snippet_;

    Wt::WPanel                               *programText_;
    Wt::WTextArea                            *inputEdit_;
    Wt::WPushButton                          *runButton_;
    Wt::WContainerWidget                     *resultContainer_;
};


#endif //SNIPPETCHAT_CODEWIDGET_H
