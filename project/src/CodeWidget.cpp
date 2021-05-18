#include "CodeWidget.hpp"

CodeWidget::CodeWidget(ChatServer& server, const Message &message)
: server_(server), message_(message) {
    setWidth(500);

    auto programTextPtr = std::make_unique<Wt::WPanel>();
    auto runButtonPtr = std::make_unique<Wt::WPushButton>("Run");
    auto stdinEditPtr = std::make_unique<Wt::WTextArea>();
    auto executionResultPtr = std::make_unique<Wt::WText>();

    programText_ = programTextPtr.get();
    runButton_ = runButtonPtr.get();
    stdinEdit_ = stdinEditPtr.get();
    executionResult_ = executionResultPtr.get();

    programText_->setTitle("Code");
    programText_->setCollapsible(true);

    Wt::WAnimation animation(Wt::AnimationEffect::SlideInFromTop,
                             Wt::TimingFunction::EaseOut,
                             100);

    programText_->setAnimation(animation);
    programText_->collapse();
    programText_->setCentralWidget(Wt::cpp14::make_unique<Wt::WText>(message_.getMessageCode()));

    stdinEdit_->setRows(3);

    createLayout(std::move(programTextPtr),
                 std::move(runButtonPtr),
                 std::move(stdinEditPtr),
                 std::move(executionResultPtr));

    clearInput_.setJavaScript
            ("function(o, e) { setTimeout(function() {"
             "" + stdinEdit_->jsRef() + ".value='';"
                                          "}, 0); }");

    runButton_->clicked().connect(this, &CodeWidget::runCode);
    runButton_->clicked().connect(clearInput_);
}

void CodeWidget::createLayout(std::unique_ptr<WWidget> programText, std::unique_ptr<WWidget> runButton,
                              std::unique_ptr<WWidget> stdinEdit, std::unique_ptr<WWidget> executionResult) {
    auto vLayout = std::make_unique<Wt::WVBoxLayout>();

    vLayout->addWidget(std::move(programText));
    vLayout->addWidget(std::move(stdinEdit));
    vLayout->addWidget(std::move(runButton), 0, Wt::AlignmentFlag::Right);
    vLayout->addWidget(std::move(executionResult));

    this->setLayout(std::move(vLayout));
}

void CodeWidget::runCode() {
//    server_.runCompilation(message_, stdinEdit_->text());

    runButton_->disable();
}

void CodeWidget::setResultCompilation(const Wt::WString &result) {
    runButton_->enable();

    executionResult_->setText(result);
}

/// короче бля надо хранить в чатвиджете переменную индекс сообщения
/// - если она -1 можем запускать код
/// - когда запускаем, записываем в нее индекс сообщения
/// херня получается .........

