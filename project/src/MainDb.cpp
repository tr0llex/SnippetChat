#include <vector>
#include <exception>
#include "MainDb.hpp"

MainDb::MainDb() {
    connect_future_ = NULL;
    close_future_ = NULL;
    cluster_ = cass_cluster_new();
    session_ = cass_session_new();
}

MainDb::~MainDb() {
    if (connect_future_) {
        cass_future_free(connect_future_);
    }
    if (close_future_) {
        cass_future_free(close_future_);
    }
    if (cluster_) {
        cass_cluster_free(cluster_);
    }
    if (session_) {
        cass_session_free(session_);
    }
}


void MainDb::connectToDb(const char* contactPoints) {
    cass_cluster_set_contact_points(cluster_, contactPoints);
    connect_future_ = cass_session_connect(session_, cluster_);
//    DEBUG
    CassError rc = cass_future_error_code(connect_future_);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(connect_future_, &message, &message_length);
        fprintf(stderr, "Connect error: '%.*s'\n", (int)message_length, message);
    }
//      DEBUG

   migrate();
}

void MainDb::migrate() {

    const char* keyspaceQ = "CREATE KEYSPACE IF NOT EXISTS mainDB "  // Keyspace and tables
    "WITH REPLICATION = { 'class' : 'SimpleStrategy',"
    "'replication_factor' : 1 };";
    const char* usersTQ = "CREATE TABLE IF NOT EXISTS mainDB.users_by_login ("
    "login text PRIMARY KEY,"
    "password text,"
    "dialogues_id list<text>"
    ")"
    ";";
    const char* dialoguesByIdTQ = "CREATE TABLE IF NOT EXISTS mainDB.dialogues_by_id ("
    "dialogue_id uuid PRIMARY KEY,"
    "participants_logins list<text>"
    ")"
    ";";
    const char* messagesByIdTQ = "CREATE TABLE IF NOT EXISTS mainDB.messages_by_id ("
    "message_id uuid,"
    "dialogue_id uuid,"
    "sender_id uuid,"
    "message_text text,"
    "message_code text,"
    "time_sent timestamp,"
    "is_read boolean,"
    "PRIMARY KEY (message_id, dialogue_id, time_sent)"
    ") "
    "WITH CLUSTERING ORDER BY (dialogue_id ASC, time_sent DESC)"
    ";";
    const char* messageByIdIndForDialog = "CREATE INDEX IF NOT EXISTS ON mainDB.messages_by_id (dialogue_id);";


    CassStatement* keyspaceSt = cass_statement_new(keyspaceQ, 0);  // made statement
    CassFuture* ks_future = cass_session_execute(session_, keyspaceSt);

// DEBUG
    CassError rc = cass_future_error_code(ks_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(ks_future, &message, &message_length);
        fprintf(stderr, "St error: '%.*s'\n", (int)message_length, message);
    }
// DEBUG

    CassStatement* usersByIdSt = cass_statement_new(usersTQ, 0);  // made statement
    CassFuture* user_by_id_future = cass_session_execute(session_, usersByIdSt);

    CassStatement* dialoguesByIdSt = cass_statement_new(dialoguesByIdTQ, 0);  // made statement
    CassFuture* dialogues_by_id_future = cass_session_execute(session_, dialoguesByIdSt);

    CassStatement* messagesByIdSt = cass_statement_new(messagesByIdTQ, 0);  // made statement
    CassFuture* messages_by_id_future = cass_session_execute(session_, messagesByIdSt);

    CassStatement* messagesByIdIndForDialogSt = cass_statement_new(messageByIdIndForDialog, 0);  // made statement
    CassFuture* messagesByIdIndForDialogSt_future = cass_session_execute(session_, messagesByIdIndForDialogSt);

    CassError rc1 = cass_future_error_code(messagesByIdIndForDialogSt_future);

    if (rc1 != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(messagesByIdIndForDialogSt_future, &message, &message_length);
        fprintf(stderr, "St error: '%.*s'\n", (int)message_length, message);
    }

    cass_statement_free(keyspaceSt);
    cass_statement_free(messagesByIdSt);
    cass_statement_free(dialoguesByIdSt);
    cass_statement_free(usersByIdSt);

    cass_future_free(ks_future);
    cass_future_free(messages_by_id_future);
    cass_future_free(dialogues_by_id_future);
    cass_future_free(user_by_id_future);
}
    
void MainDb::disconnectFromDb() {
    close_future_ = cass_session_close(session_);
    cass_future_wait(close_future_);
    cass_future_free(close_future_);
}

