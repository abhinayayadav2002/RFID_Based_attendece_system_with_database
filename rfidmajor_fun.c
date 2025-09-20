// rfid_helper.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rfid_helper.h"

#define CSV_FILE "attendance.csv"

AttendanceNode* head = NULL;

// ---------------- Time Utilities ----------------

void get_current_time(char* date_buf, char* time_buf) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(date_buf, 20, "%d-%m-%Y", tm_info);
    strftime(time_buf, 10, "%H:%M", tm_info);
}

int calculate_duration(char* in_time, char* out_time) {
    int in_hr, in_min, out_hr, out_min;
    sscanf(in_time, "%d:%d", &in_hr, &in_min);
    sscanf(out_time, "%d:%d", &out_hr, &out_min);
    return (out_hr * 60 + out_min) - (in_hr * 60 + in_min);
}

void format_duration(int minutes, char* buffer) {
    int h = minutes / 60;
    int m = minutes % 60;
    sprintf(buffer, "%d hrs %d min", h, m);
}

// ---------------- Linked List ----------------

AttendanceNode* find_user_by_id(const char* id) {
    AttendanceNode* temp = head;
    while (temp) {
        if (strcmp(temp->id, id) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void append_entry(AttendanceNode* new_node) {
    if (!head) {
        head = new_node;
        return;
    }
    AttendanceNode* temp = head;
    while (temp->next)
        temp = temp->next;
    new_node->sno = temp->sno + 1;
    temp->next = new_node;
}

// ---------------- CSV I/O ----------------

void save_to_csv() {
    FILE* file = fopen(CSV_FILE, "w");
    if (!file) {
        perror("Error saving to CSV");
        return;
    }

    fprintf(file, "S.No,USER_ID,USER_NAME,DATE,WORKING_HOURS,STATUS");
    for (int i = 1; i <= MAX_PAIRS; ++i)
        fprintf(file, ",IN%d,OUT%d", i, i);
    fprintf(file, "\n");

    AttendanceNode* temp = head;
    while (temp) {
        char duration[30];
        format_duration(temp->total_minutes, duration);
        fprintf(file, "%d,%s,%s,%s,%s,%s",
                temp->sno, temp->id, temp->name,
                temp->date, duration, temp->status);

        for (int i = 0; i < temp->pair_count; i++) {
            fprintf(file, ",%s,%s", temp->times[i].in, temp->times[i].out);
        }

        fprintf(file, "\n");
        temp = temp->next;
    }

    fclose(file);
}

void load_from_csv() {
    FILE* file = fopen(CSV_FILE, "r");
    if (!file) return;

    char line[1024];
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) {
        if(line[strlen(line)-1]=='\n')
            line[strlen(line)-1]='\0';
        AttendanceNode* node = (AttendanceNode*)malloc(sizeof(AttendanceNode));
        node->times = (InOutTime*)malloc(sizeof(InOutTime) * MAX_PAIRS);
        node->pair_count = 0;
        node->next = NULL;

        char* token = strtok(line, ",");
        node->sno = atoi(token);
        token = strtok(NULL, ","); strcpy(node->id, token);
        token = strtok(NULL, ","); strcpy(node->name, token);
        token = strtok(NULL, ","); strcpy(node->date, token);
        token = strtok(NULL, ","); // working hours (ignored)
        token = strtok(NULL, ","); strcpy(node->status, token);

        int i = 0;
        while (i < MAX_PAIRS && (token = strtok(NULL, ","))) {
            strcpy(node->times[i].in, token);
            token = strtok(NULL, ",");
            if (token) strcpy(node->times[i].out, token);
            else strcpy(node->times[i].out, "-");
            i++;
        }
        //token=strtok(NULL,",");
        node->pair_count = i;

        // Recalculate total_minutes
        node->total_minutes = 0;
        for (int j = 0; j < i; ++j) {
            if (strcmp(node->times[j].out, "-") != 0)
                node->total_minutes += calculate_duration(node->times[j].in, node->times[j].out);
        }
	//printf("calling append_entry\n");
        append_entry(node);
	//printf("after append_entry\n");
    }

    fclose(file);
    }

// ---------------- Attendance Logic ----------------

void process_user_card(AttendanceNode* entry) {
    char date[20], time[10];
    get_current_time(date, time);
    strcpy(entry->date,date);
    if (strcmp(entry->status, "OUT") == 0) {
        if (entry->pair_count < MAX_PAIRS) {
            strcpy(entry->times[entry->pair_count].in, time);
            strcpy(entry->times[entry->pair_count].out, "-");
            entry->pair_count++;
            strcpy(entry->status, "IN");
            printf("IN recorded\n");
        } else {
            printf(" Max IN/OUT pairs reached\n");
        }
    } else {
        strcpy(entry->times[entry->pair_count - 1].out, time);
        int minutes = calculate_duration(entry->times[entry->pair_count - 1].in, time);
        entry->total_minutes += minutes;
        strcpy(entry->status, "OUT");
        printf("OUT recorded (Duration: %d min)\n", minutes);
    }
}

// ---------------- Admin Operations ----------------

void list_users() {
    AttendanceNode* temp = head;
    int i = 1;
    while (temp) {
        printf("%d. ID: %s, Name: %s, Date: %s, Status: %s\n",
               i++, temp->id, temp->name, temp->date, temp->status);
        temp = temp->next;
    }
}

void add_user() {
    char id[20], name[50];
    printf("Enter new USER ID: ");
    scanf("%s", id);
    printf("Enter USER Name: ");
    scanf("%s", name);

    AttendanceNode* existing = find_user_by_id(id);
    if (existing) {
        printf("User already exists.\n");
        return;
    }

    AttendanceNode* node = (AttendanceNode*)malloc(sizeof(AttendanceNode));
    node->sno = (head == NULL) ? 1 : head->sno + 1;
    strcpy(node->id, id);
    strcpy(node->name, name);
    strcpy(node->date, "-");
    strcpy(node->status, "OUT");
    node->total_minutes = 0;
    node->pair_count = 0;
    node->times = (InOutTime*)malloc(sizeof(InOutTime) * MAX_PAIRS);
    node->next = NULL;

    append_entry(node);
    printf("User added.\n");
}

void edit_user() {
    list_users();
    int sel, i = 1;
    AttendanceNode* temp = head;

    printf("Enter the number to edit: ");
    scanf("%d", &sel);

    while (temp && i < sel) {
        temp = temp->next;
        i++;
    }

    if (temp) {
        printf("Enter new name for ID %s: ", temp->id);
        scanf("%s", temp->name);
        printf("Name updated.\n");
    } else {
        printf("Invalid selection.\n");
    }
}

void delete_user() {
    list_users();
    int sel, i = 1;
    AttendanceNode* temp = head, *prev = NULL;

    printf("Enter the number to delete: ");
    scanf("%d", &sel);

    while (temp && i < sel) {
        prev = temp;
        temp = temp->next;
        i++;
    }

    if (temp) {
        if (prev) prev->next = temp->next;
        else head = temp->next;
        free(temp->times);
        free(temp);
        printf("User deleted.\n");
    } else {
        printf("Invalid selection.\n");
    }
}
