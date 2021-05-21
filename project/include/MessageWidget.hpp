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
    MessageWidget(const Message &message, std::unique_ptr<WWidget> runButtonPtr);

    std::string getMessageId() const;

    bool isHaveCode() const;

    std::string getInput() const;

    void setResultCompilation(const std::string &result);

protected:
    void createLayout(
            std::unique_ptr<WWidget> text,
            std::unique_ptr<WWidget> time,
            std::unique_ptr<WWidget> code);

private:
    Message message_;

    Wt::WText   *text_;
    Wt::WText   *time_;
    CodeWidget  *code_;
/// TODO
// прокидываем в код кнопку, которую будем создавать в чатвиджете
};


#endif //SNIPPETCHAT_MESSAGEWIDGET_H