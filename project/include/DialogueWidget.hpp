#ifndef SNIPPETCHAT_DIALOGUEWIDGET_H
#define SNIPPETCHAT_DIALOGUEWIDGET_H

#include <Wt/WContainerWidget.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WText.h>

#include "Models.hpp"


class DialogueWidget : public Wt::WContainerWidget {
public:
    DialogueWidget(const std::string &dialogueName, const Dialogue &dialogue);

protected:
    void createLayout(
            std::unique_ptr<WWidget> dialogueName,
            std::unique_ptr<WWidget> lastMessage,
            std::unique_ptr<WWidget> time);

private:
    Dialogue     dialogue_;

    Wt::WText    *dialogueName_;
    Wt::WText    *lastMessage_;
    Wt::WText    *time_;
};


#endif //SNIPPETCHAT_DIALOGUEWIDGET_H
