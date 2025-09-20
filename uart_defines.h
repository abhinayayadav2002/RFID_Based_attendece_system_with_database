//uart_defines.h
#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define BAUD 9600
#define DIVISOR (PCLK/(16*BAUD))
//uxlcr defines
#define DLAB_BIT 7
//uxlsr defines
#define DRB 0
#define THRE 6
