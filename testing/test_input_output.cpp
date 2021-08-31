//
// Created by Hoang Nhan Le Nguyen on 28/08/2021.
//
#include "gtest/gtest.h"
#include "../../assignment1-dsa/SymbolTable.h"

#define EXPECT_THROW_WITH_MESSAGE(stmt, etype, whatstring) EXPECT_THROW( \
        try { \
            stmt; \
        } catch (const etype& ex) { \
            EXPECT_EQ(std::string(ex.what()), whatstring); \
            throw; \
        } \
    , etype)

TEST(checkTestOk, testStringEqual) {
    std::string str{"success"};
    std::string str2{"success"};
    EXPECT_EQ(str, str2);
    EXPECT_EQ(std::string{"OK"}, "OK");
}

TEST(checkTestOk, testThrow) {
    const auto a = []() { throw std::exception(); };
    EXPECT_THROW(a(), std::exception);
}

TEST(testGeneralInstruction, invalidInstructionHandle) {
    using ii = InvalidInstruction;
    SymbolTable table;

    // identifier name start with capital character
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT Abc string"), ii, "Invalid: INSERT Abc string");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN Abc 123"), ii, "Invalid: ASSIGN Abc 123");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN abc Abc"), ii, "Invalid: ASSIGN abc Abc");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN Abc Abc"), ii, "Invalid: ASSIGN Abc Abc");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("LOOKUP Abc"), ii, "Invalid: LOOKUP Abc");

    // identifier name contain special character
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT ab@c string"), ii, "Invalid: INSERT ab@c string");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN bec@use 'help'"), ii, "Invalid: ASSIGN bec@use 'help'");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN okName this-is-bad"), ii, "Invalid: ASSIGN okName this-is-bad");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN b@dn@me fuckthis-Shit"), ii,
                              "Invalid: ASSIGN b@dn@me fuckthis-Shit");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("LOOKUP b@dn@me"), ii, "Invalid: LOOKUP b@dn@me");

    // identifier name start with numeric character
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT 1bx number"), ii, "Invalid: INSERT 1bx number");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN 32fjb 123"), ii, "Invalid: ASSIGN 32fjb 123");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN xPos 69s3x"), ii, "Invalid: ASSIGN xPos 69s3x");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN 3frick 69s3x"), ii, "Invalid: ASSIGN 3frick 69s3x");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("LOOKUP 84why"), ii, "Invalid: LOOKUP 84why");

    // identifier name missing
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT  number"), ii, "Invalid: INSERT  number");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN  123"), ii, "Invalid: ASSIGN  123");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("LOOKUP "), ii, "Invalid: LOOKUP ");

    // type is not number or string, aka dog
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT ab dog"), ii, "Invalid: INSERT ab dog");

    // type is missing
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT y"), ii, "Invalid: INSERT y");

    // 1 space exist at the beginning of command
    EXPECT_THROW_WITH_MESSAGE(table.processLine(" INSERT y string"), ii, "Invalid:  INSERT y string");
    EXPECT_THROW_WITH_MESSAGE(table.processLine(" ASSIGN good 'bad'"), ii, "Invalid:  ASSIGN good 'bad'");
    EXPECT_THROW_WITH_MESSAGE(table.processLine(" END"), ii, "Invalid:  END");
    EXPECT_THROW_WITH_MESSAGE(table.processLine(" BEGIN"), ii, "Invalid:  BEGIN");
    EXPECT_THROW_WITH_MESSAGE(table.processLine(" LOOKUP xy"), ii, "Invalid:  LOOKUP xy");
    EXPECT_THROW_WITH_MESSAGE(table.processLine(" PRINT"), ii, "Invalid:  PRINT");
    EXPECT_THROW_WITH_MESSAGE(table.processLine(" RPRINT"), ii, "Invalid:  RPRINT");

    // 1 space exist at the end of command
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT y number "), ii, "Invalid: INSERT y number ");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN charName 'Test 123' "), ii,
                              "Invalid: ASSIGN charName 'Test 123' ");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("END "), ii, "Invalid: END ");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("BEGIN "), ii, "Invalid: BEGIN ");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("LOOKUP windowSuck "), ii, "Invalid: LOOKUP windowSuck ");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("PRINT "), ii, "Invalid: PRINT ");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("RPRINT "), ii, "Invalid: RPRINT ");

    // more than 1 space exist between arguments
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT  y string"), ii, "Invalid: INSERT  y string");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT y  number"), ii, "Invalid: INSERT y  number");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT  y   string"), ii, "Invalid: INSERT  y   string");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN  y 'linux'"), ii, "Invalid: ASSIGN  y 'linux'");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN linux  1804"), ii, "Invalid: ASSIGN linux  1804");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("ASSIGN  ubuntu  2104"), ii, "Invalid: ASSIGN  ubuntu  2104");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("LOOKUP  linus"), ii, "Invalid: LOOKUP  linus");

    // nothing at all
    EXPECT_THROW_WITH_MESSAGE(table.processLine(""), ii, "Invalid: ");

}

// identifier name has the following format: [a-z]\w*
// which is a normal character follow by capitalized character, normal character, underscore (_) or number
TEST(testInsert, insertNotExistingIdentifier) {
    SymbolTable table;
    EXPECT_EQ(table.processLine("INSERT y string"), "success");
    EXPECT_EQ(table.processLine("INSERT linux18_is_GooD number"), "success");
    EXPECT_EQ(table.processLine("INSERT frick_WiNdOwS_11 number"), "success");
}

