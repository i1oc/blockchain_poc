#include "blockchain.h"
#include <string.h>

chain create_chain(int difficulty){
    chain ch;

    if (difficulty < 0) {
        ch.difficulty = 0;
    } else if (difficulty > HASH_SIZE) {
        ch.difficulty = HASH_SIZE;
    } else {
        ch.difficulty = difficulty;
    }

    ch.size = 1;
    ch.blocks[0] = create_genesis_block();


    return ch;
}
    
int add_block(chain *ch, transaction trans){
    if (!is_transaction_valid(trans)){return 0;}
    if (ch->size >= MAX_BLOCKS){return 0;}


    uint32_t next_block_index = ch->size;
    block blq = create_block(next_block_index, trans, ch->blocks[next_block_index-1].curr_block_hash);

    while (!hash_matches_difficulty(blq.curr_block_hash,ch->difficulty))
    {
        blq.nonce++;
        calculate_block_hash(&blq, &blq.curr_block_hash[0]);
    }


    ch->blocks[next_block_index] = blq;
    ch->size++;
    return 1;
}
    