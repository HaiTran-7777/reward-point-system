#include "Wallet.h"

Wallet::Wallet() : wallet_id(""), balance(0) {}

Wallet::Wallet(const std::string& id, int bal) : wallet_id(id), balance(bal) {}

const std::string& Wallet::getWalletID() const { return wallet_id; }

void Wallet::setWalletID(const std::string& id) { wallet_id = id; }

int Wallet::getBalance() const { return balance; }

void Wallet::setBalance(int bal) { balance = bal; }

bool Wallet::deposit(int amount) {
    if (amount < 0) return false;
    balance += amount;
    return true;
}

bool Wallet::withdraw(int amount) {
    if (amount < 0 || amount > balance) return false;
    balance -= amount;
    return true;
}
