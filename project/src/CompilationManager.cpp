#include "CompilationManager.hpp"

CompilationManager::CompilationManager() : compilesCount(0) {
    removeOldDirs();
}

Compilation CompilationManager::runCompilation(const string &messageCode, const string &executionStdin) {
    compilesCount++;
    Compilation currentCompilation;
    currentCompilation.setCompilationId(compilesCount);
    currentCompilation.setPathToRunBox("python3/test/run-" + std::to_string(compilesCount));

    createDir(currentCompilation);
    run(currentCompilation, messageCode, executionStdin);
    readOutputFromFiles(currentCompilation);

    return currentCompilation;
}

void CompilationManager::writeInputToFiles(const Compilation &compilation, const string &code, const string &input) {
    string pathToRunBox = compilation.getPathToRunBox();

    std::ofstream sourceFile(pathToRunBox + "/source.py");
    sourceFile << code;
    sourceFile.close();

    std::ofstream stdinFile(pathToRunBox + "/run.stdin");
    stdinFile << input;
    stdinFile.close();
}

void CompilationManager::run(const Compilation &compilation, const string &code, const string &input) {
    string pathToRunBox = compilation.getPathToRunBox();

    writeInputToFiles(compilation, code, input);

    string sCommandRun = "bash " + pathToRunBox + "/run.sh >" + pathToRunBox + "/log.txt";
    const char *pcharCommandRun = sCommandRun.c_str();
    system(pcharCommandRun);
}

void CompilationManager::removeOldDirs() {
    system("rm -rf python3/test/run-*");
}

void CompilationManager::createDir(const Compilation &compilation) {
    compilesCount++;

    string pathToRunBox = compilation.getPathToRunBox();

    string sCommandMkdir = "mkdir -p " + pathToRunBox;
    const char *pcharCommandMkdir = sCommandMkdir.c_str();
    system(pcharCommandMkdir);

    string sCommandCopy = "cp -R " + pathToTemplate + "/. " + pathToRunBox + "/";
    const char *pcharCommandCopy = sCommandCopy.c_str();
    system(pcharCommandCopy);
}

void CompilationManager::readOutputFromFiles(Compilation &compilation) {
    string pathToRunBox = compilation.getPathToRunBox();

    std::ifstream outputFile(pathToRunBox + "/run.stdout");
    string output;
    output.assign((std::istreambuf_iterator<char>(outputFile)),
                  (std::istreambuf_iterator<char>()));
    compilation.setExecutionStdout(output);

    std::ifstream runStderrFile(pathToRunBox + "/run.stderr");
    string error;
    error.assign((std::istreambuf_iterator<char>(runStderrFile)),
                 (std::istreambuf_iterator<char>()));
    compilation.setExecutionStderr(error);

    string time;
    string memory;
    std::ifstream logFile(pathToRunBox + "/time.log");
    std::getline(logFile, time);
    std::getline(logFile, memory);
    compilation.setExecutionTime(time);
    compilation.setExecutionUsedMemory(memory);
}
