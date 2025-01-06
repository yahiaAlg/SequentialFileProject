#include <stdlib.h>
#include "block.h"

Block *createBlock(int blockSize) {
    Block *block = (Block *)malloc(sizeof(Block));
    block->data = (char *)malloc(blockSize);
    block->blockSize = blockSize;
    block->freeSpace = blockSize;
    block->next = NULL;
    return block;
}

void freeBlock(Block *block) {
    if (block) {
        free(block->data);
        free(block);
    }
}
