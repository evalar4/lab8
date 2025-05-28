#pragma once

class Account;  // Forward declaration

class Transaction {
 public:
  Transaction();
  virtual ~Transaction();

  bool Make(Account& from, Account& to, int sum);
  int fee() const { return fee_; }
  void set_fee(int fee) { fee_ = fee; }

 protected:
  void Credit(Account& accout, int sum);
  bool Debit(Account& accout, int sum);

  virtual void SaveToDataBase(Account& from, Account& to, int sum);

  int fee_;
};
