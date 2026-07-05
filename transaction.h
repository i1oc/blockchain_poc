#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>

#define SYSTEM_ID 0

typedef struct {
    uint32_t senderId;
    uint32_t receiverId;
    uint32_t quantity;
} transaction;

transaction create_transaction(uint32_t senderId, uint32_t receiverId, uint32_t quantity);
int is_transaction_basic_valid(transaction trans);
void print_transaction(transaction trans);

#endif