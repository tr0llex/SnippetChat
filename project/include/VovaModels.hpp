#ifndef SNIPPETCHAT_VOVAMODELS_HPP
#define SNIPPETCHAT_VOVAMODELS_HPP


#include <set>
#include <vector>
#include <chrono>
#include <string>


class User {
public:
    User() = default;
    explicit User(const std::wstring &username) : id_(0), username_(username) {}

    void setId(uint32_t id) {
        id_ = id;
    }
    uint32_t getId() const {
        return id_;
    }

    std::wstring getUsername() const {
        return username_;
    }
    std::wstring getPassword() const {
        return userPassword_;
    }

    void updateData(const User &newData) {
        username_ = newData.username_;
        userPassword_ = newData.userPassword_;
    }

    bool operator==(const User &user) const {
        return id_ == user.id_;
    }
    bool operator!=(const User &user) const {
        return id_ != user.id_;
    }

private:
    uint32_t id_;
    std::wstring username_;
    std::wstring userPassword_;
//    std::wstring userToken_;
//    int userStatus_;
};

class Message {
public:
    Message() = default;
    Message(const User &sender, uint32_t dialogueParentId, const std::wstring &messageText);
    Message(uint32_t senderId, uint32_t dialogueParentId, const std::wstring &messageText);

    uint32_t getSenderId() const {
        return senderId_;
    }
    uint32_t getDialogueId() const {
        return dialogueParentId_;
    }
    std::wstring getMessageText() const {
        return messageText_;
    }
    time_t getTimeSent() const {
        return timeSent_;
    }

    std::wstring timeSent() const {
        return std::to_wstring(timeSent_);
    }

    bool isMyMessage(const User &requester) const {
        return senderId_ == requester.getId();
    }

private:
    uint32_t senderId_;
    uint32_t dialogueParentId_;
    std::wstring messageText_;
    time_t timeSent_;
};


class DialogueInfo {
public:
    DialogueInfo() = default;
    DialogueInfo(const std::wstring &name, const Message &message) : dialogueName_(name), numberOfUnread_(0), lastMessage_(message) {}

    void newMessage(const Message &message) {
        lastMessage_ = message;
        numberOfUnread_++;
    }

    uint32_t getId() const {
        return lastMessage_.getDialogueId();
    }
    const std::wstring& getName() const {
        return dialogueName_;
    }
    const Message& getMessage() const {
        return lastMessage_;
    }
    bool withYourself(const User &requester) const {
        return dialogueName_ == requester.getUsername();
    }

private:
    std::wstring dialogueName_;
    uint16_t numberOfUnread_;
    Message lastMessage_;
};
struct ComparatorDialogueInfo {
    bool operator()(const DialogueInfo &lhs, const DialogueInfo &rhs) {
        return lhs.getMessage().getTimeSent() > rhs.getMessage().getTimeSent();
    }
};
typedef std::multiset<DialogueInfo, ComparatorDialogueInfo> DialogueList;


class Dialogue {
public:
    Dialogue() : dialogueId_(0) {}
    Dialogue(uint32_t dialogueId, const User &firstUser, const User &secondUser)
    : dialogueId_(dialogueId) {
        names.push_back(firstUser.getUsername());
        participantsList_.push_back(firstUser.getId());
        dialogueMessageList_.emplace_back(0, dialogueId_, std::wstring());

        if (firstUser != secondUser) {
            names.push_back(secondUser.getUsername());
            participantsList_.push_back(secondUser.getId());
        }
    }

    uint32_t getId() const {
        return dialogueId_;
    }
    std::vector<uint32_t> getParticipants() const {
        return participantsList_;
    }
    std::vector<Message> getMessages() const {
        return dialogueMessageList_;
    }

    bool thereIsParticipant(const User &participant) const {
        return thereIsParticipant(participant.getId());
    }
    bool thereIsParticipant(uint32_t participant) const {
        for (const auto &curParticipant : participantsList_) {
            if (participant == curParticipant) {
                return true;
            }
        }

        return false;
    }

    DialogueInfo getInfo(const User &requester) const {
        if (!thereIsParticipant(requester)) {
            return DialogueInfo();
        }

        std::wstring name = getName(requester);
        Message message = dialogueMessageList_.back();

        return DialogueInfo(name, message);
    }
    std::wstring getName(const User &requester) const {
        for (const auto &curName : names) {
            if (requester.getUsername() != curName) {
                return curName;
            }
        }

        return requester.getUsername();
    }

