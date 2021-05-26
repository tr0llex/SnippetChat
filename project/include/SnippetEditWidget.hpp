#ifndef SNIPPETCHAT_SNIPPETEDITWIDGET_H
#define SNIPPETCHAT_SNIPPETEDITWIDGET_H


#include "Models.hpp"


class SnippetEditWidget : public Wt::WContainerWidget {
public:
    explicit SnippetEditWidget(const Snippet &snippet);

    Snippet getSnippet();

protected:
    void createLayout(
            std::unique_ptr<WWidget> label,
            std::unique_ptr<WWidget> switchLanguage,
            std::unique_ptr<WWidget> editCode);

private:
    void updateLanguage();

private:
    Snippet         snippet_;

    Wt::WLabel      *label_;
    Wt::WComboBox   *switchLanguage_;
    Wt::WTextArea   *editCode_;
};


void showSnippetDialog(Wt::WObject *owner, Snippet *snippet);


#endif //SNIPPETCHAT_SNIPPETEDITWIDGET_H
