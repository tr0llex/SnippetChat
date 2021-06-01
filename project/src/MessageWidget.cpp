#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WText.h>

#include "MessageWidget.hpp"

const int kNumberOfCharInLine = 45;

static inline std::string messageView(const std::string &text) {
    if (text.size() < kNumberOfCharInLine) {
        return text;
    }

    std::string textView;
    for (int i = 0, line = 1, count = 1; i < text.size(); ++i, ++count) {
        textView += text[i];
        if (text[i] < 0) {
            i++;
            textView += text[i];
        }

        if (count == kNumberOfCharInLine * line) {
            line++;
            textView += '\n';
        }
    }

    return textView;
}

MessageWidget::MessageWidget(const Message &message, bool myMassage)
        : message_(message), myMessage_(myMassage), snippet_(nullptr) {
    std::string textView = messageView(message.getMessageText());
    auto textPtr = std::make_unique<Wt::WText>(textView, Wt::TextFormat::Plain);
    auto timePtr = std::make_unique<Wt::WText>(message.getTimeSentStr());

    text_ = textPtr.get();
    time_ = timePtr.get();

    createLayout(std::move(textPtr),
                 std::move(timePtr));
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

void MessageWidget::setClickedRunButton(const std::function<void ()> &fn) {
    snippet_->setClickedRunButton(fn);
}

std::string MessageWidget::getInput() const {
    return snippet_->getInput();
}

void MessageWidget::createLayout(std::unique_ptr<WWidget> text, std::unique_ptr<WWidget> time) {
    auto messageContainer = std::make_unique<Wt::WContainerWidget>();
    messageContainer->setStyleClass("chat-msg");
    messageContainer->setMargin(5, Wt::Side::Bottom | Wt::Side::Top);

    auto vLayout = messageContainer->setLayout(std::make_unique<Wt::WVBoxLayout>());
    vLayout->setContentsMargins(0, 0, 0, 0);

    auto hLayout = std::make_unique<Wt::WHBoxLayout>();

    text->setStyleClass("text-in-msg");
    hLayout->addWidget(std::move(text), 0);
    time->setStyleClass("chat-time");
    hLayout->addWidget(std::move(time), 0, Wt::AlignmentFlag::Bottom | Wt::AlignmentFlag::Right);

    vLayout->addLayout(std::move(hLayout));

    if (message_.isHaveSnippet()) {
        auto snippetPtr = std::make_unique<SnippetWidget>(message_.getSnippet());
        snippet_ = snippetPtr.get();

        vLayout->addWidget(std::move(snippetPtr));
    }

    auto vPositionalLayout = std::make_unique<Wt::WVBoxLayout>();
    vPositionalLayout->setContentsMargins(8, 0, 8, 0);

    auto positionMessage = (myMessage_) ? Wt::AlignmentFlag::Right : Wt::AlignmentFlag::Left;
    vPositionalLayout->addWidget(std::move(messageContainer), 0, positionMessage);

    this->setLayout(std::move(vPositionalLayout));
}
