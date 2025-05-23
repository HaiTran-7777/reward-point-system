#include "UserAccount.h"
#include <iostream>
#include <sstream>
#include "sha256.h"

UserAccount::UserAccount(const std::string& uname,
                         const std::string& hashed_pw,
                         const std::string& fullname,
                         const std::string& email_addr,
                         bool manager,
                         bool auto_gen_pw)
    : username(uname), hashed_password(hashed_pw), full_name(fullname),
      email(email_addr), is_manager(manager), is_password_auto_generated(auto_gen_pw) {}

const std::string& UserAccount::getUsername() const { return username; }

bool UserAccount::checkPassword(const std::string& pw) const {
    return sha256(pw) == hashed_password;
}

void UserAccount::setPassword(const std::string& pw, bool auto_gen) {
    hashed_password = sha256(pw);
    is_password_auto_generated = auto_gen;
}

bool UserAccount::getIsPasswordAutoGenerated() const {
    return is_password_auto_generated;
}

void UserAccount::setIsPasswordAutoGenerated(bool val) {
    is_password_auto_generated = val;
}

const std::string& UserAccount::getFullName() const { return full_name; }
void UserAccount::setFullName(const std::string& name) { full_name = name; }

const std::string& UserAccount::getEmail() const { return email; }
void UserAccount::setEmail(const std::string& mail) { email = mail; }

bool UserAccount::isManager() const { return is_manager; }

Wallet& UserAccount::getWallet() { return wallet; }

void UserAccount::displayInfo() const {
    std::cout << "Username: " << username << "\n"
              << "Full Name: " << full_name << "\n"
              << "Email: " << email << "\n"
              << "Role: " << (is_manager ? "Manager" : "User") << "\n"
              << "Wallet Balance: " << wallet.getBalance() << "\n";
}

std::string UserAccount::serialize() const {
    std::stringstream ss;
    ss << username << ","
       << hashed_password << ","
       << full_name << ","
       << email << ","
       << is_manager << ","
       << is_password_auto_generated << ","
       << wallet.getWalletID() << ","
       << wallet.getBalance();
    return ss.str();
}

UserAccount UserAccount::deserialize(const std::string& data_line) {
    std::stringstream ss(data_line);
    std::string token;
    std::string uname, hashed_pw, fullname, email_addr;
    bool manager, auto_gen;
    std::string wallet_id;
    int balance = 0;

    std::getline(ss, uname, ',');
    std::getline(ss, hashed_pw, ',');
    std::getline(ss, fullname, ',');
    std::getline(ss, email_addr, ',');
    std::getline(ss, token, ',');
    manager = (token == "1");
    std::getline(ss, token, ',');
    auto_gen = (token == "1");
    std::getline(ss, wallet_id, ',');
    std::getline(ss, token, ',');
    balance = std::stoi(token);

    UserAccount user(uname, hashed_pw, fullname, email_addr, manager, auto_gen);
    user.getWallet().setWalletID(wallet_id);
    user.getWallet().setBalance(balance);
    return user;
}