    void newMessage(const Message &message) {
        dialogueMessageList_.emplace_back(message);
    }

    bool withYourself() const {
        return participantsList_.size() == 1;
    }

    bool operator==(const Dialogue &dialogue) const {
        if (participantsList_.size() != dialogue.getParticipants().size()) {
            return false;
        }

        for (const auto &otherParticipant : dialogue.getParticipants()) {
            if (!thereIsParticipant(otherParticipant)) {
                return false;
            }
        }

        return true;
    }
    bool operator!=(const Dialogue &dialogue) const {
        return !(*this == dialogue);
    }

    bool operator==(const DialogueInfo &dialogue) const {
        return dialogueId_ == dialogue.getId();
    }
    bool operator!=(const DialogueInfo &dialogue) const {
        return !(*this == dialogue);
    }

private:
    uint32_t dialogueId_;
    std::vector<std::wstring> names;
    std::vector<uint32_t> participantsList_;
    std::vector<Message> dialogueMessageList_;
};


class DB {
public:
    DB() = default;


    bool addNewUser(User &user) {
        if (!nameIsFree(user)) {
            return false;
        }

        user.setId(users_.size());
        users_.push_back(user);
        return true;
    }
    bool isCorrectUser(User &user) {
        int i = findUserByName(user);
        if (i == -1 || users_[i].getPassword() != user.getPassword()) {
            return false;
        }

        user = users_[i];

        return true;
    }
    void addUserToAllDialogues(const User &user) {
        for (auto &curUser : users_) {
            newDialogue(curUser, user);
        }
    }
    bool updateUser(User& user, const User& newUser) {
        if (!nameIsFree(newUser)) {
            return false;
        }

        int i = findUser(user);
        if (i == -1) {
            return false;
        }

        users_[i].updateData(newUser);

        user = users_[i];

        return true;
    }
    bool newDialogue(const User &firstUser, const User &secondUser) {
        Dialogue dialogue(dialogues_.size(), firstUser, secondUser);

        for (const auto &curDialogue : dialogues_) {
            if (dialogue == curDialogue) {
                return false;
            }
        }

        dialogues_.push_back(dialogue);

        return true;
    }

    DialogueList getDialogueList(const User &user) const {
        DialogueList dialogueList;

        for (const auto &dialogue : dialogues_) {
            if (dialogue.thereIsParticipant(user)) {
                dialogueList.emplace(dialogue.getInfo(user));
            }
        }

        return dialogueList;
    }
    Dialogue getDialogue(uint32_t dialogueId) const {
        for (const auto &dialogue : dialogues_) {
            if (dialogue.getId() == dialogueId) {
                return dialogue;
            }
        }

        return Dialogue();
    }

    void saveMessage(const Message &message) {
        int i = findDialogue(message);
        if (i == -1) {
            return;
        }

        dialogues_[i].newMessage(message);
    }

    /// TODO
    const User& getUser(uint32_t id) {
        for (const auto &user : users_) {
            if (user.getId() == id) {
                return user;
            }
        }
        return users_[0];
    }
    const std::vector<User>& getUsers() const {
        return users_;
    }

private:
    bool nameIsFree(const User &user) const {
        for (const auto & otherUser : users_) {
            if (otherUser.getUsername() == user.getUsername()) {
                return false;
            }
        }

        return true;
    }
    int findUser(const User &user) const {
        for (size_t i = 0; i < users_.size(); ++i) {
            if (users_[i] == user) {
                return (int) i;
            }
        }

        return -1;
    }
    int findUserByName(const User &user) const {
        for (size_t i = 0; i < users_.size(); ++i) {
            if (users_[i].getUsername() == user.getUsername()) {
                return (int) i;
            }
        }

        return -1;
    }
    int findDialogue(const Message &message) const {
        for (size_t i = 0; i < dialogues_.size(); ++i) {
            if (dialogues_[i].getId() == message.getDialogueId()) {
                return (int) i;
            }
        }

        return -1;
    }

private:
    std::vector<User> users_;
    std::vector<Dialogue> dialogues_;

};


#endif //SNIPPETCHAT_VOVAMODELS_HPP