TEST(testInsert, insertExistingIdentifierInCurrentScope) {
    using rd = Redeclared;
    SymbolTable table;
    EXPECT_EQ(table.processLine("INSERT y string"), "success");
    EXPECT_EQ(table.processLine("INSERT linus_Tech_Tips number"), "success");

    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT y number"), rd, "Redeclared: INSERT y number");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT y string"), rd, "Redeclared: INSERT y string");

    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT linus_Tech_Tips number"), rd,
                              "Redeclared: INSERT linus_Tech_Tips number");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT linus_Tech_Tips string"), rd,
                              "Redeclared: INSERT linus_Tech_Tips string");
}

TEST(testInsert, insertExistingIdentifierInOutterScope) {
    using rd = Redeclared;
    SymbolTable table;

    EXPECT_EQ(table.processLine("INSERT someString string"), "success");
    EXPECT_EQ(table.processLine("INSERT someNumber number"), "success");

    EXPECT_NO_THROW(table.processLine("BEGIN"));

    EXPECT_EQ(table.processLine("INSERT someString string"), "success");
    EXPECT_EQ(table.processLine("INSERT someNumber number"), "success");

    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT someString string"), rd,
                              "Redeclared: INSERT someString string");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT someNumber number"), rd,
                              "Redeclared: INSERT someNumber number");

    EXPECT_NO_THROW(table.processLine("END"));
}

TEST(testInsert, insertExistingIdentifierInInnerAndOutterScope) {
    using rd = Redeclared;
    SymbolTable table;

    EXPECT_EQ(table.processLine("INSERT someString number"), "success");
    EXPECT_EQ(table.processLine("INSERT someNumber number"), "success");
    EXPECT_EQ(table.processLine("INSERT outerScope number"), "success");

    EXPECT_NO_THROW(table.processLine("BEGIN"));
    EXPECT_EQ(table.processLine("INSERT someString number"), "success");
    EXPECT_EQ(table.processLine("INSERT someNumber number"), "success");

    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT someNumber number"), rd,
                              "Redeclared: INSERT someNumber number");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT someString string"), rd,
                              "Redeclared: INSERT someString string");

    EXPECT_EQ(table.processLine("INSERT outerScope string"), "success");
    EXPECT_THROW_WITH_MESSAGE(table.processLine("INSERT outerScope string"), rd,
                              "Redeclared: INSERT outerScope string");
}

TEST(testBeginEnd, testEndGlobalScope) {
    SymbolTable table;
    EXPECT_THROW(table.processLine("END"), UnknownBlock);
}

TEST(testBeginEnd, testOpenAndCloseInnerScope) {
    SymbolTable table;
    table.processLine("BEGIN");
    EXPECT_NO_THROW(table.processLine("END"));
}

TEST(testBeginEnd, testOpenAndCloseInnerBlock2) {
    SymbolTable table;
    for (int i = 0; i < 100; i++) {
        table.processLine("BEGIN");
    }
    for (int i = 0; i < 100; i++) {
        EXPECT_NO_THROW(table.processLine("END"));
    }
    EXPECT_THROW(table.processLine("END"), UnknownBlock);
}

TEST(testLookup, testLookupGlobal) {
    SymbolTable table;
    EXPECT_NO_THROW(table.processLine("INSERT y string"));
    EXPECT_EQ(table.processLine("LOOKUP y"), "0");
}

TEST(testLookup, testLookUpInner) {
    SymbolTable table;
    table.processLine("BEGIN");
    EXPECT_NO_THROW(table.processLine("INSERT y string"));
    EXPECT_EQ(table.processLine("LOOKUP y"), "1");
    table.processLine("END");
}

TEST(testLookup, testLookupMixed) {
    SymbolTable table;
    for (int i = 0; i < 1000; i++) {
        table.processLine("BEGIN");
        std::string identifierName{"var"};
        identifierName += std::to_string(i + 1);
        EXPECT_NO_THROW(table.processLine("INSERT " + identifierName + ' ' + "string"));
    }

    for (int i = 0; i < 1000; i++) {
        EXPECT_EQ(table.processLine(std::string("LOOKUP ") + "var" + std::to_string(i + 1)), std::to_string(i + 1));
    }
    for (int i = 0; i < 1000; i++) {
        table.processLine("END");
    }
}

TEST(testLookup, testLookupNotExist) {
    SymbolTable table;
    EXPECT_THROW_WITH_MESSAGE(table.processLine("LOOKUP tomCruise"), Undeclared, "Undeclared: LOOKUP tomCruise");
}

TEST(testLookup, testLookupFromInnerScopeToOuterScope) {
    SymbolTable table;
    table.processLine("BEGIN");
    table.processLine("BEGIN");
    table.processLine("INSERT y string");
    table.processLine("INSERT k number");
    table.processLine("BEGIN");
    table.processLine("INSERT x string");
    EXPECT_EQ(table.processLine("LOOKUP x"), "3");
    EXPECT_EQ(table.processLine("LOOKUP y"), "2");
    EXPECT_EQ(table.processLine("LOOKUP k"), "2");

    table.processLine("END");
    table.processLine("END");
    table.processLine("END");
}
