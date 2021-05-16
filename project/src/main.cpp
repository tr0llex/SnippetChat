#include <iostream>
#include <string>
#include "Models.hpp"
#include "MainDb.hpp"

int main() {
    MainDb db;
    CassUuidGen* uuid_gen = cass_uuid_gen_new();
    CassUuid uuid, puuid, suuid;
    char uuid_str[CASS_UUID_STRING_LENGTH];
    char puuid_str[CASS_UUID_STRING_LENGTH];
    char suuid_str[CASS_UUID_STRING_LENGTH];
     cass_uuid_gen_random(uuid_gen, &uuid);
     cass_uuid_gen_random(uuid_gen, &puuid);
     cass_uuid_gen_random(uuid_gen, &suuid);
     cass_uuid_gen_free(uuid_gen);
     cass_uuid_string(uuid, uuid_str);
     cass_uuid_string(puuid, puuid_str);
     cass_uuid_string(suuid, suuid_str);

    Message message(uuid_str, "25f9e3e4-e1ba-4ae2-b8d8-35b2617e5482", suuid_str, "iuuas3", "cout << shhh", 323, false);
    db.connectToDb("127.0.0.1");
    // std::string out = db.getCodeFromMessage("c033e0ba-aaaf-4ea1-94f7-301954e9d023");
    // db.writeMessage(message);
    // db.writeMessage(message);
    // db.writeMessage(message);
    std::vector <Message>* messages = db.getNMessagesFromDialogue("25f9e3e4-e1ba-4ae2-b8d8-35b2617e5482", 5);
    if (messages) {
        for (const auto &message: *messages) {
            std::cout << message.getMessageId() << std::endl;
        }
    }

    // std::cout << out << std::endl;
    db.disconnectFromDb();
    return 0;
}
