#include "Transaction.h"
#include "Account.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}
    
    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};

class MockTransaction : public Transaction {
public:
    MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};

TEST(TransactionTest, MakeSuccess) {
    MockAccount from(1, 200);
    MockAccount to(2, 50);
    Transaction tr;
    tr.set_fee(10);

    {
        ::testing::InSequence seq;
        EXPECT_CALL(from, Lock());
        EXPECT_CALL(to, Lock());
        EXPECT_CALL(from, GetBalance()).WillOnce(testing::Return(200));
        EXPECT_CALL(from, ChangeBalance(-110));
        EXPECT_CALL(to, ChangeBalance(100));
        EXPECT_CALL(to, Unlock());
        EXPECT_CALL(from, Unlock());
    }

    bool result = tr.Make(from, to, 100);
    EXPECT_TRUE(result);
}

       



TEST(TransactionTest, MakeInvalidSameAccount) {
    MockAccount acc(1, 100);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc, acc, 50), std::logic_error);
}

TEST(TransactionTest, MakeInvalidNegativeSum) {
    MockAccount from(1, 100);
    MockAccount to(2, 50);
    Transaction tr;
    EXPECT_THROW(tr.Make(from, to, -10), std::invalid_argument);
}

TEST(TransactionTest, MakeInvalidTooSmallSum) {
    MockAccount from(1, 100);
    MockAccount to(2, 50);
    Transaction tr;
    EXPECT_THROW(tr.Make(from, to, 99), std::logic_error);
}

TEST(TransactionTest, MakeFeeTooHigh) {
    MockAccount from(1, 200);
    MockAccount to(2, 50);
    Transaction tr;
    tr.set_fee(60); // Для суммы 100 fee*2 = 120 > 100
    
    bool result = tr.Make(from, to, 100);
    EXPECT_FALSE(result);
}

class TestableTransaction : public Transaction {
public:
    using Transaction::Credit;
    using Transaction::Debit;
};

TEST(TransactionTest, DebitSuccess) {
    MockAccount acc(1, 100);
    TestableTransaction tr;
    
    EXPECT_CALL(acc, GetBalance()).WillOnce(testing::Return(100));
    bool result = tr.Debit(acc, 50);
    EXPECT_TRUE(result);
}

TEST(TransactionTest, DebitFail) {
    MockAccount acc(1, 100);
    TestableTransaction tr;
    
    EXPECT_CALL(acc, GetBalance()).WillOnce(testing::Return(100));
    bool result = tr.Debit(acc, 150);
    EXPECT_FALSE(result);
}

TEST(TransactionTest, Credit) {
    MockAccount acc(1, 100);
    TestableTransaction tr;
    
    EXPECT_NO_THROW(tr.Credit(acc, 50));
}

TEST(TransactionTest, SaveToDatabaseMock) {
    MockAccount from(1, 200);
    MockAccount to(2, 50);
    MockTransaction tr;
    tr.set_fee(10);

    EXPECT_CALL(tr, SaveToDataBase(testing::_, testing::_, testing::_)).Times(1);
    
    tr.Make(from, to, 100);
}

