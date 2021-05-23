#include "DialogueWidget.hpp"

DialogueWidget::DialogueWidget(const std::string &dialogueName, const Dialogue &dialogue)
: dialogue_(dialogue) {
    auto dialogueNamePtr = std::make_unique<Wt::WText>(dialogueName);
    std::string lastMessageStr = (dialogue.getLastMessage().isHaveCode()) ?
            "code ..." : dialogue.getLastMessageView();
    auto lastMessagePtr = std::make_unique<Wt::WText>(lastMessageStr);
    auto timePtr = std::make_unique<Wt::WText>(dialogue.getTimeOfLastUpdateStr());

    dialogueName_ = dialogueNamePtr.get();
    lastMessage_ = lastMessagePtr.get();
    time_ = timePtr.get();

    createLayout(std::move(dialogueNamePtr),
                 std::move(lastMessagePtr),
                 std::move(timePtr));
}

void DialogueWidget::createLayout(std::unique_ptr<WWidget> dialogueName, std::unique_ptr<WWidget> lastMessage,
                                  std::unique_ptr<WWidget> time) {
    auto vLayout = std::make_unique<Wt::WVBoxLayout>();

    auto hLayout = std::make_unique<Wt::WHBoxLayout>();

    dialogueName->setStyleClass("dialogue-name");
    if (dialogue_.withYourself()) {
        dialogueName->addStyleClass("dialogue-name-self");
    }
    hLayout->addWidget(std::move(dialogueName), 1);
    time->setStyleClass("chat-time");
    hLayout->addWidget(std::move(time));
    vLayout->addLayout(std::move(hLayout), 0);

    hLayout = std::make_unique<Wt::WHBoxLayout>();
    if (dialogue_.getLastMessage().isHaveCode()) {
        lastMessage->setStyleClass("dialogue-code");
    }
    hLayout->addWidget(std::move(lastMessage));
    vLayout->addLayout(std::move(hLayout), 1);

    this->setLayout(std::move(vLayout));
}
