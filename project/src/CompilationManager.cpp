#include "CompilationManager.hpp"
#include "Models.hpp"

CompilationManager::CompilationManager() : compilesCount(0) {
    removeOldDirs();
}

Compilation CompilationManager::runCompilation(const string &messageCode, const string &executionStdin, int language) {
    compilesCount++;
    Compilation currentCompilation;
    currentCompilation.setMessageCode(messageCode); // TODO: передавать только compilation, без code и input
    currentCompilation.setExecutionStdin(executionStdin);
    currentCompilation.setCompilationId(compilesCount);
    currentCompilation.setFileExtension(languagesExtensions[language]);
    currentCompilation.setPathToRunBox(
            "compilers/" + languagesNames[language] + "/runs/run-" + std::to_string(compilesCount));
    currentCompilation.setPathToTemplate("compilers/" + languagesNames[language] + "/template");

    createDir(currentCompilation);
    run(currentCompilation, messageCode, executionStdin);
    readOutputFromFiles(currentCompilation);

    return currentCompilation;
}

void CompilationManager::writeInputToFiles(const Compilation &compilation, const string &code, const string &input) {
    string pathToRunBox = compilation.getPathToRunBox();

    std::ofstream sourceFile(pathToRunBox + "/source." + compilation.getFileExtension());
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
    system("rm -rf compilers/python3/runs/run-*");
    system("rm -rf compilers/c/runs/run-*");
    system("rm -rf compilers/cpp/runs/run-*");
    system("rm -rf compilers/cpp20/runs/run-*");
}

void CompilationManager::createDir(const Compilation &compilation) {
    compilesCount++;

    string pathToRunBox = compilation.getPathToRunBox();

    string sCommandMkdir = "mkdir -p " + pathToRunBox;
    const char *pcharCommandMkdir = sCommandMkdir.c_str();
    system(pcharCommandMkdir);

    string sCommandCopy = "cp -R " + compilation.getPathToTemplate() + "/. " + pathToRunBox + "/";
    const char *pcharCommandCopy = sCommandCopy.c_str();
    system(pcharCommandCopy);
}

inline bool fileExists(const std::string &name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

void CompilationManager::readOutputFromFiles(Compilation &compilation) {
    string pathToRunBox = compilation.getPathToRunBox();

    std::ifstream outputFile(pathToRunBox + "/run.stdout");
    string executionOutput;
    executionOutput.assign((std::istreambuf_iterator<char>(outputFile)),
                           (std::istreambuf_iterator<char>()));
    compilation.setExecutionStdout(executionOutput);

    std::ifstream runStderrFile(pathToRunBox + "/run.stderr");
    string executionError;
    executionError.assign((std::istreambuf_iterator<char>(runStderrFile)),
                          (std::istreambuf_iterator<char>()));
    compilation.setExecutionStderr(executionError);

    std::ifstream compileStdoutFile(pathToRunBox + "/compile.stdout");
    string compileOutput;
    compileOutput.assign((std::istreambuf_iterator<char>(compileStdoutFile)),
                         (std::istreambuf_iterator<char>()));
    compilation.setCompilerStdout(compileOutput);

    std::ifstream compileStderrFile(pathToRunBox + "/compile.stderr");
    string compileError;
    compileError.assign((std::istreambuf_iterator<char>(compileStderrFile)),
                        (std::istreambuf_iterator<char>()));
    compilation.setCompilerStderr(compileError);

    if (fileExists(pathToRunBox + "tle.stderr")) {
        compilation.setTimeLimitExceeded(true);
    } else {
        compilation.setTimeLimitExceeded(false);
    }

    string time;
    string memory;
    std::ifstream logFile(pathToRunBox + "/time.log");
    std::getline(logFile, time);
    std::getline(logFile, memory);
    compilation.setExecutionTime(time);
    compilation.setExecutionUsedMemory(memory);

}
