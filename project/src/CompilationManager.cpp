#include <climits>
#include <algorithm>
#include <iostream>

#include "CompilationManager.hpp"

CompilationManager::CompilationManager() : compilesCount(0) {
    removeOldDirs();
}

Compilation CompilationManager::runCompilation(const Snippet &snippet, const string &executionStdin) {
    int language = snippet.getLanguage();
    std::string messageCode = snippet.getProgramText();

    compilesCount++;
    if (compilesCount == ULLONG_MAX) {
        removeOldDirs();
        compilesCount = 0;
    }

    Compilation currentCompilation;
    currentCompilation.setMessageCode(
            snippet.getProgramText());
    currentCompilation.setExecutionStdin(executionStdin);
    currentCompilation.setCompilationId(compilesCount);
    currentCompilation.setFileExtension(languagesExtensions[language]);
    currentCompilation.setPathToRunBox(
            "compilers/" + languagesNames[language] + "/runs/run-" + std::to_string(compilesCount));
    currentCompilation.setPathToTemplate("compilers/" + languagesNames[language] + "/template");

    createDir(currentCompilation);
    run(currentCompilation);
    readOutputFromFiles(currentCompilation);

    return currentCompilation;
}

void CompilationManager::writeInputToFiles(const Compilation &compilation, const string &code, const string &input) {
    std::string pathToRunBox = compilation.getPathToRunBox();

    std::ofstream sourceFile(pathToRunBox + "/source." + compilation.getFileExtension());
    sourceFile << code;
    sourceFile.close();

    std::ofstream stdinFile(pathToRunBox + "/run.stdin");
    stdinFile << input;
    stdinFile.close();
}

void CompilationManager::run(const Compilation &compilation) {
    std::string code = compilation.getMessageCode();
    std::string input = compilation.getExecutionStdin();
    std::string pathToRunBox = compilation.getPathToRunBox();

    writeInputToFiles(compilation, code, input);

    std::string sCommandRun = "bash " + pathToRunBox + "/run.sh >" + pathToRunBox + "/log.txt";
    const char *pcharCommandRun = sCommandRun.c_str();
    system(pcharCommandRun);
}

void CompilationManager::removeOldDirs() {
    system("rm -rf compilers/python3/runs/run-*");
    system("rm -rf compilers/c/runs/run-*");
    system("rm -rf compilers/cpp/runs/run-*");
    system("rm -rf compilers/cpp20/runs/run-*");
    system("rm -rf compilers/runs");
}

void CompilationManager::createDir(const Compilation &compilation) {
    compilesCount++;

    std::string pathToRunBox = compilation.getPathToRunBox();

    std::string sCommandMkdir = "mkdir -p " + pathToRunBox;
    const char *pcharCommandMkdir = sCommandMkdir.c_str();
    system(pcharCommandMkdir);

    std::string sCommandCopy = "cp -R " + compilation.getPathToTemplate() + "/. " + pathToRunBox + "/";
    const char *pcharCommandCopy = sCommandCopy.c_str();
    system(pcharCommandCopy);
}

bool fileExists(const std::string &name) {
    std::ifstream f(name.c_str());
    return f.good();
}

void CompilationManager::readOutputFromFiles(Compilation &compilation) {
    std::string pathToRunBox = compilation.getPathToRunBox();

    std::ifstream outputFile(pathToRunBox + "/run.stdout");
    std::string executionOutput;

    char c;
    const int outputLimit = 5000;
    int symbolsWritten = 0;
    if (outputFile.is_open()) {
        while (outputFile.good()) {
            symbolsWritten += 1;
            outputFile.get(c);
            executionOutput += c;
            if (symbolsWritten == outputLimit)
                break;
        }
    }
    if (!outputFile.eof() && outputFile.fail())
        std::cout << "error reading " << pathToRunBox + "/run.stdout" << std::endl;
    outputFile.close();
    compilation.setExecutionStdout(executionOutput);

    std::ifstream runStderrFile(pathToRunBox + "/run.stderr");
    std::string executionError;
    executionError.assign((std::istreambuf_iterator<char>(runStderrFile)),
                          (std::istreambuf_iterator<char>()));
    compilation.setExecutionStderr(executionError);

    std::ifstream compileStdoutFile(pathToRunBox + "/compile.stdout");
    std::string compileOutput;
    compileOutput.assign((std::istreambuf_iterator<char>(compileStdoutFile)),
                         (std::istreambuf_iterator<char>()));
    compilation.setCompilerStdout(compileOutput);

    std::ifstream compileStderrFile(pathToRunBox + "/compile.stderr");
    std::string compileError;
    compileError.assign((std::istreambuf_iterator<char>(compileStderrFile)),
                        (std::istreambuf_iterator<char>()));
    compilation.setCompilerStderr(compileError);

    std::string time;
    std::string memory;
    std::ifstream logFile(pathToRunBox + "/time.log");
    std::getline(logFile, time);
    std::getline(logFile, memory);
    compilation.setExecutionTime(time);
    compilation.setExecutionUsedMemory(memory);

    if (fileExists(pathToRunBox + "/tle.stderr")) {
        compilation.setTimeLimitExceeded(true);
    } else {
        compilation.setTimeLimitExceeded(false);
    }

}
