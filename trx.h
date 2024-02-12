#include <stdio.h>
#include "../wiringPi/wiringPi/wiringPi.h"

#define		TRX	27 //GPIO 27 for RX/TX key
#define 	RX	1
#define		TX	0

typedef struct transceiver
{
unsigned char state;
unsigned long swing;
} transceiver;

extern struct transceiver trx;
void *TRX_loop (void *arg);
void TRX_hwinit(void);
