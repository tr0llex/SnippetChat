#include <vector>
#include <exception>
#include <algorithm>
#include <time.h>
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
    "password text"
    ")"
    ";";
    const char* dialoguesByIdTQ = "CREATE TABLE IF NOT EXISTS mainDB.dialogues_by_id ("
    "dialogue_id uuid PRIMARY KEY,"
    "participants_logins set<text>"
    ")"
    ";";
    const char* userDialoguesTQ = "CREATE TABLE IF NOT EXISTS mainDB.user_dialogues ("
    "login text,"
    "dialogue_id uuid,"
    "time_update timestamp,"
    "PRIMARY KEY(login, time_update)"
    ") "
    "WITH CLUSTERING ORDER BY (time_update DESC)"
    ";";
    const char* messagesByIdTQ = "CREATE TABLE IF NOT EXISTS mainDB.messages_by_id ("
    "message_id uuid,"
    "dialogue_id uuid,"
    "sender_login text,"
    "message_text text,"
    "message_code text,"
    "time_sent timestamp,"
    "is_read boolean,"
    "lang int,"
    "PRIMARY KEY (dialogue_id, time_sent)"
    ") "
    "WITH CLUSTERING ORDER BY (time_sent DESC)"
    ";";
    const char* messageByIdIndForDialog = "CREATE INDEX IF NOT EXISTS ON mainDB.messages_by_id (dialogue_id);";
    const char* dialoguesByIdForParticipants = "CREATE INDEX IF NOT EXISTS "
                                               "ON mainDB.dialogues_by_id (participants_logins);";


    CassStatement* keyspaceSt = cass_statement_new(keyspaceQ, 0);  // made statement
    CassFuture* ks_future = cass_session_execute(session_, keyspaceSt);

// DEBUG
    CassError rc = cass_future_error_code(ks_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(ks_future, &message, &message_length);
        fprintf(stderr, "St3 error: '%.*s'\n", (int)message_length, message);
    }
// DEBUG

    CassStatement* usersByIdSt = cass_statement_new(usersTQ, 0);  // made statement
    CassFuture* usersByIdSt_future = cass_session_execute(session_, usersByIdSt);

    CassStatement* dialoguesByIdSt = cass_statement_new(dialoguesByIdTQ, 0);  // made statement
    CassFuture* dialoguesByIdSt_future = cass_session_execute(session_, dialoguesByIdSt);

    CassStatement* userDialoguesSt = cass_statement_new(userDialoguesTQ, 0);  // made statement
    CassFuture* userDialoguesSt_future = cass_session_execute(session_, userDialoguesSt);

    CassStatement* messagesByIdSt = cass_statement_new(messagesByIdTQ, 0);  // made statement
    CassFuture* messagesByIdSt_future = cass_session_execute(session_, messagesByIdSt);

    CassError rc1 = cass_future_error_code(messagesByIdSt_future);

    if (rc1 != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(messagesByIdSt_future, &message, &message_length);
        fprintf(stderr, "St228 error: '%.*s'\n", (int)message_length, message);
    }

    CassStatement* messagesByIdIndForDialogSt = cass_statement_new(messageByIdIndForDialog, 0);  // made statement
    CassFuture* messagesByIdIndForDialogSt_future = cass_session_execute(session_, messagesByIdIndForDialogSt);

    CassStatement* dialoguesByIdForParticipantsSt = cass_statement_new(dialoguesByIdForParticipants, 0);
    CassFuture* dialoguesByIdForParticipantsSt_future = cass_session_execute(session_, dialoguesByIdForParticipantsSt);


    cass_statement_free(keyspaceSt);
    cass_statement_free(messagesByIdSt);
    cass_statement_free(dialoguesByIdSt);
    cass_statement_free(userDialoguesSt);
    cass_statement_free(usersByIdSt);
    cass_statement_free(messagesByIdIndForDialogSt);
    cass_statement_free(dialoguesByIdForParticipantsSt);

    cass_future_free(ks_future);
    cass_future_free(messagesByIdSt_future);
    cass_future_free(dialoguesByIdSt_future);
    cass_future_free(userDialoguesSt_future);
    cass_future_free(usersByIdSt_future);
    cass_future_free(messagesByIdIndForDialogSt_future);
    cass_future_free(dialoguesByIdForParticipantsSt_future);
}
    
