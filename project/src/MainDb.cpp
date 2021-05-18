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
    CassFuture* usersByIdSt_future = cass_session_execute(session_, usersByIdSt);

    CassStatement* dialoguesByIdSt = cass_statement_new(dialoguesByIdTQ, 0);  // made statement
    CassFuture* dialoguesByIdSt_future = cass_session_execute(session_, dialoguesByIdSt);

    CassStatement* userDialoguesSt = cass_statement_new(userDialoguesTQ, 0);  // made statement
    CassFuture* userDialoguesSt_future = cass_session_execute(session_, userDialoguesSt);

    CassStatement* messagesByIdSt = cass_statement_new(messagesByIdTQ, 0);  // made statement
    CassFuture* messagesByIdSt_future = cass_session_execute(session_, messagesByIdSt);

    CassStatement* messagesByIdIndForDialogSt = cass_statement_new(messageByIdIndForDialog, 0);  // made statement
    CassFuture* messagesByIdIndForDialogSt_future = cass_session_execute(session_, messagesByIdIndForDialogSt);


    cass_statement_free(keyspaceSt);
    cass_statement_free(messagesByIdSt);
    cass_statement_free(dialoguesByIdSt);
    cass_statement_free(userDialoguesSt);
    cass_statement_free(usersByIdSt);
    cass_statement_free(messagesByIdIndForDialogSt);

    cass_future_free(ks_future);
    cass_future_free(messagesByIdSt_future);
    cass_future_free(dialoguesByIdSt_future);
    cass_future_free(userDialoguesSt_future);
    cass_future_free(usersByIdSt_future);
    cass_future_free(messagesByIdIndForDialogSt_future);
}
    
void MainDb::disconnectFromDb() {
    close_future_ = cass_session_close(session_);
    cass_future_wait(close_future_);
    cass_future_free(close_future_);
}

User* MainDb::searchUserPassword(std::string login, std::string password) {  // function pull dialogues_list
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
        fprintf(stderr, "St error: '%.*s'\n", (int)message_length, message);
    }

    const CassResult* result = cass_future_get_result(returnedUser_future);
    if (result == NULL) {  // throw exception
        cass_statement_free(returnedUser);
        cass_future_free(returnedUser_future);
        return nullptr;
    }

    const CassRow* row = cass_result_first_row(result);
    if (row == NULL) {  // user doesn't exist
        cass_result_free(result);
        cass_statement_free(returnedUser);
        cass_future_free(returnedUser_future);
        return nullptr;
    }
    const CassValue* passwordColumn = cass_row_get_column_by_name(row, "password");
    const char* passwordStr;
    size_t passwordStrLength;
    cass_value_get_string(passwordColumn, &passwordStr, &passwordStrLength);
    if (passwordStr != login) {  // password is incorrect
        return nullptr;
    }

    cass_result_free(result);
    cass_statement_free(returnedUser);
    cass_future_free(returnedUser_future);

    return new User(login, password, "", 1);
}

