#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sequential_file.h"
#include "persistence.h"

// Function prototypes
void displayMenu();
void handleInsert(SequentialFile *file);
void handleReadAll(SequentialFile *file);
void handleReadOne(SequentialFile *file);
void handleUpdate(SequentialFile *file);
void handleDelete(SequentialFile *file);

int main() {
    SequentialFile *file = initializeFile(256, 0, 0, 0, 1); // Default file setup
    int choice;

    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                handleInsert(file);
                break;
            case 2:
                handleReadAll(file);
                break;
            case 3:
                handleReadOne(file);
                break;
            case 4:
                handleUpdate(file);
                break;
            case 5:
                handleDelete(file);
                break;
            case 6:
                saveFileToDisk(file, "sequential_file.bin");
                printf("File saved to disk.\n");
                break;
            case 7:
                file = loadFileFromDisk("sequential_file.bin");
                if (file) {
                    printf("File loaded from disk.\n");
                }
                break;
            case 8:
                deleteFileFromDisk("sequential_file.bin");
                break;
            case 9:
                freeFile(file);
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

void displayMenu() {
    printf("\n=== Sequential File Menu ===\n");
    printf("1. Insert Record\n");
    printf("2. Read All Records\n");
    printf("3. Read a Specific Record\n");
    printf("4. Update a Record\n");
    printf("5. Delete a Record\n");
    printf("6. Save File to Disk\n");
    printf("7. Load File from Disk\n");
    printf("8. Delete File from Disk\n");
    printf("9. Exit\n");
    printf("============================\n");
}

void handleInsert(SequentialFile *file) {
    int id;
    char data[256];
    printf("Enter Record ID: ");
    scanf("%d", &id);
    printf("Enter Record Data: ");
    scanf(" %[^\n]", data);

    Record *record = createRecord(id, data);
    insertRecord(file, record);
    freeRecord(record);
    printf("Record inserted successfully.\n");
}

void handleReadAll(SequentialFile *file) {
    printFile(file);
}

void handleReadOne(SequentialFile *file) {
    int id;
    printf("Enter Record ID to Search: ");
    scanf("%d", &id);

    Record *record = searchRecord(file, id);
    if (record) {
        printf("\nRecord Found:\n");
        printf("+------------+----------------+\n");
        printf("| Record ID  | Data           |\n");
        printf("+------------+----------------+\n");
        printf("| %-10d | %-14s |\n", record->id, record->data);
        printf("+------------+----------------+\n");
    } else {
        printf("Record not found.\n");
    }
}

void handleUpdate(SequentialFile *file) {
    int id;
    char newData[256];
    printf("Enter Record ID to Update: ");
    scanf("%d", &id);

    Record *record = searchRecord(file, id);
    if (record) {
        printf("\nCurrent Record:\n");
        printf("+------------+----------------+\n");
        printf("| Record ID  | Data           |\n");
        printf("+------------+----------------+\n");
        printf("| %-10d | %-14s |\n", record->id, record->data);
        printf("+------------+----------------+\n\n");

        printf("Enter New Data: ");
        scanf(" %[^\n]", newData);

        if (updateRecord(file, id, newData)) {
            printf("\nRecord updated successfully:\n");
            record = searchRecord(file, id);
            printf("+------------+----------------+\n");
            printf("| Record ID  | Data           |\n");
            printf("+------------+----------------+\n");
            printf("| %-10d | %-14s |\n", record->id, record->data);
            printf("+------------+----------------+\n");
        } else {
            printf("Error updating record.\n");
        }
    } else {
        printf("Record not found.\n");
    }
}

void handleDelete(SequentialFile *file) {
    int id;
    printf("Enter Record ID to Delete: ");
    scanf("%d", &id);

    if (deleteRecord(file, id)) {
        printf("Record deleted successfully.\n");
    } else {
        printf("Record not found.\n");
    }
}