void MainDb::disconnectFromDb() {
    close_future_ = cass_session_close(session_);
    cass_future_wait(close_future_);
    cass_future_free(close_future_);
}

User MainDb::searchUserPassword(std::string login, std::string password) {  // function pull dialogues_list
    const char* searchUser = "SELECT * FROM maindb.users_by_login "
                             "WHERE login = ? LIMIT 1;";
    CassStatement* returnedUser = cass_statement_new(searchUser, 1);
    cass_statement_bind_string(returnedUser, 0, login.data());
    CassFuture* returnedUser_future = cass_session_execute(session_, returnedUser);

    CassError rc = cass_future_error_code(returnedUser_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(returnedUser_future, &message, &message_length);
        fprintf(stderr, "St1 error: '%.*s'\n", (int)message_length, message);
    }

    const CassResult* result = cass_future_get_result(returnedUser_future);
    if (result == NULL) {  // throw exception
        cass_statement_free(returnedUser);
        cass_future_free(returnedUser_future);
        return User();
    }

    const CassRow* row = cass_result_first_row(result);
    if (row == NULL) {  // user doesn't exist
        cass_result_free(result);
        cass_statement_free(returnedUser);
        cass_future_free(returnedUser_future);
        return User();
    }
    const CassValue* passwordColumn = cass_row_get_column_by_name(row, "password");
    const char* passwordStr;
    size_t passwordStrLength;
    cass_value_get_string(passwordColumn, &passwordStr, &passwordStrLength);
    std::string pasStr = passwordStr;
    pasStr = pasStr.substr(0, passwordStrLength);
    if (pasStr != password) {  // password is incorrect
        return User();
    }

    cass_result_free(result);
    cass_statement_free(returnedUser);
    cass_future_free(returnedUser_future);

    return User(login, password, "", 1);
}

bool MainDb::searchUserLogin(std::string login) const {  // function pull dialogues_list
    const char* searchUser = "SELECT * FROM maindb.users_by_login "
                             "WHERE login = ? LIMIT 1;";
    CassStatement* returnedUser = cass_statement_new(searchUser, 1);
    cass_statement_bind_string(returnedUser, 0, login.data());
    CassFuture* returnedUser_future = cass_session_execute(session_, returnedUser);

    CassError rc = cass_future_error_code(returnedUser_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(returnedUser_future, &message, &message_length);
        fprintf(stderr, "St4 error: '%.*s'\n", (int)message_length, message);
    }

    const CassResult* result = cass_future_get_result(returnedUser_future);
    if (result == NULL) {  // throw exception
        cass_statement_free(returnedUser);
        cass_future_free(returnedUser_future);
        return false;
    }

    const CassRow* row = cass_result_first_row(result);
    if (row == NULL) {  // user doesn't exist
        cass_result_free(result);
        cass_statement_free(returnedUser);
        cass_future_free(returnedUser_future);
        return false;
    }


    cass_result_free(result);
    cass_statement_free(returnedUser);
    cass_future_free(returnedUser_future);

    return true;
}

int MainDb::writeUser(const User& user) {
    if (searchUserLogin(user.getLogin())) {
        return EXIT_FAILURE;  // user is already exists
    }

    const char* insertUser = "INSERT INTO maindb.users_by_login "
    "(login, password) VALUES "
    "(?, ?)"
    ";";

    CassStatement* newUser = cass_statement_new(insertUser, 2);  // made statement

    cass_statement_bind_string(newUser, 1, user.getPassword().data());
    cass_statement_bind_string(newUser, 0, user.getLogin().data());

    CassFuture* newUser_future = cass_session_execute(session_, newUser);

    cass_statement_free(newUser);
    cass_future_free(newUser_future);

    return EXIT_SUCCESS;
}

