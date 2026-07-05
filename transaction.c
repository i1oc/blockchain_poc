#include "transaction.h"

transaction create_transaction(uint32_t senderId, uint32_t receiverId, uint32_t quantity){
    transaction trans;

    trans.senderId = senderId;
    trans.receiverId = receiverId;
    trans.quantity = quantity;

    return trans;
}