#include "blockchain.h"
#include <time.h>
#include <stdint.h>
#include <string.h>

block create_block(uint32_t index, transaction trans, const unsigned char *prev_hash){
    block blq;

    blq.index = index;
    blq.trans = trans;
    blq.timestamp = (uint64_t) time(NULL);
    blq.nonce = 0; // init a 0 
    /* Copiamos los contenidos de el param a el array struct */
    memcpy(blq.prev_block_hash, prev_hash, HASH_SIZE);
    calculate_block_hash(&blq, &blq.curr_block_hash[0]);

    return blq;
}

block create_genesis_block(void){
    unsigned char prev[HASH_SIZE] = {0};

    transaction t = create_transaction(SYSTEM_ID, SYSTEM_ID, 0);
    block b = create_block(0, t, prev);

    return b;
}