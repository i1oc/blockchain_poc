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

    /* hash_equal con hashes iguales debe devolver 1 */
    printf("hash_equal (iguales):   %s\n",
           hash_equal(b.curr_block_hash, repetido) ? "OK" : "FALLO");

    /* avalancha: cambiar UN campo debe dar un hash totalmente distinto */
    b.nonce = 1;
    calculate_block_hash(&b, b.curr_block_hash);
    printf("con nonce = 1:          ");
    print_hash(b.curr_block_hash);
    printf("\n");

    /* hash_equal con hashes distintos debe devolver 0 */
    printf("hash_equal (distintos): %s\n",
           !hash_equal(b.curr_block_hash, repetido) ? "OK" : "FALLO");

    /* y la transaccion tambien debe afectar al hash (test del bug de trans) */
    b.nonce = 0;
    b.trans.quantity = 9999;
    calculate_block_hash(&b, b.curr_block_hash);
    printf("con quantity = 9999:    ");
    print_hash(b.curr_block_hash);
    printf("\n");

    /* tests de hash_matches_difficulty: hash fabricado para controlar cada byte */
    unsigned char fabricado[HASH_SIZE] = {0}; // todo ceros...
    fabricado[2] = 0xab;                      // ...salvo el tercer byte

    printf("dificultad 0:           %s\n",
           hash_matches_difficulty(fabricado, 0) ? "OK" : "FALLO");
    printf("dificultad 2:           %s\n",
           hash_matches_difficulty(fabricado, 2) ? "OK" : "FALLO");
    printf("dificultad 3:           %s\n",
           !hash_matches_difficulty(fabricado, 3) ? "OK" : "FALLO");
    printf("dificultad 99:          %s\n",
           !hash_matches_difficulty(fabricado, 99) ? "OK" : "FALLO");

    return 0;
}
