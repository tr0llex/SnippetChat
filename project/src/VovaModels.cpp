#include "VovaModels.hpp"


static inline int64_t getTimeS() {
    return std::chrono::duration_cast<std::chrono::seconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
}

Message::Message(const User &senderId, uint32_t dialogueParentId, const std::wstring &messageText)
        : Message(senderId.getId(), dialogueParentId, messageText) {}

Message::Message(uint32_t senderId, uint32_t dialogueParentId, const std::wstring &messageText)
        : senderId_(senderId),
          dialogueParentId_(dialogueParentId),
          messageText_(messageText),
          timeSent_(getTimeS()) {}
