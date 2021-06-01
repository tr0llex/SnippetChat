#include "ChatEvent.hpp"

std::string ChatEvent::getSenderLogin() const  {
    return dialogue_.getLastMessage().getSenderLogin();
}

std::string ChatEvent::getDialogueId() const  {
    return dialogue_.getId();
}

const std::string& ChatEvent::getMessageId() const {
    return messageId_;
}

ChatEvent::ChatEvent(Type type, const std::string &userLogin, const Dialogue& dialogue)
        : type_(type),
          userLogin_(userLogin),
          dialogue_(dialogue) {}

ChatEvent::ChatEvent(Type type, const std::string &userLogin, const std::string &dialogueId,
                     const std::string &messageId, const Compilation &resultCompilation)
        : type_(type),
          userLogin_(userLogin),
          messageId_(messageId),
          resultCompilation_(resultCompilation) {
    dialogue_.setId(dialogueId);
}
