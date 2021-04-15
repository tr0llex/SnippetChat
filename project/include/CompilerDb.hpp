#ifndef PROJECT_INCLUDE_COMPILERDB_HPP_
#define PROJECT_INCLUDE_COMPILERDB_HPP_

#include <pqxx/pqxx>

class ICompilerDb {
public:
    virtual void writeBinaryToDb(uint32_t messageId) = 0;

    virtual std::string binaryExistsInDb(uint32_t messageId) = 0;

    virtual bool writeCompilationToDb(Compilation compilation) = 0;
};

class CompilerDb : public ICompilerDb {
private:
    void connect(char const options[]);

    void disconnect();

    pqxx::connection connection;
    pqxx::work worker;

public:
    virtual void writeBinaryToDb(uint32_t messageId);

    virtual std::string binaryExistsInDb(uint32_t messageId);

    virtual bool writeCompilationToDb(Compilation compilation);
};

#endif  // PROJECT_INCLUDE_COMPILERDB_HPP_
