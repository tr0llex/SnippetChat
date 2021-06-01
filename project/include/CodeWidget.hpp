#ifndef SNIPPETCHAT_CODEWIDGET_H
#define SNIPPETCHAT_CODEWIDGET_H

//#include <pthread.h>

#include <Wt/WContainerWidget.h>
#include <Wt/WPanel.h>
#include <Wt/WTextArea.h>

#include "Compilation.hpp"
#include "Models.hpp"


class CodeWidget : public Wt::WContainerWidget {
public:
    explicit CodeWidget(const Snippet &snippet);

    void setClickedRunButton(const std::function<void()> &fn);

    std::string getInput() const;

    void setResultCompilation(const Compilation &result);

protected:
    void createLayout(
            std::unique_ptr<WWidget> programText,
            std::unique_ptr<WWidget> runButton,
            std::unique_ptr<WWidget> stdinEdit,
            std::unique_ptr<WWidget> resultContainer);

    void createResultLayout(
            const Compilation &result,
            std::unique_ptr<WWidget> timeLimitExceeded,
            std::unique_ptr<WWidget> compilerStderr,
            std::unique_ptr<WWidget> metricTable,
            std::unique_ptr<WWidget> compilerStdout,
            std::unique_ptr<WWidget> executionStderr);

private:
    Snippet   snippet_;

    Wt::WPanel               *snippetPanel_;
    Wt::WTextArea            *inputEdit_;
    Wt::WPushButton          *runButton_;
    Wt::WContainerWidget     *resultContainer_;
};


#endif //SNIPPETCHAT_CODEWIDGET_H
