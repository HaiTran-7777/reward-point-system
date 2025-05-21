#ifndef WALLET_H
#define WALLET_H

#include <string>

class Wallet {
private:
    std::string wallet_id;
    int balance;

public:
    Wallet();
    Wallet(const std::string& id, int bal = 0);

    const std::string& getWalletID() const;
    void setWalletID(const std::string& id);

    int getBalance() const;
    void setBalance(int bal);

    bool deposit(int amount);
    bool withdraw(int amount);
};

#endif
