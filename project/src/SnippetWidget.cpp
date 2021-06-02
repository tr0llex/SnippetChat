#include <Wt/WAnimation.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTable.h>
#include <Wt/WTemplate.h>
#include <Wt/WVBoxLayout.h>

#include "SnippetWidget.hpp"

static inline std::string langToStyleClass(const Snippet &snippet) {
    switch (snippet.getLanguage()) {
        case Snippet::kPython_3:
            return "python";
        case Snippet::kCpp_14:
        case Snippet::kCpp_17:
        case Snippet::kCpp_20:
            return "cpp";
        case Snippet::kC_17:
            return "c";
        default:
            return std::string();
    }
}

SnippetWidget::SnippetWidget(const Snippet &snippet)
        : snippet_(snippet) {
    setWidth(580);

    auto snippetPanelPtr = std::make_unique<Wt::WPanel>();
    auto inputEditPtr = std::make_unique<Wt::WTextArea>();
    auto runButtonPtr = std::make_unique<Wt::WPushButton>();
    auto resultContainerPtr = std::make_unique<Wt::WContainerWidget>();

    snippetPanel_ = snippetPanelPtr.get();
    inputEdit_ = inputEditPtr.get();
    runButton_ = runButtonPtr.get();
    resultContainer_ = resultContainerPtr.get();

    snippetPanel_->setTitle("Code\t" + snippet_.getLanguageStr());
    snippetPanel_->setCollapsible(true);

    Wt::WAnimation animation(Wt::AnimationEffect::SlideInFromTop,
                             Wt::TimingFunction::EaseOut,
                             150);

    snippetPanel_->setAnimation(animation);
    snippetPanel_->collapse();

    snippetPanel_->setMargin(0);

    auto snippetTemplate = std::make_unique<Wt::WTemplate>("<pre style=\"padding: 0; margin: 0;\">"
                                                           "<code class=\"${lang-class}\" style=\"padding: 7px;\">"
                                                           "${snippet}"
                                                           "</code></pre>");
    snippetTemplate->bindString("lang-class", langToStyleClass(snippet_));
    auto programText = std::make_unique<Wt::WText>(snippet_.getProgramTextView(), Wt::TextFormat::UnsafeXHTML);
    snippetTemplate->bindWidget("snippet", std::move(programText));

    snippetPanel_->setCentralWidget(std::move(snippetTemplate));
    snippetPanel_->centralWidget()->setStyleClass("snippet");

    inputEdit_->setRows(3);
    inputEdit_->setPlaceholderText("input data");
    inputEdit_->setStyleClass("input-snippet");

    runButton_->addStyleClass("run-button bi-play");

    createLayout(std::move(snippetPanelPtr),
                 std::move(runButtonPtr),
                 std::move(inputEditPtr),
                 std::move(resultContainerPtr));
}

void SnippetWidget::setClickedRunButton(const std::function<void ()> &fn) {
    runButton_->clicked().connect(fn);
}

std::string SnippetWidget::getInput() const {
    runButton_->disable();
    runButton_->removeStyleClass("bi-play");
    runButton_->addStyleClass("bi-play-fill");

    resultContainer_->clear();

    resultContainer_->hide();

    std::string input = ws2s(inputEdit_->text());

    return input;
}

void SnippetWidget::setResultCompilation(const Compilation &result) {
    runButton_->enable();
    runButton_->removeStyleClass("bi-play-fill");
    runButton_->addStyleClass("bi-play");

    auto timeLimitExceededPtr = std::make_unique<Wt::WText>("Time limit (5 sec) exceeded!");
    auto compilerStderrPtr = std::make_unique<Wt::WText>(result.getCompilerStderr(), Wt::TextFormat::Plain);
    auto executionTimePtr = std::make_unique<Wt::WText>(result.getExecutionTime() + "s");
    auto executionUsedMemoryPtr = std::make_unique<Wt::WText>(result.getExecutionUsedMemory());
    auto metricTablePtr = std::make_unique<Wt::WTable>();
    auto compilerStdoutPtr = std::make_unique<Wt::WText>(result.getExecutionStdout(), Wt::TextFormat::Plain);
    auto executionStderrPtr = std::make_unique<Wt::WText>(result.getExecutionStderr(), Wt::TextFormat::Plain);

    metricTablePtr->setHeaderCount(1);

    metricTablePtr->elementAt(0, 0)->addNew<Wt::WText>("Time");
    metricTablePtr->elementAt(0, 1)->addNew<Wt::WText>("Memory");
    metricTablePtr->elementAt(1, 0)->addWidget(std::move(executionTimePtr));
    metricTablePtr->elementAt(1, 1)->addWidget(std::move(executionUsedMemoryPtr));

    createResultLayout(result,
                       std::move(timeLimitExceededPtr),
                       std::move(compilerStderrPtr),
                       std::move(metricTablePtr),
                       std::move(compilerStdoutPtr),
                       std::move(executionStderrPtr));

    resultContainer_->setStyleClass("source-view");
    resultContainer_->show();
}

void SnippetWidget::createLayout(std::unique_ptr<WWidget> programText, std::unique_ptr<WWidget> runButton,
                              std::unique_ptr<WWidget> inputEdit, std::unique_ptr<WWidget> resultContainer) {
    auto vLayout = std::make_unique<Wt::WVBoxLayout>();

    vLayout->addWidget(std::move(programText));
    if (snippet_.withLaunch()) {
        vLayout->addWidget(std::move(inputEdit));
        vLayout->addWidget(std::move(runButton), 0, Wt::AlignmentFlag::Right);
        vLayout->addWidget(std::move(resultContainer));
    }

    this->setLayout(std::move(vLayout));
}

void SnippetWidget::createResultLayout(const Compilation &result, std::unique_ptr<WWidget> timeLimitExceeded,
                                    std::unique_ptr<WWidget> compilerStderr,
                                    std::unique_ptr<WWidget> metricTable, std::unique_ptr<WWidget> compilerStdout,
                                    std::unique_ptr<WWidget> executionStderr) {
    auto vLayout = resultContainer_->setLayout(std::make_unique<Wt::WVBoxLayout>());

    if (result.getTimeLimitExceeded()) {
        vLayout->addWidget(std::move(timeLimitExceeded));
    }

    if (!result.getCompilerStderr().empty()) {
        vLayout->addWidget(std::move(compilerStderr));
    } else {
        if (!result.getTimeLimitExceeded()) {
            vLayout->addWidget(std::move(metricTable), 0, Wt::AlignmentFlag::Super);
        }

        vLayout->addWidget(std::move(compilerStdout));

        if (!result.getExecutionStderr().empty()) {
            vLayout->addWidget(std::move(executionStderr));
        }
    }
}
