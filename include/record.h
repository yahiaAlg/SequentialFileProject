#ifndef RECORD_H
#define RECORD_H

typedef struct {
    int id;           // Record identifier
    char *data;       // Dynamically allocated record data
    int size;         // Size of the record
} Record;

// Function prototypes
Record *createRecord(int id, const char *data);
void freeRecord(Record *record);

#endif // RECORD_H
