#include "MessageWidget.hpp"

MessageWidget::MessageWidget(const Message &message)
        : message_(message) {
    auto textPtr = std::make_unique<Wt::WText>(message.getMessageText());
    auto timePtr = std::make_unique<Wt::WText>(message.getTimeSentStr());

    text_ = textPtr.get();
    time_ = timePtr.get();

    createLayout(std::move(textPtr),
                 std::move(timePtr));
}

void MessageWidget::createLayout(std::unique_ptr<WWidget> text, std::unique_ptr<WWidget> time) {
    auto vLayout = std::make_unique<Wt::WVBoxLayout>();

    auto hLayout = std::make_unique<Wt::WHBoxLayout>();

    text->setStyleClass("text-in-msg");
    hLayout->addWidget(std::move(text), 1);
    time->setStyleClass("chat-time");
    hLayout->addWidget(std::move(time));

    vLayout->addLayout(std::move(hLayout));

    this->setLayout(std::move(vLayout));
}

std::string MessageWidget::getMessageId() const {
    return message_.getId();
}

bool MessageWidget::isHaveSnippet() const {
    return message_.isHaveSnippet();
}

void MessageWidget::setResultCompilation(const Compilation &result) {
    if (isHaveSnippet() && snippet_) {
        snippet_->setResultCompilation(result);
    }
}

void MessageWidget::setSnippet(std::unique_ptr<CodeWidget> snippetPtr) {
    snippet_ = snippetPtr.get();

    layout()->addWidget(std::move(snippetPtr));
}
