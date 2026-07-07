#include "blockchain.h"


chain create_chain(int difficulty){
    chain ch;

    ch.difficulty = difficulty;
    ch.size = 1;
    ch.blocks[0] = create_genesis_block();


    return ch;
}
    
