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
    virtual Compilation runCompilation(const string &messageCode, const string &executionStdin) = 0;
};

class CompilationManager : ICompilationManager {
private:
    /*enum language {
        NOT_SELECTED = 0,
        PYTHON3 = 1,
        C = 2,
        CPP = 3,
    };*/

    static void writeInputToFiles(const Compilation &compilation, const string &code, const string &input);

    static void run(const Compilation &compilation, const string &code, const string &input);

    static void removeOldDirs();

    void createDir(const Compilation &compilation);

    static void readOutputFromFiles(Compilation &compilation);

    //std::string getCodeFromDb(uint32_t messageId); TODO

    //void logCompilaton(Compilation *compilation); TODO

    unsigned long long compilesCount{};

    string pathToTemplate = "../python3/test/template";

public:
    CompilationManager();

    ~CompilationManager() = default;

    Compilation runCompilation(const string &messageCode, const string &executionStdin) override;
};

#endif  // PROJECT_INCLUDE_COMPILATIONMANAGER_HPP_