void MainDb::changePassword(const User& user) {
    const char* updateUserQ = "UPDATE maindb.users_by_login "
    "SET password = ? "
    "WHERE login = ?;";

    CassStatement* newUser = cass_statement_new(updateUserQ, 2);  // made statement

    cass_statement_bind_string(newUser, 1, user.getLogin().data());
    cass_statement_bind_string(newUser, 0, user.getPassword().data());

    CassFuture* newUser_future = cass_session_execute(session_, newUser);

    cass_statement_free(newUser);
    cass_future_free(newUser_future);
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

void MainDb::updateDialogueTime(Message& message) {
    const char* getTimeUpdate = "SELECT time_update FROM mainDB.user_dialogues "
                                "WHERE dialogue_id = ?;";
    
    CassStatement* getTimeUpdateSt = cass_statement_new(getTimeUpdate, 1);
    CassUuid uuid;
    cass_uuid_from_string(message.getId().data(), &uuid);

    cass_statement_bind_uuid(getTimeUpdateSt, 1, uuid);
    CassFuture* getTimeUpdateSt_future = cass_session_execute(session_, getTimeUpdateSt);

    const CassResult* result = cass_future_get_result(getTimeUpdateSt_future);
    if (result == NULL) {  // throw exception
        cass_statement_free(getTimeUpdateSt);
        cass_future_free(getTimeUpdateSt_future);
        return;
    }

    if (cass_result_first_row(result) == NULL) {  // exception
        cass_result_free(result);
        cass_statement_free(getTimeUpdateSt);
        cass_future_free(getTimeUpdateSt_future);
        return;
    }

    const CassRow* row = cass_result_first_row(result);
    const CassValue* timeSentValue = cass_row_get_column(row, 0);
    time_t timeSent;
    cass_value_get_int64(timeSentValue, &timeSent);
    if (timeSent < message.getTimeSent()) {
        const char* updateTime = "UPDATE mainDB.user_dialogues "
                                 "SET time_update = ? "
                                 "WHERE login = ? and dialogue_id = ?;";
        CassStatement* updateTimeSt = cass_statement_new(updateTime, 3);
        cass_statement_bind_uuid(updateTimeSt, 2, uuid);
        cass_statement_bind_string(updateTimeSt, 1, message.getSenderLogin().data());
        cass_statement_bind_int64(updateTimeSt, 0, message.getTimeSent());
        CassFuture* updateTimeSt_future = cass_session_execute(session_, updateTimeSt);
        cass_statement_free(updateTimeSt);
        cass_future_free(updateTimeSt_future);
    }

    cass_statement_free(getTimeUpdateSt);
    cass_future_free(getTimeUpdateSt_future);
}


void MainDb::writeMessage(Message& message) {
    // exceptions if message is not full filled
    // if (message.getDialogueParentId() == "" || message.getMessageId() == "" 
    //     || message.getSenderLogin() == "" || message.getTimeSent() == 0) {
    //     throw(-2);
    // }
    const char* insertMessage = "INSERT INTO maindb.messages_by_id " 
    "(message_id, dialogue_id, sender_login, message_text, message_code, time_sent, is_read, lang) VALUES "
    "(?, ?, ?, ?, ?, ?, ?, ?)"
    ";";
    CassUuidGen* uuidGen = cass_uuid_gen_new();
    CassUuid uuid;

    char uuidStr[CASS_UUID_STRING_LENGTH];

    cass_uuid_gen_random(uuidGen, &uuid);
    cass_uuid_gen_free(uuidGen);
    cass_uuid_string(uuid, uuidStr);

    message.setId(uuidStr);

    CassStatement* insertMessageSt = cass_statement_new(insertMessage, 8);
    cass_uuid_from_string(message.getId().data(), &uuid);
    cass_statement_bind_uuid(insertMessageSt, 0, uuid);

    cass_uuid_from_string(message.getDialogueParentId().data(), &uuid);
    cass_statement_bind_uuid(insertMessageSt, 1, uuid);

    cass_statement_bind_string(insertMessageSt, 2, message.getSenderLogin().data());

    cass_statement_bind_string(insertMessageSt, 3, message.getMessageText().data());
    cass_statement_bind_string(insertMessageSt, 4, message.getMessageCode().data());
    cass_statement_bind_int64(insertMessageSt, 5, message.getTimeSent());
    cass_statement_bind_bool(insertMessageSt, 6, (cass_bool_t)message.isRead());
    cass_statement_bind_int32(insertMessageSt, 7, message.getCodeLang());

    CassFuture* insertMessageSt_future = cass_session_execute(session_, insertMessageSt);

    updateDialogueTime(message);

    cass_statement_free(insertMessageSt);
    cass_future_free(insertMessageSt_future);
}

std::vector<Message> MainDb::getNLastMessagesFromDialogue(std::string dialogueId, long count) const {
    std::vector<Message> messages;

    const char* getNMessagesQ = "SELECT * FROM maindb.messages_by_id "
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
        fprintf(stderr, "St5 error: '%.*s'\n", (int)message_length, message);
    }

    const CassResult* result = cass_future_get_result(getNMessagesSt_future);
    if (result == NULL) {  // throw exception
        cass_statement_free(getNMessagesSt);
        cass_future_free(getNMessagesSt_future);
        return messages;
    }

    if (cass_result_first_row(result) == NULL) {  // user doesn't exist or password is incorrect
        cass_result_free(result);
        cass_statement_free(getNMessagesSt);
        cass_future_free(getNMessagesSt_future);
        return messages;
    }

    fillMessagesFromResult(messages, result);
    std::reverse(messages.begin(), messages.end());

    cass_result_free(result);
    cass_statement_free(getNMessagesSt);
    cass_future_free(getNMessagesSt_future);

    return messages;
}

