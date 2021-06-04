#include "ChatApplication.hpp"


int main(int argc, char *argv[]) {
    Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);

    MainDb db;
    db.connectToDb("127.0.0.1");

    ChatServer chatServer(server, db);

    server.addEntryPoint(Wt::EntryPointType::Application,
                         std::bind(createApplication, std::placeholders::_1,
                                   std::ref(chatServer)));

    if (server.start()) {
        int sig = Wt::WServer::waitForShutdown();
        std::cerr << "Shutting down: (signal = " << sig << ")" << std::endl;
        server.stop();
    }

    return 0;
}
