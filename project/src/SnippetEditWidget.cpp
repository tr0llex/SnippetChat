#include <Wt/WDialog.h>
#include <Wt/WLabel.h>
#include <Wt/WTextArea.h>
#include <Wt/WPushButton.h>
#include <Wt/WComboBox.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>

#include "SnippetEditWidget.hpp"


static inline int convertLanguageToIndex(Snippet::Language language) {
    return language;
}

static inline Snippet::Language convertIndexToLanguage(int index) {
    return Snippet::Language(index);
}

SnippetEditWidget::SnippetEditWidget(const Snippet &snippet)
: snippet_(snippet) {
    auto labelPtr = std::make_unique<Wt::WLabel>("Language: ");
    auto switchLanguagePtr = std::make_unique<Wt::WComboBox>();
    auto editCodePtr = std::make_unique<Wt::WTextArea>(snippet.getProgramText());

    label_ = labelPtr.get();
    switchLanguage_ = switchLanguagePtr.get();
    editCode_ = editCodePtr.get();


    switchLanguage_->addItem("Python 3");
    switchLanguage_->addItem("C++ 17");
    switchLanguage_->addItem("C++ 20");
    switchLanguage_->addItem("C");
    switchLanguage_->setCurrentIndex(convertLanguageToIndex(snippet.getLanguage())); // TODO

    label_->setBuddy(switchLanguage_);

    editCode_->setRows(30);
    editCode_->setWidth(700);


    createLayout(std::move(labelPtr),
                 std::move(switchLanguagePtr),
                 std::move(editCodePtr));

    switchLanguage_->changed().connect(this, &SnippetEditWidget::updateLanguage);
}

void SnippetEditWidget::createLayout(std::unique_ptr<WWidget> label,
                                     std::unique_ptr<WWidget> switchLanguage,
                                     std::unique_ptr<WWidget> editCode) {
    auto vLayout = std::make_unique<Wt::WVBoxLayout>();

    auto hLayout = std::make_unique<Wt::WHBoxLayout>();
    hLayout->addWidget(std::move(label));
    hLayout->addWidget(std::move(switchLanguage));
    vLayout->addLayout(std::move(hLayout), 0, Wt::AlignmentFlag::Right);

    vLayout->addWidget(std::move(editCode));

    this->setLayout(std::move(vLayout));
}

Snippet SnippetEditWidget::getSnippet() {
    snippet_.setProgramText(ws2s(editCode_->text()));
    return snippet_;
}

void SnippetEditWidget::updateLanguage() {
    snippet_.setLanguage(convertIndexToLanguage(switchLanguage_->currentIndex()));
}

void showSnippetDialog(Wt::WObject *owner, Snippet *snippet) {
    auto dialog = owner->addChild(Wt::cpp14::make_unique<Wt::WDialog>("Write or copy the program"));

    /*auto vLayout = std::make_unique<Wt::WVBoxLayout>();

    auto hLayout = std::make_unique<Wt::WHBoxLayout>();

    auto labelPtr = std::make_unique<Wt::WLabel>("Language: ");
    auto label = labelPtr.get();

    auto switchLanguagePtr = std::make_unique<Wt::WComboBox>();
    auto switchLanguage = switchLanguagePtr.get();

    switchLanguage->addItem("Python 3");
    switchLanguage->addItem("C++ 17");
    switchLanguage->addItem("C++ 20");
    switchLanguage->addItem("C");

    label->setBuddy(switchLanguage);

    hLayout->addWidget(std::move(labelPtr));
    hLayout->addWidget(std::move(switchLanguagePtr));

    vLayout->addLayout(std::move(hLayout), 0, Wt::AlignmentFlag::Right);

    switchLanguage->changed().connect([=] {
        std::string selectedLanguage = ws2s(switchLanguage->currentText());

        if (selectedLanguage == "Python 3") {
            snippet->setLanguage(Snippet::Python_3);
            return ;
        }
        if (selectedLanguage == "C++ 17") {
            snippet->setLanguage(Snippet::Cpp_17);
            return ;
        }
        if (selectedLanguage == "C++ 20") {
            snippet->setLanguage(Snippet::Cpp_20);
            return ;
        }
        if (selectedLanguage == "C") {
            snippet->setLanguage(Snippet::C_98);
            return ;
        }
    });


    auto editProgramText = std::make_unique<Wt::WTextArea>(snippet->getProgramText());
    auto editProgramTextPtr = editProgramText.get();

    editProgramText->setRows(30);
    editProgramText->setWidth(700);

    vLayout->addWidget(std::move(editProgramText));*/


    auto snippetEdit = dialog->contents()->addNew<SnippetEditWidget>(*snippet);


    auto save = dialog->footer()->addNew<Wt::WPushButton>("Save");

    auto cancel = dialog->footer()->addNew<Wt::WPushButton>("Cancel");
    dialog->rejectWhenEscapePressed();


    save->clicked().connect(dialog, &Wt::WDialog::accept);

    cancel->clicked().connect(dialog, &Wt::WDialog::reject);

    dialog->finished().connect([=] {
        if (dialog->result() == Wt::DialogCode::Accepted) {
            *snippet = snippetEdit->getSnippet();
        } else {
            snippet->clear();
        }

        owner->removeChild(dialog);
    });

    dialog->show();
}
