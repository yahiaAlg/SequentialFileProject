#include <stdlib.h>
#include <string.h>
#include "record.h"

Record *createRecord(int id, const char *data) {
    Record *record = (Record *)malloc(sizeof(Record));
    record->id = id;
    record->size = strlen(data) + 1;
    record->data = (char *)malloc(record->size);
    strcpy(record->data, data);
    return record;
}

void freeRecord(Record *record) {
    if (record) {
        free(record->data);
        free(record);
    }
}
