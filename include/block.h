#ifndef BLOCK_H
#define BLOCK_H

#include "record.h"

typedef struct Block {
    char *data;         // Block data (dynamically allocated)
    int blockSize;      // Fixed size of the block
    int freeSpace;      // Remaining space in the block
    struct Block *next; // Pointer to the next block
} Block;

// Function prototypes
Block *createBlock(int blockSize);
void freeBlock(Block *block);

#endif // BLOCK_H
