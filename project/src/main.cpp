#include "ChatApplication.hpp"

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment &env, ChatServer &server) {
    return std::make_unique<ChatApplication>(env, server);
}

int main(int argc, char *argv[]) {
    Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);
    ChatServer chatServer(server);

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
