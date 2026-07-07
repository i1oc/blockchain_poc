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


int validate_chain(const chain *ch){
    for (int i = 1; i < ch->size; i++) {
        const block *b = &ch->blocks[i];

        if (b->index != (uint32_t) i) { return 0; }

        if (!hash_equal(b->prev_block_hash, ch->blocks[i - 1].curr_block_hash)) { return 0; }

        unsigned char recalculado[HASH_SIZE];
        calculate_block_hash(b, recalculado);
        if (!hash_equal(recalculado, b->curr_block_hash)) { return 0; }

        if (!hash_matches_difficulty(b->curr_block_hash, ch->difficulty)) { return 0; }
    }

    return 1;
}
    