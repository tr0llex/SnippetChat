#include "../include/Compilation.hpp"

uint32_t Compilation::getCompilationId() const {
    return compilationId_;
}

void Compilation::setCompilationId(const uint32_t &compilationId) {
    compilationId_ = compilationId;
}

uint32_t Compilation::getMessageId() const {
    return messageId_;
}

void Compilation::setMessageId(const uint32_t &messageId) {
    messageId_ = messageId;
}

std::string Compilation::getMessageCode() const {
    return messageCode_;
}

void Compilation::setMessageCode(const std::string &messageCode) {
    messageCode_ = messageCode;
}

//std::string Compilation::getCompilerStderr() const {
//    return compilerStderr_;
//}

//void Compilation::setCompilerStderr(const std::string &compilerStderr) {
//    compilerStderr_ = compilerStderr;
//}

//std::string Compilation::getCompilerStdout() const {
//    return compilerStdout_;
//}

//void Compilation::setCompilerStdout(const std::string &compilerStdout) {
//    compilerStdout_ = compilerStdout;
//}

std::string Compilation::getExecutionStderr() const {
    return executionStderr_;
}

void Compilation::setExecutionStderr(const std::string &executionStderr) {
    executionStderr_ = executionStderr;
}

std::string Compilation::getExecutionStdin() const {
    return executionStdin_;
}

void Compilation::setExecutionStdin(const std::string &executionStdin) {
    executionStdin_ = executionStdin;
}

std::string Compilation::getExecutionStdout() const {
    return executionStdout_;
}

void Compilation::setExecutionStdout(const std::string &executionStdout) {
    executionStdout_ = executionStdout;
}

std::string Compilation::getExecutionUsedMemory() const {
    return executionUsedMemory_;
}

void Compilation::setExecutionUsedMemory(const std::string &executionUsedMemory) {
    executionUsedMemory_ = executionUsedMemory;
}

std::string Compilation::getExecutionTime() const {
    return executionTime_;
}

void Compilation::setExecutionTime(const std::string &executionTime) {
    executionTime_ = executionTime;
}

std::string Compilation::getPathToRunBox() const {
    return pathToRunBox_;
}

void Compilation::setPathToRunBox(const std::string &pathToRunBox) {
    pathToRunBox_ = pathToRunBox;
}