bool MainDb::searchUserLogin(std::string login) {  // function pull dialogues_list
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
        fprintf(stderr, "St error: '%.*s'\n", (int)message_length, message);
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
    if (!searchUserLogin(user.getLogin())) {
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

void MainDb::writeMessage(Message& message) {
    // exceptions if message is not full filled
    // if (message.getDialogueParentId() == "" || message.getMessageId() == "" 
    //     || message.getSenderLogin() == "" || message.getTimeSent() == 0) {
    //     throw(-2);
    // }
    const char* insertMessage = "INSERT INTO maindb.messages_by_id " 
    "(message_id, dialogue_id, sender_id, message_text, message_code, time_sent, is_read) VALUES "
    "(?, ?, ?, ?, ?, ?, ?)"
    ";";
    CassUuid uuid;
    CassStatement* insertMessageSt = cass_statement_new(insertMessage, 7);
    cass_uuid_from_string(message.getId().data(), &uuid);
    cass_statement_bind_uuid(insertMessageSt, 0, uuid);

    cass_uuid_from_string(message.getDialogueParentId().data(), &uuid);
    cass_statement_bind_uuid(insertMessageSt, 1, uuid);

    cass_uuid_from_string(message.getSenderLogin().data(), &uuid);
    cass_statement_bind_uuid(insertMessageSt, 2, uuid);

    cass_statement_bind_string(insertMessageSt, 3, message.getMessageText().data());
    cass_statement_bind_string(insertMessageSt, 4, message.getMessageCode().data());
    cass_statement_bind_int64(insertMessageSt, 5, message.getTimeSent());
    cass_statement_bind_bool(insertMessageSt, 6, (cass_bool_t)message.isRead());

    CassFuture* insertMessageSt_future = cass_session_execute(session_, insertMessageSt);

    const char* getTimeUpdate = "SELECT time_update FROM mainDB.user_dialogues "
                                "WHERE dialogue_id = ?;";

    CassStatement* getTimeUpdateSt = cass_statement_new(getTimeUpdate, 1);
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

    cass_statement_free(insertMessageSt);
    cass_statement_free(getTimeUpdateSt);
    cass_future_free(insertMessageSt_future);
    cass_future_free(getTimeUpdateSt_future);
}

std::vector<Message> MainDb::getNLastMessagesFromDialogue(std::string dialogueId, long count) const {
    std::vector<Message> messages(count);
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
        fprintf(stderr, "St error: '%.*s'\n", (int)message_length, message);
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

    CassIterator* messages_iterator = cass_iterator_from_result(result);

    long i = count - 1;
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

        CassUuid senderUuid;
        char senderUuidStr[CASS_UUID_STRING_LENGTH];
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

        Message newMessage(messageUuidStr, dialogueUuidStr,
                           senderUuidStr, messageTextStr,
                           messageCodeStr, messageTimeT, isReadB);

    
        messages[i] = newMessage;
        i--;
    }

    cass_result_free(result);
    cass_iterator_free(messages_iterator);
    cass_statement_free(getNMessagesSt);
    cass_future_free(getNMessagesSt_future);

    return messages;
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
        fprintf(stderr, "St error: '%.*s'\n", (int)message_length, message);
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
        const CassRow* сurrRow = cass_iterator_get_row(dialogues_iterator);

        CassUuid dialogueUuid;
        char dialogueUuidStr[CASS_UUID_STRING_LENGTH];
        const CassValue* dialogueId = cass_row_get_column_by_name(сurrRow, "dialogue_id");
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
                                  "WHERE login = ? LIMIT = ?;";
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
        fprintf(stderr, "St error: '%.*s'\n", (int)message_length, message);
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
        const CassRow* сurrRow = cass_iterator_get_row(dialogues_iterator);

        CassUuid dialogueUuid;
        char dialogueUuidStr[CASS_UUID_STRING_LENGTH];
        const CassValue* dialogueId = cass_row_get_column_by_name(сurrRow, "dialogue_id");
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

std::vector<std::string> MainDb::getParticipantsLoginsFromDialogue(std::string dialogueId) const {
    std::vector<std::string> participantsLogins;
    const char* searchUser = "SELECT participants_logins FROM maindb.dialogues_by_id "
                             "WHERE dialogue_id = ? LIMIT 1;";
    CassStatement* returnedUser = cass_statement_new(searchUser, 1);
    cass_statement_bind_string(returnedUser, 0, dialogueId.data());
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
    
        participantsLogins.push_back(dialogueStr);
    }
    return participantsLogins;
}

DialogueList MainDb::getLastNDialoguesWithLastMessage(const User& user, long count) const {
    std::vector<std::string> dialoguesId = getLastNDialoguesIdByLogin(user.getLogin(), count);
    DialogueList dialogues;

    for (int i = 0; i < dialoguesId.size(); i++) {
        std::vector<Message> messagesFromCurrDialogue = getNLastMessagesFromDialogue(dialoguesId[i], 1);
        std::vector<std::string> participantsList = getParticipantsLoginsFromDialogue(dialoguesId[i]);
        Dialogue currDialogue(dialoguesId[i], participantsList, messagesFromCurrDialogue);
        dialogues.emplace(currDialogue);
    }

    return dialogues;
}


Dialogue MainDb::createDialogue(std::string firstId, std::string secondId) {
    const char* newDialogue = "INSERT INTO maindb.dialogues_by_id (dialogue_id, participants_logins) "
                              "VALUES (?, ?);";
    CassStatement* newDialogueSt = cass_statement_new(newDialogue, 2);
    CassUuidGen* uuidGen = cass_uuid_gen_new();
    CassUuid uuid;

    char uuidStr[CASS_UUID_STRING_LENGTH];

    cass_uuid_gen_random(uuidGen, &uuid);
    cass_uuid_gen_free(uuidGen);
    cass_uuid_string(uuid, uuidStr);

    cass_statement_bind_uuid(newDialogueSt, 0, uuid);

    CassCollection* set = cass_collection_new(CASS_COLLECTION_TYPE_SET, 2);

    cass_collection_append_string(set, firstId.data());
    cass_collection_append_string(set, secondId.data());
    cass_statement_bind_collection(newDialogueSt, 1, set);

    CassFuture* newDialogueSt_future = cass_session_execute(session_, newDialogueSt);

    CassError rc = cass_future_error_code(newDialogueSt_future);

    if (rc != CASS_OK) {
        /* Display connection error message */
        const char* message;
        size_t message_length;
        cass_future_error_message(newDialogueSt_future, &message, &message_length);
        fprintf(stderr, "St error: '%.*s'\n", (int)message_length, message);
    }
    std::vector<Message> messages;
    std::vector<std::string> participants;

    participants.push_back(firstId);
    if (firstId != secondId) {
        participants.push_back(secondId);
    }

    return Dialogue(uuidStr, participants, messages);
}

void MainDb::deleteMessage(Message& message) {
}

void MainDb::deleteDialogue(Dialogue& dialogue) {
}
