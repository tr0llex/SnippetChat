#include "Models.hpp"
#include "Auth.hpp"

int main() {
    /*InMemoryDb imd;
    std::cout <<imd.getUserName("AAA") << "!"<<std::endl;
    std::cout <<imd.searchToken("AAA") << "!"<<std::endl;
    std::cout <<imd.updateUserStatus(1, -1) << "!"<<std::endl;
    std::vector<bool> a = imd.getOnline({1,2,3});
    for (bool i : a)
        std::cout << i << "!"<<std::endl;
    std::cout <<imd.writeInMemory(4, "created_by_C", "BBB", 1) << "!"<<std::endl;
    std::cout <<imd.searchToken("BBC") << "!"<<std::endl;
    TokenManager tman;
    std::cout <<tman.generateToken(4) <<std::endl;*/
    Auth ServAuth;
    ServAuth.logoutUser("admin");
    LoginData ldt;
    ldt.setLogin("created_by_C");
    ldt.setPassword("BBB");
    ldt.setType(-1);
    std::cout << ServAuth.loginUser(ldt)<< std::endl;
    std::cout << ServAuth.registerUser("MeMeMe", "pass")<<std::endl;
    std::cout << ServAuth.verifyToken("BBQ")<<std::endl;
    std::cout << ServAuth.verifyToken("miss")<<std::endl;


    return 0;
}