User* MainDb::searchUserLogin(std::string login, std::string password) {  // function pull dialogues_list
    const char* searchUser = "SELECT dialogues_id FROM maindb.users_by_login "
                             "WHERE login = ? AND password = ? LIMIT 1 ALLOW FILTERING;";
    CassStatement* returnedUser = cass_statement_new(searchUser, 2);
    cass_statement_bind_string(returnedUser, 0, login.data());
    cass_statement_bind_string(returnedUser, 1, password.data());
    CassFuture* returnedUser_future = cass_session_execute(session_, returnedUser);

    CassError rc = cass_future_error_code(returnedUser_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(returnedUser_future, &message, &message_length);
        fprintf(stderr, "St error: '%.*s'\n", (int)message_length, message);
    }

    const CassResult* result = cass_future_get_result(returnedUser_future);
    if (result == NULL) {  // throw exception
        cass_statement_free(returnedUser);
        cass_future_free(returnedUser_future);
        return new User();
    }

    const CassRow* row = cass_result_first_row(result);
    if (row == NULL) {  // user doesn't exist or password is incorrect
        cass_result_free(result);
        cass_statement_free(returnedUser);
        cass_future_free(returnedUser_future);
        return new User();
    }
    const CassValue* dialogues_column = cass_row_get_column_by_name(row, "dialogues_id");

    std::vector<std::string> dialogues_list;
    CassIterator* dialogues_iterator = cass_iterator_from_collection(dialogues_column);
    while (cass_iterator_next(dialogues_iterator)) {
        const CassValue* current_dialogue = cass_iterator_get_value(dialogues_iterator);

        const char* dialogue_value;
        size_t dialogue_value_length;
        cass_value_get_string(current_dialogue, &dialogue_value, &dialogue_value_length);
    
        std::string temp_str = dialogue_value;
        temp_str[dialogue_value_length] = '\0';
    
        dialogues_list.push_back(temp_str);
    }

    cass_result_free(result);
    cass_iterator_free(dialogues_iterator);
    cass_statement_free(returnedUser);
    cass_future_free(returnedUser_future);

    return new User(login, password, dialogues_list);
}

void MainDb::writeUser(User& user) {
    const char* insertUser = "INSERT INTO maindb.users_by_login " 
    "(login, password, dialogues_id) VALUES "
    "(?, ?, ?)"
    ";";

    CassStatement* newUser = cass_statement_new(insertUser, 3);  // made statement

    CassCollection* list = cass_collection_new(CASS_COLLECTION_TYPE_LIST, user.getDialogues().size());
    for (int i = 0; i < user.getDialogues().size(); i++) {
        cass_collection_append_string(list, user.getDialogues()[i].data());
    }

    cass_statement_bind_collection(newUser, 2, list);
    cass_statement_bind_string(newUser, 1, user.getPassword().data());
    cass_statement_bind_string(newUser, 0, user.getLogin().data());

    CassFuture* newUser_future = cass_session_execute(session_, newUser);

    cass_statement_free(newUser);
    cass_future_free(newUser_future);
}

int MainDb::updateUser(User& user) {
    const char* updateUserQ = "UPDATE maindb.users_by_login "
    "SET password = ?, "
    "dialogues_id = ? "
    "WHERE login = ?;";

    CassStatement* newUser = cass_statement_new(updateUserQ, 3);  // made statement

    CassCollection* list = cass_collection_new(CASS_COLLECTION_TYPE_LIST, user.getDialogues().size());
    for (int i = 0; i < user.getDialogues().size(); i++) {
        cass_collection_append_string(list, user.getDialogues()[i].data());
    }

    cass_statement_bind_string(newUser, 2, user.getLogin().data());
    cass_statement_bind_collection(newUser, 1, list);
    cass_statement_bind_string(newUser, 0, user.getPassword().data());

    CassFuture* newUser_future = cass_session_execute(session_, newUser);

    cass_statement_free(newUser);
    cass_future_free(newUser_future);

    return EXIT_SUCCESS;
}

std::string MainDb::getCodeFromMessage(std::string messageId) {
    const char* getCode = "SELECT message_code FROM maindb.messages_by_id "
                          "WHERE message_id = ?;";
    CassUuid uuid;
    cass_uuid_from_string(messageId.data(), &uuid);
    CassStatement* returnedCode = cass_statement_new(getCode, 1);
    cass_statement_bind_uuid(returnedCode, 0, uuid);
    CassFuture* returnedCode_future = cass_session_execute(session_, returnedCode);

    const CassResult* result = cass_future_get_result(returnedCode_future);
    if (result == NULL) { // throw exception
        cass_statement_free(returnedCode);
        cass_future_free(returnedCode_future);
        return "";
    }
    const CassRow* row = cass_result_first_row(result);

    if (row == NULL) {  // user doesn't exist or password is incorrect
        cass_result_free(result);
        cass_statement_free(returnedCode);
        cass_future_free(returnedCode_future);
        return "";
    }

    const CassValue* code_column = cass_row_get_column(row, 0);

    const char* code_value;
    size_t code_value_length;
    cass_value_get_string(code_column, &code_value, &code_value_length);
    
    std::string code = code_value;
    

    cass_result_free(result);
    cass_statement_free(returnedCode);
    cass_future_free(returnedCode_future);
    
    return code;
}

