#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <vector>
#include <string>
#include "UserAccount.h"
#include "Wallet.h"

class UserManager {
private:
    std::vector<UserAccount> users;

public:
    UserManager();

    bool addUser(const UserAccount& user);
    UserAccount* findUserByUsername(const std::string& username);
    Wallet* findWalletByID(const std::string& wallet_id);

    bool saveToFile(const std::string& filename_users);
    bool loadFromFile(const std::string& filename_users);

    void listUsers() const;
};

extern UserManager gUserManager;

#endif
