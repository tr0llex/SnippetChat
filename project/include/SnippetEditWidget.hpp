#ifndef SNIPPETCHAT_SNIPPETEDITWIDGET_H
#define SNIPPETCHAT_SNIPPETEDITWIDGET_H


#include <Wt/WContainerWidget.h>

#include "Models.hpp"

class SnippetEditWidget : public Wt::WContainerWidget {
public:
    SnippetEditWidget(Wt::WObject *owner, Snippet &snippet);

private:
    void saveSnippet();

private:
    Snippet &snippet_;

    Wt::WDialog   *dialog_;
    Wt::WTextArea *editProgramText_;
};

// TODO
void showSnippetDialog(Wt::WObject *owner, Snippet *snippet);


#endif //SNIPPETCHAT_SNIPPETEDITWIDGET_H
