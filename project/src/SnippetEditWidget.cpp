#include <Wt/WApplication.h>
#include <Wt/WDialog.h>
#include <Wt/WEnvironment.h>
#include <Wt/WLabel.h>
#include <Wt/WTextArea.h>
#include <Wt/WPushButton.h>
#include <Wt/WRegExpValidator.h>

#include "SnippetEditWidget.hpp"

SnippetEditWidget::SnippetEditWidget(Wt::WObject *owner, Snippet &snippet)
: snippet_(snippet) {
    dialog_ = owner->addChild(Wt::cpp14::make_unique<Wt::WDialog>("Go to cell"));

    auto *label = dialog_->contents()->addNew<Wt::WLabel>("write or copy the program");

    editProgramText_ = dialog_->contents()->addNew<Wt::WTextArea>(snippet.getProgramText());

    editProgramText_->setRows(30);
    editProgramText_->setWidth(700);
    label->setBuddy(editProgramText_);
    label->setInline(false);

    auto *save = dialog_->footer()->addNew<Wt::WPushButton>("Save");
//    ok->setDefault(true);
//    if (wApp->environment().ajax()) {
//        ok->disable();
//    }

    Wt::WPushButton *cancel = dialog_->footer()->addNew<Wt::WPushButton>("Cancel");
    dialog_->rejectWhenEscapePressed();


    save->clicked().connect(this, &SnippetEditWidget::saveSnippet);

    cancel->clicked().connect([&] {
//        editProgramText_->setText("");
        dialog_->reject();
    });

    dialog_->finished().connect([&] {
        std::cout << "-- close dialogue --" << std::endl;

        owner->removeChild(dialog_);
    });

    dialog_->show();
}

void SnippetEditWidget::saveSnippet() {
    if (!editProgramText_->text().empty()) {
        std::wstring wedit = editProgramText_->text();
        snippet_.setProgramText(ws2s(wedit));
        dialog_->accept();
    }
}

void showSnippetDialog(Wt::WObject *owner, Snippet *snippet) {
    auto dialog = owner->addChild(Wt::cpp14::make_unique<Wt::WDialog>("Go to cell"));

    Wt::WLabel *label =
            dialog->contents()->addNew<Wt::WLabel>("Cell location (A1..Z999)");
    Wt::WTextArea *editProgramText =
            dialog->contents()->addNew<Wt::WTextArea>(snippet->getProgramText());
    editProgramText->setRows(30);
    editProgramText->setWidth(700);
    label->setBuddy(editProgramText);
    label->setInline(false);

    Wt::WPushButton *save =
            dialog->footer()->addNew<Wt::WPushButton>("OK");

    Wt::WPushButton *cancel =
            dialog->footer()->addNew<Wt::WPushButton>("Cancel");
    dialog->rejectWhenEscapePressed();

    /*
     * Accept the dialog
     */
    save->clicked().connect(dialog, &Wt::WDialog::accept);

    /*
     * Reject the dialog
     */
    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    /*
     * Process the dialog result.
     */
    dialog->finished().connect([=] {
        if (dialog->result() == Wt::DialogCode::Accepted) {
            snippet->setProgramText(ws2s(editProgramText->text()));
        }
        else {
            snippet->clear();
        }

        owner->removeChild(dialog);
    });

    dialog->show();
}
