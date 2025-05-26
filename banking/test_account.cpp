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

TEST(AccountTest, Constructor) {
    Account acc(1, 100);
    EXPECT_EQ(acc.id(), 1);
    EXPECT_EQ(acc.GetBalance(), 100);
}

TEST(AccountTest, LockUnlock) {
    Account acc(1, 100);
    acc.Lock();
    EXPECT_NO_THROW(acc.ChangeBalance(10));
    acc.Unlock();
    EXPECT_THROW(acc.ChangeBalance(10), std::runtime_error);
}

TEST(AccountTest, ChangeBalance) {
    Account acc(1, 100);
    acc.Lock();
    acc.ChangeBalance(50);
    EXPECT_EQ(acc.GetBalance(), 150);
    acc.ChangeBalance(-70);
    EXPECT_EQ(acc.GetBalance(), 80);
}

TEST(AccountTest, DoubleLock) {
    Account acc(1, 100);
    acc.Lock();
    EXPECT_THROW(acc.Lock(), std::runtime_error);
}

TEST(AccountTest, ChangeWithoutLock) {
    Account acc(1, 100);
    EXPECT_THROW(acc.ChangeBalance(10), std::runtime_error);
}

TEST(AccountTest, MockTest) {
    MockAccount acc(1, 100);
    
    EXPECT_CALL(acc, Lock()).Times(1);
    EXPECT_CALL(acc, Unlock()).Times(1);
    EXPECT_CALL(acc, GetBalance()).WillOnce(testing::Return(100));
    EXPECT_CALL(acc, ChangeBalance(testing::_)).Times(1);
    
    acc.Lock();
    acc.ChangeBalance(10);
    EXPECT_EQ(acc.GetBalance(), 100); // Mock возвращает 100
    acc.Unlock();
}

