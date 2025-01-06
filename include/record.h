#ifndef RECORD_H
#define RECORD_H

typedef struct {
    int id;           // Record identifier
    int size;         // Size of the data
    char *data;       // Dynamically allocated record data
} Record;

// Function prototypes
Record *createRecord(int id, const char *data);
void freeRecord(Record *record);

#endif // RECORD_H
