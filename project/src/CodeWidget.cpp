#include "CodeWidget.hpp"

CodeWidget::CodeWidget(const Snippet &snippet, std::unique_ptr<Wt::WPushButton> runButtonPtr)
: snippet_(snippet) {
    setWidth(500);

    auto programTextPtr = std::make_unique<Wt::WPanel>();
    auto inputEditPtr = std::make_unique<Wt::WTextArea>();
    auto executionResultPtr = std::make_unique<Wt::WText>();

    programText_ = programTextPtr.get();
    inputEdit_ = inputEditPtr.get();
    executionResult_ = executionResultPtr.get();
    runButton_ = runButtonPtr.get();

    programText_->setTitle("Code\t" + snippet_.getLanguageStr());
    programText_->setCollapsible(true);

    Wt::WAnimation animation(Wt::AnimationEffect::SlideInFromTop,
                             Wt::TimingFunction::EaseOut,
                             100);

    programText_->setAnimation(animation);
    programText_->collapse();
    auto codeTextWidget = Wt::cpp14::make_unique<Wt::WText>(snippet_.getProgramText());
    codeTextWidget->setStyleClass("source-view");
    programText_->setCentralWidget(std::move(codeTextWidget));

    inputEdit_->setRows(3);

    createLayout(std::move(programTextPtr),
                 std::move(runButtonPtr),
                 std::move(inputEditPtr),
                 std::move(executionResultPtr));
}

void CodeWidget::createLayout(std::unique_ptr<WWidget> programText, std::unique_ptr<WWidget> runButton,
                              std::unique_ptr<WWidget> inputEdit, std::unique_ptr<WWidget> executionResult) {
    auto vLayout = std::make_unique<Wt::WVBoxLayout>();

    vLayout->addWidget(std::move(programText));
    vLayout->addWidget(std::move(inputEdit));
    vLayout->addWidget(std::move(runButton), 0, Wt::AlignmentFlag::Right);
    vLayout->addWidget(std::move(executionResult));

    this->setLayout(std::move(vLayout));
}

std::string CodeWidget::getInput() const {
    runButton_->disable();
    std::string input = ws2s(inputEdit_->text());
    return input;
}

void CodeWidget::setResultCompilation(const std::string &result) {
    runButton_->enable();
    executionResult_->setText(result);
}
