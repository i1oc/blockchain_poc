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
    /*
    for (size_t i = 0; i < HASH_SIZE; i++)
    {
        *(blq.prev_block_hash + i) = *(prev_hash + i);
    }
    */
    memcpy(blq.prev_block_hash, prev_hash, HASH_SIZE);

    /*
    aqui llamamos a la funcion de hasheo para poder rellenar
    lo que nos quedae que es el curr_block_hash, pero le pasamos
    la referencia del objeto para que escriba en el
    */ 
    calculate_block_hash(&blq, &blq.curr_block_hash[0]);

    return blq;
}

block create_genesis_block(void){
    unsigned char prev[HASH_SIZE] = {0};

    transaction t = create_transaction(SYSTEM_ID, SYSTEM_ID, 0);
    block b = create_block(0, t, prev);

    return b;
}