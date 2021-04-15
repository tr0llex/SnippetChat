#ifndef PROJECT_INCLUDE_COMPILATIONMANAGER_HPP_
#define PROJECT_INCLUDE_COMPILATIONMANAGER_HPP_

#include "Models.hpp"
#include "CompilerDb.hpp"
#include "CompilerResultWriter.hpp"

class CompilationManager {
private:
    CompilerDb compilerDb;
    uint32_t timeLimit;
    ResultWriter resultWriter;

    std::wstring getCodeFromDb(uint32_t messageId);

    void logCompilaton(Compilation *compilation);

public:
    Compilation runCompilation(uint32_t messageId, std::string executionStdin);
};

#endif  // PROJECT_INCLUDE_COMPILATIONMANAGER_HPP_
