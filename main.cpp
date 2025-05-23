#include <iostream>
#include <string>
#include <ctime>
#include <random>
#include "UserAccount.h"
#include "UserManager.h"
#include "Wallet.h"
#include "Transaction.h"
#include "OTPManager.h"

UserManager gUserManager;
OTPManager gOTPManager;

std::string generateRandomPassword(int length = 8) {
    const char chars[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(chars) - 2);

    std::string password;
    for (int i = 0; i < length; i++) {
        password += chars[dis(gen)];
    }
    return password;
}

void registerUser() {
    std::string username, fullname, email, password;
    std::cout << "Nhập username: ";
    std::getline(std::cin, username);

    if (gUserManager.findUserByUsername(username) != nullptr) {
        std::cout << "Username đã tồn tại.\n";
        return;
    }

    std::cout << "Nhập họ tên: ";
    std::getline(std::cin, fullname);

    std::cout << "Nhập email: ";
    std::getline(std::cin, email);

    std::cout << "Nhập mật khẩu (để trống để tự sinh): ";
    std::getline(std::cin, password);

    bool is_auto_pw = false;
    if (password.empty()) {
        password = generateRandomPassword();
        std::cout << "Mật khẩu tự sinh là: " << password << "\n";
        is_auto_pw = true;
    }

    std::string wallet_id = "W" + std::to_string(std::time(nullptr)) + username;

    UserAccount user(username, "", fullname, email, false, is_auto_pw);
    user.setPassword(password, is_auto_pw);
    user.getWallet().setWalletID(wallet_id);
    user.getWallet().setBalance(0);

    if (gUserManager.addUser(user)) {
        std::cout << "Đăng ký thành công!\n";
    } else {
        std::cout << "Lỗi khi đăng ký.\n";
    }
}

UserAccount* login() {
    std::string username, password;
    std::cout << "Username: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    UserAccount* user = gUserManager.findUserByUsername(username);
    if (user == nullptr) {
        std::cout << "Không tìm thấy username.\n";
        return nullptr;
    }
    if (!user->checkPassword(password)) {
        std::cout << "Sai mật khẩu.\n";
        return nullptr;
    }
    if (user->getIsPasswordAutoGenerated()) {
        std::cout << "Bạn đang dùng mật khẩu tự sinh, vui lòng đổi mật khẩu ngay!\n";
    }
    std::cout << "Đăng nhập thành công.\n";
    return user;
}

void changePassword(UserAccount* user) {
    std::string old_pw, new_pw, otp;
    std::cout << "Nhập mật khẩu cũ: ";
    std::getline(std::cin, old_pw);
    if (!user->checkPassword(old_pw)) {
        std::cout << "Mật khẩu cũ không đúng.\n";
        return;
    }
    std::cout << "Nhập mật khẩu mới: ";
    std::getline(std::cin, new_pw);

    gOTPManager.generateOTP(user->getUsername());
    std::cout << "Nhập mã OTP: ";
    std::getline(std::cin, otp);
    if (!gOTPManager.verifyOTP(user->getUsername(), otp)) {
        std::cout << "OTP không hợp lệ.\n";
        return;
    }
    user->setPassword(new_pw, false);
    user->setIsPasswordAutoGenerated(false);
    std::cout << "Đổi mật khẩu thành công.\n";
}

void showUserInfo(UserAccount* user) {
    user->displayInfo();
}

void transferPoints(UserAccount* user) {
    std::string to_username;
    int amount;
    std::string otp;

    std::cout << "Nhập username ví nhận điểm: ";
    std::getline(std::cin, to_username);
    UserAccount* to_user = gUserManager.findUserByUsername(to_username);
    if (!to_user) {
        std::cout << "Không tìm thấy người nhận.\n";
        return;
    }

    std::cout << "Nhập số điểm muốn chuyển: ";
    std::cin >> amount;
    std::cin.ignore();

    if (amount <= 0) {
        std::cout << "Số điểm không hợp lệ.\n";
        return;
    }

    if (user->getWallet().getBalance() < amount) {
        std::cout << "Số dư không đủ.\n";
        return;
    }

    gOTPManager.generateOTP(user->getUsername());
    std::cout << "Nhập mã OTP để xác nhận giao dịch: ";
    std::getline(std::cin, otp);
    if (!gOTPManager.verifyOTP(user->getUsername(), otp)) {
        std::cout << "OTP không hợp lệ.\n";
        return;
    }

    std::string transaction_id = "T" + std::to_string(std::time(nullptr)) + user->getUsername();

    Transaction trans(transaction_id, user->getWallet().getWalletID(), to_user->getWallet().getWalletID(), amount);
    if (trans.processTransaction()) {
        std::cout << "Chuyển điểm thành công!\n";
    } else {
        std::cout << "Chuyển điểm thất bại.\n";
    }
}

void saveData() {
    if (gUserManager.saveToFile("users.txt")) {
        std::cout << "Lưu dữ liệu thành công.\n";
    } else {
        std::cout << "Lưu dữ liệu thất bại.\n";
    }
}

void loadData() {
    if (gUserManager.loadFromFile("users.txt")) {
        std::cout << "Đọc dữ liệu thành công.\n";
    } else {
        std::cout << "Đọc dữ liệu thất bại hoặc không tìm thấy file.\n";
    }
}

int main() {
    loadData();

    UserAccount* current_user = nullptr;

    while (true) {
        std::cout << "=== HỆ THỐNG ĐĂNG NHẬP VÀ QUẢN LÝ ĐIỂM THƯỞNG ===\n";
        if (!current_user) {
            std::cout << "1. Đăng ký\n2. Đăng nhập\n3. Thoát\nChọn: ";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice == "1") {
                registerUser();
            } else if (choice == "2") {
                current_user = login();
            } else if (choice == "3") {
                saveData();
                break;
            } else {
                std::cout << "Lựa chọn không hợp lệ.\n";
            }
        } else {
            std::cout << "Chào " << current_user->getUsername() << ". Chọn chức năng:\n";
            std::cout << "1. Xem thông tin\n";
            std::cout << "2. Đổi mật khẩu\n";
            std::cout << "3. Chuyển điểm\n";
            std::cout << "4. Đăng xuất\n";
            std::cout << "Chọn: ";
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "1") {
                showUserInfo(current_user);
            } else if (choice == "2") {
                changePassword(current_user);
            } else if (choice == "3") {
                transferPoints(current_user);
            } else if (choice == "4") {
                current_user = nullptr;
                saveData();
            } else {
                std::cout << "Lựa chọn không hợp lệ.\n";
            }
        }
    }
    return 0;
}
