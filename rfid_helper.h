// rfid_helper.h

#ifndef RFID_HELPER_H
#define RFID_HELPER_H

#define MAX_PAIRS 10

typedef struct InOutTime {
    char in[10];
    char out[10];
} InOutTime;

typedef struct AttendanceNode {
    int sno;
    char id[20];
    char name[50];
    char date[20];
    char status[5];  // IN / OUT
    int total_minutes;
    InOutTime* times;
    int pair_count;
    struct AttendanceNode* next;
} AttendanceNode;

// Global list head
extern AttendanceNode* head;

// Linked list operations
AttendanceNode* find_entry(const char* id, const char* date);
AttendanceNode* find_user_by_id(const char* id);
void append_entry(AttendanceNode* node);
void add_new_user_entry(const char* id, const char* name, const char* date, const char* time);

// CSV I/O
void load_from_csv();
void save_to_csv();

// Time utilities
void get_current_time(char* date_buf, char* time_buf);
int calculate_duration(char* in_time, char* out_time);
void format_duration(int minutes, char* buffer);

// Attendance logic
void process_user_card(AttendanceNode* );
void list_users();
void add_user();
void edit_user();
void delete_user();

#endif
