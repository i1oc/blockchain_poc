#ifndef CHAIN_H
#define CHAIN_H

#include "transaction.h"
#include <stdint.h>

#define HASH_SIZE 32
#define MAX_BLOCKS 100

typedef struct{    
    uint32_t index;
    uint64_t timestamp;
    transaction trans;
    unsigned char prev_block_hash[HASH_SIZE];
    unsigned char curr_block_hash[HASH_SIZE];
    uint32_t nonce;
} block;

typedef struct{
    block blocks[MAX_BLOCKS];
    int difficulty;
    int size;
} chain;

/* hash */
void print_hash(const unsigned char *hash);
int hash_equal(const unsigned char *hash_a, const unsigned char *hash_b);
int hash_matches_difficulty(const unsigned char *hash);


/* block */
block create_block(uint32_t index, transaction trans, const unsigned char *prev_hash);
void calculate_block_hash(const block *b, unsigned char *out_hash);
block create_genesis_block(void);

/* chain */
chain create_chain(block *genesis_block, int difficulty, int size);

#endif