void MainDb::fillMessagesFromResult(std::vector<Message>& messages, const CassResult* result) const {
    CassIterator* messages_iterator = cass_iterator_from_result(result);

    while (cass_iterator_next(messages_iterator)) {
        const CassRow* row = cass_iterator_get_row(messages_iterator);

        CassUuid messageUuid;
        char messageUuidStr[CASS_UUID_STRING_LENGTH];
        const CassValue* messageId = cass_row_get_column_by_name(row, "message_id");
        cass_value_get_uuid(messageId, &messageUuid);
        cass_uuid_string(messageUuid, messageUuidStr);

        CassUuid dialogueUuid;
        char dialogueUuidStr[CASS_UUID_STRING_LENGTH];
        const CassValue* currDialogueId = cass_row_get_column_by_name(row, "dialogue_id");
        cass_value_get_uuid(currDialogueId, &dialogueUuid);
        cass_uuid_string(dialogueUuid, dialogueUuidStr);

        const char* senderLogin;
        size_t senderLoginLength;
        const CassValue* senderLoginValue = cass_row_get_column_by_name(row, "sender_login");
        cass_value_get_string(senderLoginValue, &senderLogin, &senderLoginLength);
        std::string senderLoginStr = senderLogin;
        senderLoginStr = senderLoginStr.substr(0, senderLoginLength);


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

        int lang;
        const CassValue* messageLang = cass_row_get_column_by_name(row, "lang");
        cass_value_get_int32(messageLang, &lang);

        Snippet snippet(static_cast<std::string>(messageCodeStr), 
                        static_cast<Snippet::Language>(lang));

        Message newMessage(messageUuidStr, dialogueUuidStr,
                           senderLoginStr, messageTextStr,
                           snippet, messageTimeT, isReadB);

        messages.push_back(newMessage);
    }
    cass_iterator_free(messages_iterator);
}

std::vector<std::string> MainDb::getAllDialoguesIdByLogin(std::string login) {
    std::vector <std::string> dialoguesId;
    const char* searchDialogues = "SELECT dialogue_id FROM maindb.user_dialogues "
                                  "WHERE login = ?;";
    CassStatement* searchDialoguesSt = cass_statement_new(searchDialogues, 1);
    cass_statement_bind_string(searchDialoguesSt, 0, login.data());
    CassFuture* searchDialoguesSt_future = cass_session_execute(session_, searchDialoguesSt);

    CassError rc = cass_future_error_code(searchDialoguesSt_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(searchDialoguesSt_future, &message, &message_length);
        fprintf(stderr, "St6 error: '%.*s'\n", (int)message_length, message);
    }

    const CassResult* result = cass_future_get_result(searchDialoguesSt_future);
    if (result == NULL) {
        cass_statement_free(searchDialoguesSt);
        cass_future_free(searchDialoguesSt_future);
        return dialoguesId;  // throw exception
    }

    const CassRow* row = cass_result_first_row(result);
    if (row == NULL) {  // user doesn't exist or password is incorrect
        cass_result_free(result);
        cass_statement_free(searchDialoguesSt);
        cass_future_free(searchDialoguesSt_future);
        return dialoguesId;  // throw exception
    }


    CassIterator* dialogues_iterator = cass_iterator_from_result(result);
    while (cass_iterator_next(dialogues_iterator)) {
        const CassRow* currRow = cass_iterator_get_row(dialogues_iterator);

        CassUuid dialogueUuid;
        char dialogueUuidStr[CASS_UUID_STRING_LENGTH];
        const CassValue* dialogueId = cass_row_get_column_by_name(currRow, "dialogue_id");
        cass_value_get_uuid(dialogueId, &dialogueUuid);
        cass_uuid_string(dialogueUuid, dialogueUuidStr);

        dialoguesId.push_back(dialogueUuidStr);
    }

    cass_result_free(result);
    cass_iterator_free(dialogues_iterator);
    cass_statement_free(searchDialoguesSt);
    cass_future_free(searchDialoguesSt_future);

    return dialoguesId;
}