void MainDb::writeMessage(Message& message) {
    // exceptions if message is not full filled
    if (message.getDialogueParentId() == "" || message.getMessageId() == "" 
        || message.getSenderId() == "" || message.getTimeSent() == 0) {
        throw(-2);
    }
    const char* insertMessage = "INSERT INTO maindb.messages_by_id " 
    "(message_id, dialogue_id, sender_id, message_text, message_code, time_sent, is_read) VALUES "
    "(?, ?, ?, ?, ?, ?, ?)"
    ";";
    CassUuid uuid;
    CassStatement* insertMessageSt = cass_statement_new(insertMessage, 7);
    cass_uuid_from_string(message.getMessageId().data(), &uuid);
    cass_statement_bind_uuid(insertMessageSt, 0, uuid);

    cass_uuid_from_string(message.getDialogueParentId().data(), &uuid);
    cass_statement_bind_uuid(insertMessageSt, 1, uuid);

    cass_uuid_from_string(message.getSenderId().data(), &uuid);
    cass_statement_bind_uuid(insertMessageSt, 2, uuid);

    cass_statement_bind_string(insertMessageSt, 3, message.getMessageText().data());
    cass_statement_bind_string(insertMessageSt, 4, message.getMessageCode().data());
    cass_statement_bind_int64(insertMessageSt, 5, message.getTimeSent());
    cass_statement_bind_bool(insertMessageSt, 6, (cass_bool_t)message.isRead());

    CassFuture* insertMessageSt_future = cass_session_execute(session_, insertMessageSt);

    cass_statement_free(insertMessageSt);
    cass_future_free(insertMessageSt_future);
}

std::vector <Message>* MainDb::getNMessagesFromDialogue(std::string dialogueId, long count) {
    char* getNMessagesQ = "SELECT * FROM maindb.messages_by_id "
    "WHERE dialogue_id = ? LIMIT ?;";
    CassStatement* getNMessagesSt = cass_statement_new(getNMessagesQ, 2);
    CassUuid uuid;
    cass_uuid_from_string(dialogueId.data(), &uuid);
    cass_statement_bind_uuid(getNMessagesSt, 0, uuid);
    cass_statement_bind_int32(getNMessagesSt, 1, count);
    CassFuture* getNMessagesSt_future = cass_session_execute(session_, getNMessagesSt);

    CassError rc = cass_future_error_code(getNMessagesSt_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(getNMessagesSt_future, &message, &message_length);
        fprintf(stderr, "St error: '%.*s'\n", (int)message_length, message);
    }

    const CassResult* result = cass_future_get_result(getNMessagesSt_future);
    if (result == NULL) {  // throw exception
        cass_statement_free(getNMessagesSt);
        cass_future_free(getNMessagesSt_future);
        return nullptr;
    }

    if (cass_result_first_row(result) == NULL) {  // user doesn't exist or password is incorrect
        cass_result_free(result);
        cass_statement_free(getNMessagesSt);
        cass_future_free(getNMessagesSt_future);
        return nullptr;
    }

    CassIterator* messages_iterator = cass_iterator_from_result(result);

    std::vector <Message>* messages = new std::vector <Message>;

    while (cass_iterator_next(messages_iterator)) {
        const CassRow* row = cass_iterator_get_row(messages_iterator);
    
        CassUuid messageUuid;
        char* messageUuidStr;
        const CassValue* messageId = cass_row_get_column_by_name(row, "message_id");
        cass_value_get_uuid(messageId, &messageUuid);
        cass_uuid_string(messageUuid, messageUuidStr);

        CassUuid dialogueUuid;
        char* dialogueUuidStr;
        const CassValue* dialogueId = cass_row_get_column_by_name(row, "dialogue_id");
        cass_value_get_uuid(dialogueId, &dialogueUuid);
        cass_uuid_string(dialogueUuid, dialogueUuidStr);

        CassUuid senderUuid;
        char* senderUuidStr;
        const CassValue* senderId = cass_row_get_column_by_name(row, "sender_id");
        cass_value_get_uuid(senderId, &senderUuid);
        cass_uuid_string(senderUuid, senderUuidStr);

        const char* messageTextStr;
        size_t messageTextLength;
        const CassValue* messageText = cass_row_get_column_by_name(row, "message_text");
        cass_value_get_string(messageText, &messageTextStr, &messageTextLength);

        const char* messageCodeStr;
        size_t messageCodeLength;
        const CassValue* messageCode = cass_row_get_column_by_name(row, "message_code");
        cass_value_get_string(messageCode, &messageCodeStr, &messageCodeLength);

        time_t messageTimeT;
        const CassValue* messageTime = cass_row_get_column_by_name(row, "time_sent");
        cass_value_get_int64(messageTime, &messageTimeT);

        bool isReadB;
        cass_bool_t cassBool;
        const CassValue* isRead = cass_row_get_column_by_name(row, "is_read");
        cass_value_get_bool(isRead, &cassBool);
        isReadB = cassBool;

        Message newMessage((std::string)messageUuidStr, (std::string)dialogueUuidStr, 
                           (std::string)senderUuidStr, (std::string)messageTextStr,
                           (std::string)messageCodeStr, messageTimeT, isReadB);

    
        messages->push_back(newMessage);
    }


    cass_result_free(result);
    cass_iterator_free(messages_iterator);
    cass_statement_free(getNMessagesSt);
    cass_future_free(getNMessagesSt_future);

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
