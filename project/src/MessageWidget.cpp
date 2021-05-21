#include "MessageWidget.hpp"

MessageWidget::MessageWidget(const Message &message, std::unique_ptr<WWidget> runButtonPtr)
        : message_(message), code_(nullptr) {
    auto textPtr = std::make_unique<Wt::WText>(message.getMessageText());
    auto timePtr = std::make_unique<Wt::WText>(message.getTimeSentStr());
    std::unique_ptr<CodeWidget> codePtr = nullptr;

    text_ = textPtr.get();
    time_ = timePtr.get();
    if (isHaveCode()) {
        codePtr = std::make_unique<CodeWidget>(message.getMessageCode(), std::move(runButtonPtr));
        code_ = codePtr.get();
    }

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

    if (isHaveCode()) {
        vLayout->addWidget(std::move(code));
    }

    this->setLayout(std::move(vLayout));
}

std::string MessageWidget::getMessageId() const {
    return message_.getId();
}

bool MessageWidget::isHaveCode() const {
    return message_.isHaveCode();
}

std::string MessageWidget::getInput() const {
    if (!isHaveCode()) {
        return std::string();
    }
    return code_->getInput();
}

void MessageWidget::setResultCompilation(const std::string &result) {
    if (isHaveCode()) {
        code_->setResultCompilation(result);
    }
}
