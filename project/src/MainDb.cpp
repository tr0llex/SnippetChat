#include "MainDb.hpp"

    MainDb::MainDb() {
        CassFuture* connect_future_ = NULL;
        CassFuture* close_future_ = NULL;
        CassCluster* cluster_ = cass_cluster_new();
        CassSession* session_ = cass_session_new();
    }
    MainDb::~MainDb() {
        cass_future_free(connect_future_);
        cass_future_free(close_future_);
        cass_cluster_free(cluster_);
        cass_session_free(session_);
    }

    void MainDb::connectToDb(const char* contactPoints) {
        cass_cluster_set_contact_points(cluster_, contactPoints);
        connect_future_ = cass_session_connect(session_, cluster_);
    }
    void MainDb::disconnectFromDb() {
        close_future_ = cass_session_close(session_);
        cass_future_wait(close_future_);
        cass_future_free(close_future_);
    }

    User* MainDb::searchUserLogin(std::string login, std::string password) {
        return new User;
    }
    uint32_t MainDb::writeUser(User& user) {
        return rand();
    }
    int MainDb::updateUser(User& user) {
        return EXIT_SUCCESS;
    }

    std::string MainDb::getCodeFromMessage(int messageId) {
        return "";
    }
    void MainDb::writeMessageToDialogue(Message message) {
    }
    std::vector <Message>* MainDb::getNMessagesFromDialogue(uint32_t dialogueId, uint32_t senderId, 
                                                            uint32_t receiverId, long count) {
        std::vector <Message>* messages = new std::vector <Message>;
        return messages;
    }

    std::vector <Dialogue>* MainDb::getDialoguessByUserId(int userId) {
        std::vector <Dialogue>* dialogues = new std::vector <Dialogue>;
        return dialogues;
    }
    uint32_t MainDb::createDialogue(uint32_t senderId, uint32_t receiverId) {
        return rand();
    }
    void MainDb::deleteMessageFromDialogue(Message& message) {
    }
    void MainDb::deleteDialogue(Dialogue& dialogue) {
    }
