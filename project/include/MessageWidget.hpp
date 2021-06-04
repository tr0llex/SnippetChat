#ifndef SNIPPETCHAT_MESSAGEWIDGET_H
#define SNIPPETCHAT_MESSAGEWIDGET_H


#include <Wt/WContainerWidget.h>

#include "SnippetWidget.hpp"
#include "Models.hpp"


class MessageWidget : public Wt::WContainerWidget {
public:
    MessageWidget(const Message &message, bool myMassage);

    std::string getMessageId() const;

    bool isHaveSnippet() const;

    void setClickedRunButton(const std::function<void ()> &fn);

    std::string getInput() const;

    void setResultCompilation(const Compilation &result);

protected:
    void createLayout(
            std::unique_ptr<WWidget> text,
            std::unique_ptr<WWidget> time);

private:
    Message message_;
    bool myMessage_;

    Wt::WText   *text_;
    Wt::WText   *time_;
    SnippetWidget  *snippet_;
};


#endif //SNIPPETCHAT_MESSAGEWIDGET_H
