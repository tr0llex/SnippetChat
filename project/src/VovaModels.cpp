#include "VovaModels.h"


static inline int64_t getTimeS() {
    return std::chrono::duration_cast<std::chrono::seconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
}

Message::Message(const User &sender, uint32_t dialogueParentId, const std::wstring &messageText)
        : Message(sender.getId(), dialogueParentId, messageText) {}
Message::Message(uint32_t senderId, uint32_t dialogueParentId, const std::wstring &messageText)
        : senderId_(senderId),
          dialogueParentId_(dialogueParentId),
          messageText_(messageText),
          timeSent_(getTimeS()) {}

Message::Message(const User &sender, uint32_t dialogueParentId,
                 const std::wstring &messageText, const std::wstring &codeText)
        : Message(sender.getId(), dialogueParentId, messageText, codeText) {}

Message::Message(uint32_t senderId, uint32_t dialogueParentId, const std::wstring &messageText, const std::wstring &codeText)
        : senderId_(senderId),
          dialogueParentId_(dialogueParentId),
          messageText_(messageText),
          codeText_(codeText),
          timeSent_(getTimeS()) {}
