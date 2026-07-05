#include "blockchain.h"
#include <openssl/evp.h>
#include <stddef.h>
#include <stdio.h>

void print_hash(const unsigned char *hash){

    for (size_t i = 0; i < HASH_SIZE; i++)
    {
        printf("%02x",*(hash + i));
    }
}

void calculate_block_hash(const block *b, unsigned char *out_hash){
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_sha256();

    EVP_DigestInit_ex(ctx, md, NULL);

    EVP_DigestUpdate(ctx, &b->index, sizeof(b->index));
    EVP_DigestUpdate(ctx, &b->timestamp, sizeof(b->timestamp));
    EVP_DigestUpdate(ctx, &b->trans, sizeof(b->trans));
    EVP_DigestUpdate(ctx, b->prev_block_hash, HASH_SIZE);
    EVP_DigestUpdate(ctx, &b->nonce, sizeof(b->nonce));

    EVP_DigestFinal_ex(ctx, out_hash, NULL);

    EVP_MD_CTX_free(ctx);
}


int hash_equal(const unsigned char *hash_a, const unsigned char *hash_b){
    /*
    for (size_t i = 0; i < HASH_SIZE; i++)    
    {
        if (hash_a[i] != hash_b[i]) {
            return 0;
        } 
    }
    
    return 1;
    */
    return memcmp(hash_a, hash_b, HASH_SIZE) == 0;
}