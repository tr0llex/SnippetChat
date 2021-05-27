#ifndef PROJECT_INCLUDE_COMPILATIONMANAGER_HPP_
#define PROJECT_INCLUDE_COMPILATIONMANAGER_HPP_

#include <fstream>
#include <sstream>

#include "Compilation.hpp"
//#include "CompilerDb.hpp"
//#include "CompilerResultWriter.hpp"

using std::string;

class ICompilationManager {
public:
    virtual Compilation runCompilation(const string &messageCode, const string &executionStdin, int language) = 0;
};

class CompilationManager : ICompilationManager {
private:

    std::string languagesNames[5] = {"", "python3", "cpp", "cpp20" "c"};
    std::string languagesExtensions[5] = {"", "py", "cpp", "cpp" "c"};

    static void writeInputToFiles(const Compilation &compilation, const string &code, const string &input);

    static void run(const Compilation &compilation, const string &code, const string &input);

    static void removeOldDirs();

    void createDir(const Compilation &compilation);

    static void readOutputFromFiles(Compilation &compilation);

    //std::string getCodeFromDb(uint32_t messageId); TODO

    //void logCompilaton(Compilation *compilation); TODO

    unsigned long long compilesCount{};

    string defaultPathToTemplate = "/test/template"; // add language in start

public:
    CompilationManager();

    ~CompilationManager() = default;

    Compilation runCompilation(const string &messageCode, const string &executionStdin, int language) override;
};

#endif  // PROJECT_INCLUDE_COMPILATIONMANAGER_HPP_
