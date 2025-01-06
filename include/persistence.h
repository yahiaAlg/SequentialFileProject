#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "sequential_file.h"

// Function prototypes
void saveFileToDisk(SequentialFile *file, const char *filename);
SequentialFile *loadFileFromDisk(const char *filename);
int deleteFileFromDisk(const char *filename);

#endif // PERSISTENCE_H
