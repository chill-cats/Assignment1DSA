//
// Created by Hoang Nhan Le Nguyen on 30/08/2021.
//

#include <gtest/gtest.h>
#include "../SymbolTable.h"

TEST(testInsert, testInsertGlobal) {
    SymbolTable table;
    EXPECT_NO_THROW(table.processLine("INSERT x string"));

    EXPECT_NE(table.scopes.globalScope->idList.head, nullptr);
    EXPECT_NE(table.scopes.globalScope->idList.tail, nullptr);

    EXPECT_THROW(table.processLine("INSERT x number"), Redeclared);

}

TEST(testInsert, testInsertMixed) {
    SymbolTable table;
    EXPECT_NO_THROW(table.processLine("INSERT x number"));

    EXPECT_NE(table.scopes.globalScope->idList.head, nullptr);
    EXPECT_NE(table.scopes.globalScope->idList.tail, nullptr);

    EXPECT_THROW(table.processLine("INSERT x number"), Redeclared);

    EXPECT_NO_THROW(table.processLine("BEGIN"));
    EXPECT_NO_THROW(table.processLine("INSERT x string"));

    // test if the same type pointer work
    EXPECT_EQ(table.scopes.globalScope->idList.head->nextOfSameType, table.scopes.innerMostScope->idList.head);
    EXPECT_EQ(table.scopes.globalScope->idList.head, table.scopes.innerMostScope->idList.head->prevOfSameType);

    // test if the type is correct
    EXPECT_EQ(table.scopes.innerMostScope->idList.head->id.type, IdentifierType::string);
    EXPECT_EQ(table.scopes.globalScope->idList.head->id.type, IdentifierType::number);

}

TEST(testInsert, testInsertMixed2) {
    SymbolTable table;
    EXPECT_NO_THROW(table.processLine("INSERT x number"));
    EXPECT_NO_THROW(table.processLine("INSERT y number"));
    EXPECT_NO_THROW(table.processLine("INSERT z string"));

    table.processLine("BEGIN");
    EXPECT_NO_THROW(table.processLine("INSERT y string"));

    table.processLine("BEGIN");
    EXPECT_NO_THROW(table.processLine("INSERT z string"));
    EXPECT_NO_THROW(table.processLine("INSERT y string"));

    auto *scp = &table.scopes;
    EXPECT_EQ(scp->globalScope->idList.head->id.name, "x");
    EXPECT_EQ(scp->globalScope->idList.head->nextInSameScope->id.name, "y");
    EXPECT_EQ(scp->globalScope->idList.head->nextInSameScope->nextInSameScope->id.name, "z");

    EXPECT_EQ(scp->globalScope->childScope->idList.head->id.name, "y");
    EXPECT_EQ(scp->innerMostScope->idList.head->id.name, "z");
    EXPECT_EQ(scp->innerMostScope->idList.head->nextInSameScope->id.name, "y");

    EXPECT_EQ(scp->globalScope->idList.head->nextOfSameType, nullptr);
    EXPECT_EQ(scp->globalScope->idList.head->nextInSameScope->nextOfSameType, scp->globalScope->childScope->idList.head);
    EXPECT_EQ(scp->globalScope->childScope->idList.head->nextOfSameType, scp->innerMostScope->idList.head->nextInSameScope);
    EXPECT_EQ(scp->globalScope->idList.head->nextInSameScope->nextInSameScope->nextOfSameType, scp->innerMostScope->idList.head);

    EXPECT_EQ(scp->innerMostScope->idList.head->prevOfSameType, scp->globalScope->idList.head->nextInSameScope->nextInSameScope);
    EXPECT_EQ(scp->innerMostScope->idList.head->nextInSameScope->prevOfSameType, scp->globalScope->childScope->idList.head);
    EXPECT_EQ(scp->globalScope->childScope->idList.head->prevOfSameType, scp->globalScope->idList.head->nextInSameScope);
}