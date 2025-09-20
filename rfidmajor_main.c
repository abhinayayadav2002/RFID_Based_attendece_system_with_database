// rfid_main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "rfid_helper.h"

#define UART_DEV "/dev/ttyUSB0"
#define BUFFER_SIZE 128

// -------- UART Setup --------
int open_uart() {
    int fd = open(UART_DEV, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("Unable to open UART device");
        return -1;
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD);  // Enable receiver
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;               // 8 data bits
    options.c_cflag &= ~PARENB;           // No parity
    options.c_cflag &= ~CSTOPB;           // 1 stop bit
    options.c_cflag &= ~CRTSCTS;          // No hardware flow control
    tcsetattr(fd, TCSANOW, &options);

    return fd;
}

// -------- Admin Menu --------
void admin_menu(int uart_fd) {
    int choice;
    do {
        printf("\n------ ADMIN MENU ------\n");
        printf("1. Add User\n");
        printf("2. Edit User\n");
        printf("3. Delete User\n");
        printf("4. List Users\n");
        printf("5. Exit Admin Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_user(); break;
            case 2: edit_user(); break;
            case 3: delete_user(); break;
            case 4: list_users(); break;
        }
    } while (choice != 5);

    // Send ADMIN_EXIT$ acknowledgment to controller
    write(uart_fd, "ADMIN_EXIT$\n", strlen("ADMIN_EXIT$\n"));
    printf("Sent 'ADMIN_EXIT$' to controller\n");
}

// -------- Main Loop --------
int main() {
    printf("Initializing RFID Attendance System...\n");

    load_from_csv();
    printf("after load_from_csv\n");

    int uart_fd = open_uart();
    if (uart_fd < 0) return 1;

    char date[20], time[10];
    get_current_time(date, time);
    printf("sync time&date\n");
    write(uart_fd,time, strlen(time));
    write(uart_fd,"\n", strlen("\n"));
    printf("sent time to controller\n");
    sleep(1);
    write(uart_fd,date, strlen(date));
    write(uart_fd,"\n", strlen("\n"));
    printf("Sent date to controller\n");
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int n = read(uart_fd, buffer, BUFFER_SIZE - 1);
        if (n <= 0) continue;
	/*printf("Enter your card number:\n");
	scanf("%s", buffer);
	printf("buffer='%s',len=%lu\n",buffer, strlen(buffer));
	int n = strlen(buffer) + 1;

        buffer[n] = '\0';*/
        printf("\nReceived: %s\n", buffer);

        if (buffer[0] == 'A') {
            char admin_id[20];
            sscanf(buffer, "A%[^$]$", admin_id);
            printf("ADMIN card detected: %s\n", admin_id);
            admin_menu(uart_fd);
        }
        else if (buffer[0] == 'U') {
            char user_id[20];
            sscanf(buffer, "U%[^$]$", user_id);
            printf("USER card detected: %s\n", user_id);

            AttendanceNode* user = find_user_by_id(user_id);
            if (user) {
                process_user_card(user);
                write(uart_fd, "USER_OK$\n", strlen("USER_OK$\n"));
                printf("Sent 'USER_OK$' to controller\n");
            } else {
                write(uart_fd, "USER_NOT_FOUND$\n", strlen("USER_NOT_FOUND$\n"));
                printf("Sent 'USER_NOT_FOUND$' to controller\n");
            }
        }
        else {
            printf("Unknown card format received.\n");
        }

        // Always sync data after processing
        save_to_csv();
    }

    close(uart_fd);
    return 0;
}
