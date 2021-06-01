#ifndef PROJECT_INCLUDE_COMPILATIONMANAGER_HPP_
#define PROJECT_INCLUDE_COMPILATIONMANAGER_HPP_

#include <fstream>
#include <sstream>
#include <map>

#include "Compilation.hpp"
#include "Models.hpp"

using std::string;

class ICompilationManager {
public:
    virtual Compilation runCompilation(const Snippet &snippet, const string &executionStdin) = 0;
};

class CompilationManager : ICompilationManager {
private:

    std::map<int, string> languagesNames = {{-1, ""},
                                            {0,  "python3"},
                                            {1,  "cpp14"},
                                            {2,  "cpp17"},
                                            {3,  "cpp20"},
                                            {4,  "c17"}};

    std::map<int, string> languagesExtensions = {{-1, ""},
                                                 {0,  "py"},
                                                 {1,  "cpp"},
                                                 {2,  "cpp"},
                                                 {3,  "cpp"},
                                                 {4,  "c"}};

    static void writeInputToFiles(const Compilation &compilation, const string &code, const string &input);

    static void run(const Compilation &compilation);

    static void removeOldDirs();

    void createDir(const Compilation &compilation);

    static void readOutputFromFiles(Compilation &compilation);

    //std::string getCodeFromDb(uint32_t messageId); TODO

    //void logCompilaton(Compilation *compilation); TODO

    unsigned long long compilesCount{};

public:
    CompilationManager();

    ~CompilationManager() = default;

    Compilation runCompilation(const Snippet &snippet, const string &executionStdin) override;
};

#endif  // PROJECT_INCLUDE_COMPILATIONMANAGER_HPP_