std::vector<std::string> MainDb::getLastNDialoguesIdByLogin(std::string login, long count) const {
    std::vector <std::string> dialoguesId;
    const char* searchDialogues = "SELECT dialogue_id FROM maindb.user_dialogues "
                                  "WHERE login = ? LIMIT ?;";
    CassStatement* searchDialoguesSt = cass_statement_new(searchDialogues, 2);
    cass_statement_bind_string(searchDialoguesSt, 0, login.data());
    cass_statement_bind_int32(searchDialoguesSt, 1, count);
    CassFuture* searchDialoguesSt_future = cass_session_execute(session_, searchDialoguesSt);

    CassError rc = cass_future_error_code(searchDialoguesSt_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(searchDialoguesSt_future, &message, &message_length);
        fprintf(stderr, "St7 error: '%.*s'\n", (int)message_length, message);
    }

    const CassResult* result = cass_future_get_result(searchDialoguesSt_future);
    if (result == NULL) {
        cass_statement_free(searchDialoguesSt);
        cass_future_free(searchDialoguesSt_future);
        return dialoguesId;  // throw exception
    }

    const CassRow* row = cass_result_first_row(result);
    if (row == NULL) {  // user doesn't exist or password is incorrect
        cass_result_free(result);
        cass_statement_free(searchDialoguesSt);
        cass_future_free(searchDialoguesSt_future);
        return dialoguesId;  // throw exception
    }


    CassIterator* dialogues_iterator = cass_iterator_from_result(result);
    while (cass_iterator_next(dialogues_iterator)) {
        const CassRow* currRow = cass_iterator_get_row(dialogues_iterator);

        CassUuid dialogueUuid;
        char dialogueUuidStr[CASS_UUID_STRING_LENGTH];
        const CassValue* dialogueId = cass_row_get_column_by_name(currRow, "dialogue_id");
        cass_value_get_uuid(dialogueId, &dialogueUuid);
        cass_uuid_string(dialogueUuid, dialogueUuidStr);

        dialoguesId.emplace_back(dialogueUuidStr);
    }

    cass_result_free(result);
    cass_iterator_free(dialogues_iterator);
    cass_statement_free(searchDialoguesSt);
    cass_future_free(searchDialoguesSt_future);

    return dialoguesId;
}

std::vector<std::string> MainDb::getParticipantsLoginsFromDialogue(std::string dialogueId) const {
    std::vector<std::string> participantsLogins;
    const char* searchUser = "SELECT participants_logins FROM maindb.dialogues_by_id "
                             "WHERE dialogue_id = ? LIMIT 1;";
    CassStatement* returnedUser = cass_statement_new(searchUser, 1);
    CassUuid uuid;
    cass_uuid_from_string(dialogueId.data(), &uuid);
    cass_statement_bind_uuid(returnedUser, 0, uuid);
    CassFuture* returnedUser_future = cass_session_execute(session_, returnedUser);

    CassError rc = cass_future_error_code(returnedUser_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(returnedUser_future, &message, &message_length);
        fprintf(stderr, "St8 error: '%.*s'\n", (int)message_length, message);
    }

    const CassResult* result = cass_future_get_result(returnedUser_future);
    if (result == NULL) {  // throw exception
        cass_statement_free(returnedUser);
        cass_future_free(returnedUser_future);
        return participantsLogins;
    }

    const CassRow* row = cass_result_first_row(result);
    if (row == NULL) {  // exception
        cass_result_free(result);
        cass_statement_free(returnedUser);
        cass_future_free(returnedUser_future);
        return participantsLogins;
    }

    const CassValue* dialoguesColumn = cass_row_get_column_by_name(row, "participants_logins");

    CassIterator* dialoguesIterator = cass_iterator_from_collection(dialoguesColumn);
    while (cass_iterator_next(dialoguesIterator)) {
        const CassValue* currentDialogue = cass_iterator_get_value(dialoguesIterator);

        const char* dialogueStr;
        size_t dialogueStrLength;
        cass_value_get_string(currentDialogue, &dialogueStr, &dialogueStrLength);
        std::string str = dialogueStr;
        str = str.substr(0, dialogueStrLength);
    
        participantsLogins.push_back(str);
    }
    return participantsLogins;
}

