# blockchain_poc

A small proof of concept of a blockchain written in C, built as a learning
project. It records simple transactions in blocks linked by SHA-256 hashes,
with a basic proof of work.

Hashing is done with OpenSSL (EVP API).

## How it works

- Each block stores an index, a timestamp, one transaction, the hash of the
  previous block, its own hash and a nonce.
- New blocks are mined: the nonce is incremented until the block hash starts
  with N zero bytes, where N is the chain difficulty.
- `validate_chain` walks the chain and checks that every block sits at its
  index, links to the previous block, still produces its stored hash and
  meets the difficulty. Changing any byte of an old block makes it fail.

## Build and run

Requires gcc and OpenSSL headers.

```
gcc main.c block.c hash.c transaction.c chain.c -o main -lcrypto
./main
```

`main.c` runs a set of tests: hashing determinism, difficulty checks, mining
a few blocks and detecting a tampered transaction.

## Files

- `blockchain.h` - structs and declarations
- `block.c` - block creation
- `hash.c` - hashing, comparison and difficulty check
- `chain.c` - chain creation, adding blocks (with mining) and validation
- `transaction.c` - transaction creation and validation
- `main.c` - tests

## Limitations

Single node, in memory only. No network, no signatures, no balances, fixed
difficulty and a fixed maximum of 100 blocks. It is meant to show the core
data structure, not to be a real blockchain.
