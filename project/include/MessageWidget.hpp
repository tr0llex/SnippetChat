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

#include "VovaModels.hpp"
#include "CodeWidget.hpp"

class MessageWidget : public Wt::WContainerWidget {
public:
    explicit MessageWidget(const Message &message);

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

};


#endif //SNIPPETCHAT_MESSAGEWIDGET_H
