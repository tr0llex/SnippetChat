#ifndef PROJECT_INCLUDE_ICOMPILERDB_HPP_
#define PROJECT_INCLUDE_ICOMPILERDB_HPP_

#include "Models.hpp"

class ICompilerDb {
public:
    virtual void writeBinaryToDb(uint32_t messageId) = 0;

    virtual std::string binaryExistsInDb(uint32_t messageId) = 0;

    virtual bool writeCompilationToDb(Compilation compilation) = 0;
};

#endif  // PROJECT_INCLUDE_ICOMPILERDB_HPP_
