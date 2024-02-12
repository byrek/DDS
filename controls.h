#include "../wiringPi/wiringPi/wiringPi.h"
#include "dds.h"

//hardware GPIO connections
#define QUADR_SWITCH	23
#define QUADR_IN1	22
#define QUADR_IN2	21
//state logic
#define QUADR_SW_ON	0
#define QUADR_SW_OFF	1

//quadr_state bits definition
#define QUADR_STATE_SW	2
#define QUADR_STATE_IN2	1
#define QUADR_STATE_IN1	0

unsigned int QUADR_read(void);
unsigned int QUADR_hwinit(void);
unsigned char quadr_state, quadr_state_new;
//menu selection
#define MOD_TUNING	1
#define MOD_STEP	2
#define MOD_IF_TUNING	3
#define MOD_VFO_SEL	4

//vfo selection
#define VFO_DIRECT	1
#define VFO_IF_PLUS	2
#define VFO_IF_MINUS	3


unsigned int QUADR_read(void);
unsigned int QUADR_hwinit(void);
void *Controls (void * arg );

//unsigned long dds_step;
unsigned char mode;
unsigned int vfo_mode;
