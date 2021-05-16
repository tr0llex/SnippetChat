#include <iostream>
#include <string>
#include "Models.hpp"
#include "MainDb.hpp"

int main() {
    MainDb db;

    db.connectToDb("127.0.0.1");

    // std::cout << out << std::endl;
    db.disconnectFromDb();
    return 0;
}
