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

    /* ---- tests de la chain completa: crear, minar, enlazar ---- */
    chain ch = create_chain(3);
    printf("\n== chain con dificultad %d ==\n", ch.difficulty);

    if (!add_block(&ch, create_transaction(1, 2, 100))) { printf("fallo anadiendo bloque 1\n"); }
    if (!add_block(&ch, create_transaction(2, 3, 25)))  { printf("fallo anadiendo bloque 2\n"); }
    if (!add_block(&ch, create_transaction(3, 1, 60)))  { printf("fallo anadiendo bloque 3\n"); }

    for (int i = 0; i < ch.size; i++) {
        printf("bloque %u | nonce %6u | hash: ", ch.blocks[i].index, ch.blocks[i].nonce);
        print_hash(ch.blocks[i].curr_block_hash);
        printf("\n");
    }

    /* el prev_hash de cada bloque debe ser el curr_hash del anterior */
    int enlaces_ok = 1;
    for (int i = 1; i < ch.size; i++) {
        if (!hash_equal(ch.blocks[i].prev_block_hash, ch.blocks[i - 1].curr_block_hash)) {
            enlaces_ok = 0;
        }
    }
    printf("enlaces de la cadena:   %s\n", enlaces_ok ? "OK" : "FALLO");

    /* una transaccion invalida debe rechazarse sin tocar la chain */
    int size_antes = ch.size;
    int rechazada = !add_block(&ch, create_transaction(5, 5, 0));
    printf("tx invalida rechazada:  %s\n",
           (rechazada && ch.size == size_antes) ? "OK" : "FALLO");

    /* ---- el experimento: validate_chain detecta manipulaciones ---- */
    printf("\n== validate_chain ==\n");

    /* la cadena recien construida debe ser valida */
    printf("cadena intacta:         %s\n",
           validate_chain(&ch) ? "OK" : "FALLO");

    /* manipulamos una transaccion antigua: el hash guardado ya no casa, incluso recalculando algun otro */
    ch.blocks[1].trans.quantity = 999;
    printf("tras manipular tx:      %s\n",
           !validate_chain(&ch) ? "OK (detectada)" : "FALLO (no detectada)");

    calculate_block_hash(&ch.blocks[1], ch.blocks[1].curr_block_hash);
    printf("tras recalcular hash:   %s\n",
           !validate_chain(&ch) ? "OK (detectada)" : "FALLO (no detectada)");

    return 0;
}
