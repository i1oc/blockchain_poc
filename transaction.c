#include "transaction.h"

transaction create_transaction(uint32_t senderId, uint32_t receiverId, uint32_t quantity){
    transaction trans;

    trans.senderId = senderId;
    trans.receiverId = receiverId;
    trans.quantity = quantity;

    return trans;
}

int is_transaction_valid(transaction trans){
    if (trans.senderId == trans.receiverId) {
        return 0;
    }

    if (trans.quantity == 0) {
        return 0;
    }

    return 1;
}