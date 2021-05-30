#include "CodeWidget.hpp"
#include <Wt/WTable.h>

static inline std::string langToStyleClass(const Snippet &snippet) {
    switch (snippet.getLanguage()) {
        case Snippet::Python_3:
            return "python";
        case Snippet::Cpp_14:
        case Snippet::Cpp_20:
            return "cpp";
        case Snippet::C_98:
            return "c";
        default:
            return std::string();
    }
}

CodeWidget::CodeWidget(const Snippet &snippet)
: snippet_(snippet) {
    setWidth(580);

    auto programTextPtr = std::make_unique<Wt::WPanel>();
    auto inputEditPtr = std::make_unique<Wt::WTextArea>();
    auto runButtonPtr = std::make_unique<Wt::WPushButton>("Run");
    auto resultContainerPtr = std::make_unique<Wt::WContainerWidget>();

    programText_ = programTextPtr.get();
    inputEdit_ = inputEditPtr.get();
    runButton_ = runButtonPtr.get();
    resultContainer_ = resultContainerPtr.get();

    programText_->setTitle("Code\t" + snippet_.getLanguageStr());
    programText_->setCollapsible(true);

    Wt::WAnimation animation(Wt::AnimationEffect::SlideInFromTop,
                             Wt::TimingFunction::EaseOut,
                             100);

    programText_->setAnimation(animation);
    programText_->collapse();

    programText_->setMargin(0);

    auto container = std::make_unique<Wt::WContainerWidget>();
    container->setHtmlTagName("pre");
    container->setStyleClass("margin-0");

    auto codeTextWidget = Wt::cpp14::make_unique<Wt::WText>(snippet_.getProgramText());
    codeTextWidget->setHtmlTagName("code class=" + langToStyleClass(snippet_));
    container->addWidget(std::move(codeTextWidget));

    programText_->setCentralWidget(std::move(container));

    programText_->centralWidget()->parent()->setStyleClass("padding-0");

    inputEdit_->setRows(3);
    inputEdit_->setPlaceholderText("input data");

    createLayout(std::move(programTextPtr),
                 std::move(runButtonPtr),
                 std::move(inputEditPtr),
                 std::move(resultContainerPtr));
}

void CodeWidget::createLayout(std::unique_ptr<WWidget> programText, std::unique_ptr<WWidget> runButton,
                              std::unique_ptr<WWidget> inputEdit, std::unique_ptr<WWidget> resultContainer) {
    auto vLayout = std::make_unique<Wt::WVBoxLayout>();

    vLayout->addWidget(std::move(programText));
    vLayout->addWidget(std::move(inputEdit));
    vLayout->addWidget(std::move(runButton), 0, Wt::AlignmentFlag::Right);
    vLayout->addWidget(std::move(resultContainer));

    this->setLayout(std::move(vLayout));
}

void CodeWidget::setClickedRunButton(const std::function<void()> &fn) {
    runButton_->clicked().connect(fn);
}

std::string CodeWidget::getInput() const {
    runButton_->disable();
    resultContainer_->clear();

    std::string input = ws2s(inputEdit_->text());

    return input;
}

void CodeWidget::setResultCompilation(const Compilation &result) {
    runButton_->enable();

    auto vLayout = resultContainer_->setLayout(std::make_unique<Wt::WVBoxLayout>());

    resultContainer_->setStyleClass("source-view");

    if (result.getTimeLimitExceeded()) {
        auto timeLimitExceeded = std::make_unique<Wt::WText>("Time limit (5 sec) exceeded!");
        vLayout->addWidget(std::move(timeLimitExceeded));
    }

    if (!result.getCompilerStderr().empty()) {
        auto compilerStderrPtr = std::make_unique<Wt::WText>(result.getCompilerStderr(), Wt::TextFormat::Plain);
        vLayout->addWidget(std::move(compilerStderrPtr));
    } else {
        if (!result.getTimeLimitExceeded()) {
            auto executionTimePtr = std::make_unique<Wt::WText>(result.getExecutionTime());
            auto executionUsedMemoryPtr = std::make_unique<Wt::WText>(result.getExecutionUsedMemory());

            auto table = std::make_unique<Wt::WTable>();
            table->setHeaderCount(1);

            table->elementAt(0, 0)->addNew<Wt::WText>("Time");
            table->elementAt(0, 1)->addNew<Wt::WText>("Memory");
            table->elementAt(1, 0)->addWidget(std::move(executionTimePtr));
            table->elementAt(1, 1)->addWidget(std::move(executionUsedMemoryPtr));

            vLayout->addWidget(std::move(table), 0, Wt::AlignmentFlag::Super);
        }

        auto compilerStdoutPtr = std::make_unique<Wt::WText>(result.getExecutionStdout(), Wt::TextFormat::Plain);
        vLayout->addWidget(std::move(compilerStdoutPtr));

        if (!result.getExecutionStderr().empty()) {
            auto executionStderrPtr = std::make_unique<Wt::WText>(result.getExecutionStderr(), Wt::TextFormat::Plain);
            vLayout->addWidget(std::move(executionStderrPtr));
        }
    }
}
