#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

class Transaction {
private:
    std::string transaction_id;
    std::string from_wallet_id;
    std::string to_wallet_id;
    int amount;
    std::time_t timestamp;
    bool success;

public:
    Transaction(const std::string& tid,
                const std::string& from_id,
                const std::string& to_id,
                int amt);

    bool processTransaction();

    std::string serialize() const;
};

#endif
