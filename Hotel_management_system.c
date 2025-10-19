
/* main.c
   Simple Hotel Management System (C)
   - Single-file demo: check-in/check-out, list rooms, export CSV/JSON
   - Compile: gcc -std=c99 -Wall -O2 -o hotel main.c
   - Author: R. Goutham
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOMS 10
#define NAME_LEN 64

typedef struct {
    int number;
    int occupied;       // 0 = vacant, 1 = occupied
    char guest[NAME_LEN];
    int nights;
    double bill;
} Room;

Room rooms[MAX_ROOMS];

void init_rooms() {
    for (int i = 0; i < MAX_ROOMS; ++i) {
        rooms[i].number = i + 1;
        rooms[i].occupied = 0;
        rooms[i].guest[0] = '\0';
        rooms[i].nights = 0;
        rooms[i].bill = 0.0;
    }
}

Room* find_room(int num) {
    if (num < 1 || num > MAX_ROOMS) return NULL;
    return &rooms[num - 1];
}

void clear_input_line() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void read_string_line(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
}

void list_rooms() {
    printf("\nRoom Status:\n");
    printf("Room\tStatus\t\tGuest\t\tNights\tBill\n");
    printf("----\t------\t\t-----\t\t------\t----\n");
    for (int i = 0; i < MAX_ROOMS; ++i) {
        printf("%2d\t%s\t\t%-12s\t%3d\t%.2f\n",
               rooms[i].number,
               rooms[i].occupied ? "Occupied" : "Vacant",
               rooms[i].occupied ? rooms[i].guest : "-",
               rooms[i].occupied ? rooms[i].nights : 0,
               rooms[i].occupied ? rooms[i].bill : 0.0);
    }
    printf("\n");
}

void check_in() {
    int roomno;
    printf("Enter room number to check in (1-%d): ", MAX_ROOMS);
    if (scanf("%d", &roomno) != 1) { clear_input_line(); printf("Invalid input.\n"); return; }
    clear_input_line();

    Room *r = find_room(roomno);
    if (!r) { printf("Room number invalid.\n"); return; }
    if (r->occupied) { printf("Room %d is already occupied by %s.\n", r->number, r->guest); return; }

    char name[NAME_LEN];
    int nights;
    printf("Enter guest name: ");
    read_string_line(name, NAME_LEN);
    if (strlen(name) == 0) { printf("Name cannot be empty.\n"); return; }
    printf("Enter number of nights: ");
    if (scanf("%d", &nights) != 1 || nights <= 0) { clear_input_line(); printf("Invalid nights.\n"); return; }
    clear_input_line();

    double rate = 1500.0; // default nightly rate (for demo)
    r->occupied = 1;
    strncpy(r->guest, name, NAME_LEN - 1);
    r->guest[NAME_LEN - 1] = '\0';
    r->nights = nights;
    r->bill = rate * nights;

    printf("Checked in %s to room %d for %d nights. Estimated bill: %.2f\n", r->guest, r->number, r->nights, r->bill);
}

void check_out() {
    int roomno;
    printf("Enter room number to check out: ");
    if (scanf("%d", &roomno) != 1) { clear_input_line(); printf("Invalid input.\n"); return; }
    clear_input_line();

    Room *r = find_room(roomno);
    if (!r) { printf("Room number invalid.\n"); return; }
    if (!r->occupied) { printf("Room %d is already vacant.\n", r->number); return; }

    printf("Checking out %s from room %d.\n", r->guest, r->number);
    printf("Nights: %d, Bill: %.2f\n", r->nights, r->bill);

    // Reset room
    r->occupied = 0;
    r->guest[0] = '\0';
    r->nights = 0;
    r->bill = 0.0;
    printf("Room %d is now vacant.\n", r->number);
}

void export_csv(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) { printf("Failed to open %s for writing.\n", filename); return; }
    fprintf(f, "room,occupied,guest,nights,bill\n");
    for (int i = 0; i < MAX_ROOMS; ++i) {
        fprintf(f, "%d,%s,%s,%d,%.2f\n",
                rooms[i].number,
                rooms[i].occupied ? "true" : "false",
                rooms[i].occupied ? rooms[i].guest : "n/a",
                rooms[i].occupied ? rooms[i].nights : 0,
                rooms[i].occupied ? rooms[i].bill : 0.0);
    }
    fclose(f);
    printf("CSV exported to %s\n", filename);
}

void export_json(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) { printf("Failed to open %s for writing.\n", filename); return; }
    fprintf(f, "{\n  \"rooms\": [\n");
    for (int i = 0; i < MAX_ROOMS; ++i) {
        fprintf(f, "    {\n");
        fprintf(f, "      \"room\": %d,\n", rooms[i].number);
        fprintf(f, "      \"occupied\": %s,\n", rooms[i].occupied ? "true" : "false");
        fprintf(f, "      \"guest\": \"%s\",\n", rooms[i].occupied ? rooms[i].guest : "n/a");
        fprintf(f, "      \"nights\": %d,\n", rooms[i].occupied ? rooms[i].nights : 0);
        fprintf(f, "      \"bill\": %.2f\n", rooms[i].occupied ? rooms[i].bill : 0.0);
        fprintf(f, "    }%s\n", (i == MAX_ROOMS - 1) ? "" : ",");
    }
    fprintf(f, "  ]\n}\n");
    fclose(f);
    printf("JSON exported to %s\n", filename);
}

int main() {
    init_rooms();

    // (Optional) pre-fill one or two rooms for demo
    strncpy(rooms[0].guest, "Alice", NAME_LEN-1); rooms[0].occupied = 1; rooms[0].nights = 2; rooms[0].bill = 3000.0;
    strncpy(rooms[3].guest, "Bob", NAME_LEN-1); rooms[3].occupied = 1; rooms[3].nights = 3; rooms[3].bill = 4500.0;

    int choice;
    char csv_name[128] = "results.csv";
    char json_name[128] = "results.json";

    while (1) {
        printf("\nHotel Management - Menu\n");
        printf("1. List rooms\n");
        printf("2. Check-in\n");
        printf("3. Check-out\n");
        printf("4. Export CSV (default: %s)\n", csv_name);
        printf("5. Export JSON (default: %s)\n", json_name);
        printf("6. Quit\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) { clear_input_line(); printf("Invalid selection.\n"); continue; }
        clear_input_line();

        switch (choice) {
            case 1: list_rooms(); break;
            case 2: check_in(); break;
            case 3: check_out(); break;
            case 4:
                printf("Enter CSV filename (or press Enter for default): ");
                read_string_line(csv_name, sizeof(csv_name));
                if (strlen(csv_name) == 0) strcpy(csv_name, "results.csv");
                export_csv(csv_name);
                break;
            case 5:
                printf("Enter JSON filename (or press Enter for default): ");
                read_string_line(json_name, sizeof(json_name));
                if (strlen(json_name) == 0) strcpy(json_name, "results.json");
                export_json(json_name);
                break;
            case 6:
                printf("Exiting. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