time_t MainDb::getTimeLastUpdateFromDialogue(std::string dialogueId, const User& user) const {
    const char* getTime = "SELECT time_update FROM maindb.user_dialogues "
                          "WHERE login = ? AND dialogue_id = ? LIMIT 1 ALLOW FILTERING;";
    CassStatement* getTimeSt = cass_statement_new(getTime, 2);
    CassUuid uuid;
    cass_uuid_from_string(dialogueId.data(), &uuid);
    cass_statement_bind_uuid(getTimeSt, 1, uuid);
    cass_statement_bind_string(getTimeSt, 0, user.getLogin().data());

    CassFuture* getTimeSt_future = cass_session_execute(session_, getTimeSt);

    CassError rc = cass_future_error_code(getTimeSt_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(getTimeSt_future, &message, &message_length);
        fprintf(stderr, "St11 error: '%.*s'\n", (int)message_length, message);
    }

    const CassResult* result = cass_future_get_result(getTimeSt_future);
    if (result == nullptr) {  // throw exception
        cass_statement_free(getTimeSt);
        cass_future_free(getTimeSt_future);
        return 0;
    }

    const CassRow* row = cass_result_first_row(result);
    if (row == nullptr) {  // exception
        cass_result_free(result);
        cass_statement_free(getTimeSt);
        cass_future_free(getTimeSt_future);
        return 0;
    }

    const CassValue* time = cass_row_get_column_by_name(row, "time_update");
    time_t returnTime = 0;
    cass_value_get_int64(time, &returnTime);

    return returnTime;
}

DialogueList MainDb::getLastNDialoguesWithLastMessage(const User& user, long count) const {
    std::vector<std::string> dialoguesId = getLastNDialoguesIdByLogin(user.getLogin(), count);
    DialogueList dialogues; // select * from user_dialogues

    for (auto & i : dialoguesId) {
        std::vector<Message> messagesFromCurrDialogue = getNLastMessagesFromDialogue(i, 1);
        std::vector<std::string> participantsList = getParticipantsLoginsFromDialogue(i);
        time_t time = getTimeLastUpdateFromDialogue(i, user);
        Dialogue currDialogue(i, messagesFromCurrDialogue, participantsList, time);
        dialogues.emplace(currDialogue);
    }

    return dialogues;
}

std::string MainDb::findDialogue(std::vector<std::string> participantsList) const {
    const char* searchDialogue = "SELECT * FROM maindb.dialogues_by_id WHERE paricipants_logins CONTAINS ? AND "
                                 "paricipants_logins CONTAINS ? LIMIT 1 ALLOW FILTERING;";
    CassStatement* searchDialogueSt = cass_statement_new(searchDialogue, 2);
    cass_statement_bind_string(searchDialogueSt, 0, participantsList[0].data());
    cass_statement_bind_string(searchDialogueSt, 1, participantsList[1].data());

    CassFuture* searchDialogueSt_future = cass_session_execute(session_, searchDialogueSt);

    CassError rc = cass_future_error_code(searchDialogueSt_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(searchDialogueSt_future, &message, &message_length);
        fprintf(stderr, "St9 error: '%.*s'\n", (int)message_length, message);
    }

    const CassResult* result = cass_future_get_result(searchDialogueSt_future);
    if (result == nullptr) {  // throw exception
        cass_statement_free(searchDialogueSt);
        cass_future_free(searchDialogueSt_future);
        return "";
    }

    const CassRow* row = cass_result_first_row(result);
    if (row == nullptr) {  // exception
        cass_result_free(result);
        cass_statement_free(searchDialogueSt);
        cass_future_free(searchDialogueSt_future);
        return "";
    }

    CassUuid dialogueUuid;
    char dialogueUuidStr[CASS_UUID_STRING_LENGTH];
    const CassValue* dialogueId = cass_row_get_column_by_name(row, "dialogue_id");
    cass_value_get_uuid(dialogueId, &dialogueUuid);
    cass_uuid_string(dialogueUuid, dialogueUuidStr);

    return dialogueUuidStr;
}



