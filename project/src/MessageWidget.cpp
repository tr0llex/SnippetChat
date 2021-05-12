#include "MessageWidget.hpp"

MessageWidget::MessageWidget(const Message &message)
        : message_(message), code_(nullptr) {
    auto textPtr = std::make_unique<Wt::WText>(message.getMessageText());
    auto timePtr = std::make_unique<Wt::WText>(message.timeSent());
    auto codePtr = (message.isCode()) ? std::make_unique<CodeWidget>(message) : nullptr;

    text_ = textPtr.get();
    time_ = timePtr.get();
    code_ = (message.isCode()) ? codePtr.get() : nullptr;

    createLayout(std::move(textPtr),
                 std::move(timePtr),
                 std::move(codePtr));
}

void MessageWidget::createLayout(std::unique_ptr<WWidget> text, std::unique_ptr<WWidget> time,
                                 std::unique_ptr<WWidget> code) {
    auto vLayout = std::make_unique<Wt::WVBoxLayout>();

    auto hLayout = std::make_unique<Wt::WHBoxLayout>();

    text->setStyleClass("text-in-msg");
    hLayout->addWidget(std::move(text), 1);
    time->setStyleClass("time-in-msg");
    hLayout->addWidget(std::move(time));

    vLayout->addLayout(std::move(hLayout));

    if (code_) {
        vLayout->addWidget(std::move(code));
    }

    this->setLayout(std::move(vLayout));
}
