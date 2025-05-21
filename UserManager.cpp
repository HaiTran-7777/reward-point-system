#include "UserManager.h"
#include <fstream>
#include <iostream>

UserManager gUserManager;

UserManager::UserManager() {}

bool UserManager::addUser(const UserAccount& user) {
    if (findUserByUsername(user.getUsername()) != nullptr)
        return false;
    users.push_back(user);
    return true;
}

UserAccount* UserManager::findUserByUsername(const std::string& username) {
    for (auto& u : users) {
        if (u.getUsername() == username)
            return &u;
    }
    return nullptr;
}

Wallet* UserManager::findWalletByID(const std::string& wallet_id) {
    for (auto& u : users) {
        if (u.getWallet().getWalletID() == wallet_id)
            return &(u.getWallet());
    }
    return nullptr;
}

bool UserManager::saveToFile(const std::string& filename_users) {
    std::ofstream fout(filename_users);
    if (!fout.is_open()) return false;

    for (const auto& u : users) {
        fout << u.serialize() << "\n";
    }
    fout.close();
    return true;
}

bool UserManager::loadFromFile(const std::string& filename_users) {
    std::ifstream fin(filename_users);
    if (!fin.is_open()) return false;

    users.clear();
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        UserAccount u = UserAccount::deserialize(line);
        users.push_back(u);
    }
    fin.close();
    return true;
}

void UserManager::listUsers() const {
    for (const auto& u : users) {
        u.displayInfo();
        std::cout << "--------------------------\n";
    }
}
