#include <stdio.h>
#include "blockchain.h"

int main(void) {
    /* el "bloque anterior" de mentira: 32 bytes a cero */
    unsigned char prev[HASH_SIZE] = {0};

    transaction t = create_transaction(1, 2, 50);
    block b = create_block(1, t, prev);

    /* hasheamos el bloque escribiendo directamente en su propio campo */
    calculate_block_hash(&b, b.curr_block_hash);
    printf("hash del bloque:        ");
    print_hash(b.curr_block_hash);
    printf("\n");

    /* determinismo: el mismo bloque debe dar exactamente el mismo hash */
    unsigned char repetido[HASH_SIZE];
    calculate_block_hash(&b, repetido);
    printf("mismo bloque otra vez:  ");
    print_hash(repetido);
    printf("\n");

    /* avalancha: cambiar UN campo debe dar un hash totalmente distinto */
    b.nonce = 1;
    calculate_block_hash(&b, b.curr_block_hash);
    printf("con nonce = 1:          ");
    print_hash(b.curr_block_hash);
    printf("\n");

    /* y la transaccion tambien debe afectar al hash (test del bug de trans) */
    b.nonce = 0;
    b.trans.quantity = 9999;
    calculate_block_hash(&b, b.curr_block_hash);
    printf("con quantity = 9999:    ");
    print_hash(b.curr_block_hash);
    printf("\n");

    return 0;
}
