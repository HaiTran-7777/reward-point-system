#include "Transaction.h"
#include "UserManager.h"
#include <sstream>
#include <iostream>

extern UserManager gUserManager;

Transaction::Transaction(const std::string& tid,
                         const std::string& from_id,
                         const std::string& to_id,
                         int amt)
    : transaction_id(tid), from_wallet_id(from_id), to_wallet_id(to_id), amount(amt) {
    timestamp = std::time(nullptr);
    success = false;
}

bool Transaction::processTransaction() {
    auto* from_wallet = gUserManager.findWalletByID(from_wallet_id);
    auto* to_wallet = gUserManager.findWalletByID(to_wallet_id);

    if (!from_wallet || !to_wallet) {
        std::cout << "Ví không tồn tại.\n";
        return false;
    }
    if (from_wallet->getBalance() < amount) {
        std::cout << "Số dư không đủ.\n";
        return false;
    }
    if (!from_wallet->withdraw(amount)) {
        std::cout << "Trừ điểm thất bại.\n";
        return false;
    }
    if (!to_wallet->deposit(amount)) {
        from_wallet->deposit(amount);
        std::cout << "Cộng điểm thất bại, rollback.\n";
        return false;
    }
    success = true;
    return true;
}

std::string Transaction::serialize() const {
    std::stringstream ss;
    ss << transaction_id << "," << from_wallet_id << "," << to_wallet_id << ","
       << amount << "," << timestamp << "," << (success ? 1 : 0);
    return ss.str();
}
