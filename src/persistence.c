#include <stdio.h>
#include <stdlib.h>
#include "persistence.h" // For saving and loading files, refer to persistence.h


void saveFileToDisk(SequentialFile *file, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening file for writing");
        return;
    }

    // Write header information
    fwrite(&file->blockSize, sizeof(int), 1, fp);
    fwrite(&file->isContiguous, sizeof(int), 1, fp);
    fwrite(&file->isOrdered, sizeof(int), 1, fp);
    fwrite(&file->isFixed, sizeof(int), 1, fp);
    fwrite(&file->allowOverlap, sizeof(int), 1, fp);

    // Write blocks
    Block *current = file->head;
    while (current) {
        int usedSpace = current->blockSize - current->freeSpace;
        fwrite(&usedSpace, sizeof(int), 1, fp);
        fwrite(current->data, sizeof(char), usedSpace, fp);
        current = current->next;
    }

    fclose(fp);
}

SequentialFile *loadFileFromDisk(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening file for reading");
        return NULL;
    }

    // Read header information
    int blockSize, isContiguous, isOrdered, isFixed, allowOverlap;
    fread(&blockSize, sizeof(int), 1, fp);
    fread(&isContiguous, sizeof(int), 1, fp);
    fread(&isOrdered, sizeof(int), 1, fp);
    fread(&isFixed, sizeof(int), 1, fp);
    fread(&allowOverlap, sizeof(int), 1, fp);

    // Initialize the file
    SequentialFile *file = initializeFile(blockSize, isContiguous, isOrdered, isFixed, allowOverlap);

    // Read blocks
    Block *current = NULL;
    while (!feof(fp)) {
        int usedSpace;
        if (fread(&usedSpace, sizeof(int), 1, fp) != 1) break;

        Block *newBlock = createBlock(blockSize);
        fread(newBlock->data, sizeof(char), usedSpace, fp);
        newBlock->freeSpace = blockSize - usedSpace;

        if (!file->head) {
            file->head = newBlock;
        } else {
            current->next = newBlock;
        }
        current = newBlock;
    }

    fclose(fp);
    return file;
}



// Function to delete the sequential file from disk
int deleteFileFromDisk(const char *filename) {
    if (remove(filename) == 0) {
        printf("File '%s' deleted successfully.\n", filename);
        return 1; // Success
    } else {
        perror("Error deleting file");
        return 0; // Failure
    }
}