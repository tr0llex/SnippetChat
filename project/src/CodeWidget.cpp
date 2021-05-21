#include "CodeWidget.hpp"

CodeWidget::CodeWidget(const std::string &codeText, std::unique_ptr<WWidget> runButtonPtr)
: codeText_(codeText)/*, runButton_(runButton)*/ {
    setWidth(500);

    auto programTextPtr = std::make_unique<Wt::WPanel>();
//    auto runButtonPtr = std::make_unique<Wt::WPushButton>("Run");
    auto inputEditPtr = std::make_unique<Wt::WTextArea>();
    auto executionResultPtr = std::make_unique<Wt::WText>();

    programText_ = programTextPtr.get();
//    runButton_ = runButtonPtr.get();
    inputEdit_ = inputEditPtr.get();
    executionResult_ = executionResultPtr.get();

    programText_->setTitle("Code");
    programText_->setCollapsible(true);

    Wt::WAnimation animation(Wt::AnimationEffect::SlideInFromTop,
                             Wt::TimingFunction::EaseOut,
                             100);

    programText_->setAnimation(animation);
    programText_->collapse();
    auto codeTextWidget = Wt::cpp14::make_unique<Wt::WText>(codeText);
    codeTextWidget->setStyleClass("source-view");
    programText_->setCentralWidget(std::move(codeTextWidget));

    inputEdit_->setRows(3);

    createLayout(std::move(programTextPtr),
                 std::move(runButtonPtr),
                 std::move(inputEditPtr),
                 std::move(executionResultPtr));

    clearInput_.setJavaScript
            ("function(o, e) { setTimeout(function() {"
             "" + inputEdit_->jsRef() + ".value='';"
                                          "}, 0); }");

//    runButton_->clicked().connect(this, &CodeWidget::runCode);
//    runButton_->clicked().connect(clearInput_);
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

void CodeWidget::runCode() {
//    runButton_->disable();
}

std::string CodeWidget::getInput() const {
    std::string input = ws2s(inputEdit_->text());
    inputEdit_->setText("");
    return input;
}

void CodeWidget::setResultCompilation(const std::string &result) {
//    runButton_->enable();

    executionResult_->setText(result);
}
