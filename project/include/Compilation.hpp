#ifndef PROJECT_INCLUDE_COMPILATION_HPP_
#define PROJECT_INCLUDE_COMPILATION_HPP_

#include <cstdint>
#include <string>

class Compilation {
public:
    Compilation();

    ~Compilation() = default;

    uint32_t getCompilationId() const;

    void setCompilationId(const uint32_t &compilationId);

    uint32_t getMessageId() const;

    void setMessageId(const uint32_t &messageId);

    std::string getMessageCode() const;

    void setMessageCode(const std::string &messageCode);

    std::string getCompilerStderr() const;

    void setCompilerStderr(const std::string &compilerStderr);

    std::string getCompilerStdout() const;

    void setCompilerStdout(const std::string &compilerStdout);

    std::string getExecutionStderr() const;

    void setExecutionStderr(const std::string &executionStderr);

    std::string getExecutionStdin() const;

    void setExecutionStdin(const std::string &executionStdin);

    std::string getExecutionStdout() const;

    void setExecutionStdout(const std::string &executionStdout);

    std::string getExecutionUsedMemory() const;

    void setExecutionUsedMemory(const std::string &executionUsedMemory);

    std::string getExecutionTime() const;

    void setExecutionTime(const std::string &executionTime);

    std::string getPathToRunBox() const;

    void setPathToRunBox(const std::string &pathToRunBox);

    std::string getPathToTemplate() const;

    void setPathToTemplate(const std::string &pathToTemplate);

    void setFileExtension(const std::string &fileExtension);

    std::string getFileExtension() const;

    void setTimeLimitExceeded(bool);

    bool getTimeLimitExceeded() const;

private:
    uint32_t compilationId_{};
    uint32_t messageId_{};
    std::string messageCode_;
    std::string compilerStderr_; // ошибка компиляции, если есть, выводим только его
    std::string compilerStdout_;
    std::string executionStderr_; // время, память, аутпут, и эту ошибку
    std::string executionStdin_;
    std::string executionStdout_;
    std::string executionUsedMemory_;
    std::string executionTime_;
    std::string pathToRunBox_;
    std::string pathToTemplate_;
    std::string fileExtension_;
    bool timeLimitExceeded_; // время 5 сек
};

#endif  // PROJECT_INCLUDE_COMPILATION_HPP_
