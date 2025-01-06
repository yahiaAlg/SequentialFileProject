#ifndef SEQUENTIAL_FILE_H
#define SEQUENTIAL_FILE_H

#include "block.h"
#include "record.h"

typedef struct {
    Block *head;       // Pointer to the first block
    int blockSize;     // Size of each block
    int isContiguous;  // 1 for Table, 0 for List
    int isOrdered;     // 1 for Ordered, 0 for Unordered
    int isFixed;       // 1 for Fixed, 0 for Variable
    int allowOverlap;  // 1 for Continued, 0 for Not Continued
} SequentialFile;

// Function prototypes
SequentialFile *initializeFile(int blockSize, int isContiguous, int isOrdered, int isFixed, int allowOverlap);
void insertRecord(SequentialFile *file, Record *record);
int updateRecord(SequentialFile *file, int id, const char *newData);
int deleteRecord(SequentialFile *file, int id);
Record *searchRecord(SequentialFile *file, int key);
void reorganizeFile(SequentialFile *file);
void freeFile(SequentialFile *file);
void printFile(SequentialFile *file);
Record *binarySearchInFile(SequentialFile *file, int key);

#endif // SEQUENTIAL_FILE_H
