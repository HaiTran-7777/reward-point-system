#ifndef OTP_MANAGER_H
#define OTP_MANAGER_H

#include <string>
#include <chrono>
#include <unordered_map>

class OTPManager {
private:
    struct OTPInfo {
        std::string code;
        std::chrono::steady_clock::time_point expire_time;
    };

    std::unordered_map<std::string, OTPInfo> otp_storage;

public:
    std::string generateOTP(const std::string& username);
    bool verifyOTP(const std::string& username, const std::string& code);
};

#endif
