#ifndef PROJECT_INCLUDE_COMPILERRESULTWRITER_HPP_
#define PROJECT_INCLUDE_COMPILERRESULTWRITER_HPP_

class ResultWriter {
private:
    CompilerDb compilerDb;

public:
    void writeCompilationResults(uint32_t messageId);
};

#endif  // PROJECT_INCLUDE_COMPILERRESULTWRITER_HPP_
