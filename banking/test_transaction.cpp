#include "Account.h"
#include "Transaction.h"
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
    MockTransaction tr;
    tr.set_fee(10);

    // Устанавливаем ожидания в строгой последовательности
    ::testing::InSequence seq;

    // Блокировка аккаунтов
    EXPECT_CALL(from, Lock());
    EXPECT_CALL(to, Lock());
    
    // Проверка баланса перед переводом
    EXPECT_CALL(from, GetBalance()).WillOnce(testing::Return(200));
    
    // Изменение балансов
    EXPECT_CALL(from, ChangeBalance(-110));
    EXPECT_CALL(to, ChangeBalance(100));
    
    // Ожидаем вызов SaveToDataBase
    EXPECT_CALL(tr, SaveToDataBase(testing::Ref(from), testing::Ref(to), 100))
        .WillOnce(testing::Invoke([&](Account& from_acc, Account& to_acc, int sum) {
            // Внутри SaveToDataBase происходят вызовы GetBalance()
            // Устанавливаем ожидания для этих вызовов
            EXPECT_CALL(from, GetBalance()).WillOnce(testing::Return(90));
            EXPECT_CALL(to, GetBalance()).WillOnce(testing::Return(150));
            
            // Вызываем реальную реализацию SaveToDataBase
            static_cast<Transaction&>(tr).SaveToDataBase(from_acc, to_acc, sum);
        });
    
    // Разблокировка аккаунтов
    EXPECT_CALL(to, Unlock());
    EXPECT_CALL(from, Unlock());

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
    tr.set_fee(60);
    
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
    EXPECT_CALL(acc, ChangeBalance(-50)).Times(1);
    bool result = tr.Debit(acc, 50);
    EXPECT_TRUE(result);
}

TEST(TransactionTest, DebitFail) {
    MockAccount acc(1, 100);
    TestableTransaction tr;
    
    EXPECT_CALL(acc, GetBalance()).WillOnce(testing::Return(100));
    EXPECT_CALL(acc, ChangeBalance(testing::_)).Times(0);
    bool result = tr.Debit(acc, 150);
    EXPECT_FALSE(result);
}

TEST(TransactionTest, Credit) {
    MockAccount acc(1, 100);
    TestableTransaction tr;
    
    EXPECT_CALL(acc, ChangeBalance(50)).Times(1);
    tr.Credit(acc, 50);
}