void MainDb::createDialogue(Dialogue& dialogue) {
//    std::string existingUuid = findDialogue(participantsList);
//    if (existingUuid != "") {
//        std::vector<Message> messages;
//        return Dialogue(existingUuid, messages, participantsList);
//    }
    const char* newDialogue = "INSERT INTO maindb.dialogues_by_id (dialogue_id, participants_logins) "
                              "VALUES (?, ?);";
    CassStatement* newDialogueSt = cass_statement_new(newDialogue, 2);
    CassUuidGen* uuidGen = cass_uuid_gen_new();
    CassUuid uuid;

    char uuidStr[CASS_UUID_STRING_LENGTH];

    cass_uuid_gen_random(uuidGen, &uuid);
    cass_uuid_gen_free(uuidGen);
    cass_uuid_string(uuid, uuidStr);

    dialogue.setId(uuidStr);

    cass_statement_bind_uuid(newDialogueSt, 0, uuid);

    CassCollection* set = cass_collection_new(CASS_COLLECTION_TYPE_SET, 2);

    for (auto & i : dialogue.getParticipantsList()) {
        cass_collection_append_string(set, i.data());
    }
    cass_statement_bind_collection(newDialogueSt, 1, set);

    CassFuture* newDialogueSt_future = cass_session_execute(session_, newDialogueSt);

    CassError rc = cass_future_error_code(newDialogueSt_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(newDialogueSt_future, &message, &message_length);
        fprintf(stderr, "St2 error: '%.*s'\n", (int)message_length, message);
    }

    const char* newDToUserDialogue = "INSERT INTO maindb.user_dialogues (login, dialogue_id, time_update) "
                                             "VALUES (?, ?, ?);";

    for (size_t i = 0; i < dialogue.getParticipantsList().size(); i++) {
        CassStatement* newDToUserDialogueSt = cass_statement_new(newDToUserDialogue, 3);
        cass_statement_bind_string(newDToUserDialogueSt, 0, dialogue.getParticipantsList()[i].data());
        cass_statement_bind_uuid(newDToUserDialogueSt, 1, uuid);
        cass_statement_bind_int64(newDToUserDialogueSt, 2, dialogue.getTimeOfLastUpdate());
        CassFuture* newDToUserDialogueSt_future = cass_session_execute(session_, newDToUserDialogueSt);
        CassError rc1 = cass_future_error_code(newDToUserDialogueSt_future);
        if (rc1 != CASS_OK) {
            /* Display connection error message */
            const char* message;
            size_t message_length;
            cass_future_error_message(newDToUserDialogueSt_future, &message, &message_length);
            fprintf(stderr, "St10 error: '%.*s'\n", (int)message_length, message);
        }
    }
}


void MainDb::changePaginatedMessages(CassStatement*& statement, int amount, 
                                     paginatedMessages& messages) {
    if (!statement) {
        fprintf(stderr, "Statement is empty!!\n");
        return;
        // throw exception
    }
    if (amount <= 0) {
        // throw exception
        return;
    }

    cass_statement_set_paging_size(statement, amount);
    
    if (!messages.pagingState) {
        CassFuture* query_future = cass_session_execute(session_, statement);

        const CassResult* result = cass_future_get_result(query_future);

        if (result == nullptr) {
            cass_future_free(query_future);
            return;
        }

        fillMessagesFromResult(messages.messages, result);
        messages.pagingState = result;
        return;
    }

    cass_bool_t has_more_pages = cass_result_has_more_pages(messages.pagingState);

    if (has_more_pages) {
        CassFuture* query_future = cass_session_execute(session_, statement);

        const CassResult* result = cass_future_get_result(query_future);

        if (result == NULL) {
            cass_future_free(query_future);
            return;
        }
        fillMessagesFromResult(messages.messages, result);

        cass_statement_set_paging_state(statement, result);

        cass_future_free(query_future);
        cass_result_free(messages.pagingState);
        messages.pagingState = result;
    } else {
        return;
    }
}

void MainDb::deleteMessage(Message& message) {
}

void MainDb::deleteDialogue(Dialogue& dialogue) {
}
