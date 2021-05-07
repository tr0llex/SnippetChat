#include <vector>
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
    "participants_id list<int>"
    ")"
    ";";
    const char* messagesByIdTQ = "CREATE TABLE IF NOT EXISTS mainDB.messages_by_id ("
    "message_id uuid PRIMARY KEY,"
    "dialogue_id int,"
    "sender_id int,"
    "message_text text,"
    "message_code text,"
    "time_sent timestamp,"
    "is_read boolean"
    ")"
    ";";


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

    if (result == NULL) {  // user doesn't exist or password is incorrect
        return new User();
    }

    const CassRow* row = cass_result_first_row(result);
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

std::string MainDb::getCodeFromMessage(uint32_t messageId) {
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
