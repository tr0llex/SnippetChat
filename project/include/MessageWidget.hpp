#ifndef SNIPPETCHAT_MESSAGEWIDGET_H
#define SNIPPETCHAT_MESSAGEWIDGET_H


#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WPanel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>

#include "Models.hpp"
#include "CodeWidget.hpp"

class MessageWidget : public Wt::WContainerWidget {
public:
    explicit MessageWidget(const Message &message);

    std::string getMessageId() const;

    bool isHaveSnippet() const;

    void setSnippet(std::unique_ptr<CodeWidget> snippetPtr);

    void setResultCompilation(const std::string &result);

protected:
    void createLayout(
            std::unique_ptr<WWidget> text,
            std::unique_ptr<WWidget> time);

private:
    Message message_;

    Wt::WText   *text_;
    Wt::WText   *time_;
    CodeWidget  *snippet_;
};


#endif //SNIPPETCHAT_MESSAGEWIDGET_H
