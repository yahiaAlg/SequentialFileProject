#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sequential_file.h"
SequentialFile *initializeFile(int blockSize, int isContiguous, int isOrdered, int isFixed, int allowOverlap) {
    SequentialFile *file = (SequentialFile *)malloc(sizeof(SequentialFile));
    file->head = NULL;
    file->blockSize = blockSize;
    file->isContiguous = isContiguous;
    file->isOrdered = isOrdered;
    file->isFixed = isFixed;
    file->allowOverlap = allowOverlap;
    return file;
}

// void insertRecord(SequentialFile *file, Record *record) {
//     Block *current = file->head;

//     // Create first block if the file is empty
//     if (!current) {
//         file->head = createBlock(file->blockSize);
//         current = file->head;
//     }

//     while (current) {
//         // If record fits in the current block
//         if (record->size <= current->freeSpace) {
//             memcpy(current->data + (current->blockSize - current->freeSpace), record, record->size);
//             current->freeSpace -= record->size;
//             return;
//         } 

//         // If overlap is allowed, split the record
//         if (file->allowOverlap && current->freeSpace > 0) {
//             int partSize = current->freeSpace;
//             memcpy(current->data + (current->blockSize - current->freeSpace), record, partSize);
//             current->freeSpace = 0;

//             // Adjust the record for the remaining part
//             record->size -= partSize;
//             record->data += partSize;
//         }

//         // Move to the next block or create a new block
//         if (!current->next) {
//             current->next = createBlock(file->blockSize);
//         }
//         current = current->next;
//     }
// }


void insertRecord(SequentialFile *file, Record *record) {
    Block *current = file->head;

    // Create first block if the file is empty
    if (!current) {
        file->head = createBlock(file->blockSize);
        current = file->head;
    }

    while (current) {
        // Calculate total space needed for the record
        int totalSpace = sizeof(Record) + record->size;
        
        // If record fits in the current block
        if (totalSpace <= current->freeSpace) {
            char *insertPoint = current->data + (current->blockSize - current->freeSpace);
            
            // Create a new record in the block
            Record *newRecord = (Record *)insertPoint;
            newRecord->id = record->id;
            newRecord->size = record->size;
            
            // Allocate and copy the data
            newRecord->data = (char *)malloc(record->size);
            memcpy(newRecord->data, record->data, record->size);
            
            current->freeSpace -= totalSpace;
            return;
        }

        // Move to next block or create new one
        if (!current->next) {
            current->next = createBlock(file->blockSize);
        }
        current = current->next;
    }
}


int updateRecord(SequentialFile *file, int id, const char *newData) {
    Block *current = file->head;

    while (current) {
        char *ptr = current->data;
        int remaining = current->blockSize - current->freeSpace;
        
        while (remaining > sizeof(Record)) {
            Record *record = (Record *)ptr;
            
            if (record->id == -1 || record->size <= 0) {
                break;
            }
            
            if (record->id == id) {
                int newSize = strlen(newData) + 1;
                int totalSpaceNeeded = sizeof(Record) + newSize;
                int oldTotalSpace = sizeof(Record) + record->size;
                
                // Calculate the actual space difference needed
                int spaceDifference = totalSpaceNeeded - oldTotalSpace;
                
                // If new data fits in the current space or we have enough free space
                if (spaceDifference <= current->freeSpace) {
                    // Free old data and allocate new
                    free(record->data);
                    record->data = (char *)malloc(newSize);
                    strcpy(record->data, newData);
                    record->size = newSize;
                    
                    // Update the block's free space only if we're using more space
                    if (spaceDifference > 0) {
                        current->freeSpace -= spaceDifference;
                    } else {
                        // If new data is smaller, we gain some free space
                        current->freeSpace -= spaceDifference;
                    }
                    return 1;
                } else {
                    printf("Error: Not enough space for the update\n");
                    return 0;
                }
            }
            
            // Move to next record
            ptr += sizeof(Record) + record->size;
            remaining -= (sizeof(Record) + record->size);
        }
        current = current->next;
    }
    return 0;
}

int deleteRecord(SequentialFile *file, int id) {
    Block *current = file->head;

    while (current) {
        char *ptr = current->data;
        int remaining = current->blockSize - current->freeSpace;
        
        while (remaining > 0) {
            Record *record = (Record *)ptr;
            if (record->id == id && record->id != -1) {
                record->id = -1; // Mark as deleted
                return 1; // Success
            }
            ptr += sizeof(Record) + record->size;
            remaining -= (sizeof(Record) + record->size);
        }
        current = current->next;
    }
    return 0; // Record not found
}


Record *searchRecord(SequentialFile *file, int key) {
    Block *current = file->head;

    while (current) {
        char *ptr = current->data;
        int remaining = current->blockSize - current->freeSpace;
        
        while (remaining > 0) {
            Record *record = (Record *)ptr;
            if (record->id == key && record->id != -1) {
                return record;
            }
            ptr += sizeof(Record) + record->size;
            remaining -= (sizeof(Record) + record->size);
        }
        current = current->next;
    }

    return NULL;
}


void reorganizeFile(SequentialFile *file) {
    if (file->isOrdered) return; // Skip for ordered files

    // Temporary storage
    int totalSize = 0;
    Block *current = file->head;
    while (current) {
        totalSize += (current->blockSize - current->freeSpace);
        current = current->next;
    }

    char *tempData = (char *)malloc(totalSize);
    int offset = 0;

    current = file->head;
    while (current) {
        memcpy(tempData + offset, current->data, current->blockSize - current->freeSpace);
        offset += (current->blockSize - current->freeSpace);
        current = current->next;
    }

    // Reorganize blocks
    current = file->head;
    offset = 0;
    while (current) {
        int toCopy = (current->blockSize < totalSize - offset) ? current->blockSize : totalSize - offset;
        memcpy(current->data, tempData + offset, toCopy);
        current->freeSpace = current->blockSize - toCopy;
        offset += toCopy;
        current = current->next;
    }

    free(tempData);
}



void freeFile(SequentialFile *file) {
    Block *current = file->head;
    while (current) {
        Block *next = current->next;
        freeBlock(current);
        current = next;
    }
    free(file);
}


// Function to print the sequential file in a human-readable format
void printFile(SequentialFile *file) {
    Block *current = file->head;
    
    printf("\nSequential File Contents:\n");
    printf("+------------+-----------------+\n");
    printf("| Record ID  | Data            |\n");
    printf("+------------+-----------------+\n");

    while (current) {
        char *ptr = current->data;
        int remaining = current->blockSize - current->freeSpace;
        
        while (remaining > 0) {
            Record *record = (Record *)ptr;
            if (record->id != -1) {  // Only print non-deleted records
                printf("| %-10d | %-15s |\n", record->id, record->data);
            }
            // Move pointer to next record
            ptr += sizeof(Record) + record->size;
            remaining -= (sizeof(Record) + record->size);
            
            if (remaining < sizeof(Record)) break;
        }
        current = current->next;
    }
    printf("+------------+-----------------+\n");
}

Record *binarySearchInFile(SequentialFile *file, int key) {
    if (!file->isOrdered) {
        printf("Binary search is only supported for ordered files.\n");
        return NULL;
    }

    Block *current = file->head;

    while (current) {
        int left = 0;
        int right = (current->blockSize - current->freeSpace) / sizeof(Record) - 1;
        Record *records = (Record *)(current->data);

        while (left <= right) {
            int mid = (left + right) / 2;
            if (records[mid].id == key) {
                return &records[mid];
            } else if (records[mid].id < key) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        current = current->next;
    }

    return NULL; // Not found
}
