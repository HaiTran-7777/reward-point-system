#include "OTPManager.h"
#include <random>
#include <iostream>

std::string OTPManager::generateOTP(const std::string& username) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(100000, 999999);
    std::string otp_code = std::to_string(dist(gen));

    OTPInfo info;
    info.code = otp_code;
    info.expire_time = std::chrono::steady_clock::now() + std::chrono::minutes(2);

    otp_storage[username] = info;

    std::cout << "[Hệ thống] OTP cho '" << username << "' là: " << otp_code << " (hết hạn 2 phút)\n";
    return otp_code;
}

bool OTPManager::verifyOTP(const std::string& username, const std::string& code) {
    auto it = otp_storage.find(username);
    if (it == otp_storage.end()) return false;

    auto now = std::chrono::steady_clock::now();
    if (now > it->second.expire_time) {
        otp_storage.erase(it);
        std::cout << "OTP hết hạn.\n";
        return false;
    }

    if (it->second.code == code) {
        otp_storage.erase(it);
        return true;
    }
    return false;
}
