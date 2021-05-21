#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../include/CompilationManager.hpp"

using ::testing::Return;
/*
class MockCompilationManager : public ICompilationManager {
public:
    MOCK_METHOD(Compilation, runCompilation, (uint32_t messageId, std::string executionStdin), (override));
};

TEST(RunCompilation, correctRequest) {
    MockCompilationManager mock;

    Compilation comp;
    comp.setExecutionStdout("3 2 1");

    EXPECT_CALL(mock, runCompilation(123, std::string("1 2 3"))).WillOnce(Return(comp));
    EXPECT_EQ((mock.runCompilation(123, std::string("1 2 3")).getExecutionStdout()), "3 2 1");
}
*/
