#include <math.h>
#include <unistd.h>
#include "../wiringPi/wiringPi/wiringPi.h"

/* 0-70MHz DDS generator on AD9851 for Raspberry Pi
               Michal Byrecki SP5BYK
		   License: Creative Commons Attribution-NonCommercial 4.0 International		*/

//   HARDDWARE WIING FOR RASPBERRY PI-2 with 40-pole connector	*/
// Now, the GPIO addresses. This part may be quite confusing, as libwiringPi uses a different
// GPIO descriptors than the pictures published on the Internet (i.e. at RS Components).
// But as far as we're using the libwiringPi, we will stick to the libwiringPi coding.
// The coding way of libwiringPi can be found by executing "pintest" program, that will
// result with a nice, semi-graphic table showing names & states of each GPIO pin.

// GND supplied from 		PIN 39
// Power 3.3V supplied from 	PIN 1
#ifndef DDS_H__
#define DDS_H__

#define DDS_DATA	29	//GPIO 29 (pin 40)
#define DDS_CLK		28	//GPIO 27 (pin 38)
#define	DDS_ENA		25	//GPIO 25 (pin 37), FQ_UPLOAD
#define DDS_RST		24	//GPIO 24 (pin 35)
#define DDS_CLOCK	30000000UL
#define DDS_MUL_TRIGGER	10000000UL
#define DDS_FREQ_MAX	70000000UL
#define DDS_IF_MAX	DDS_FREQ_MAX	//dummy value for +/- intermediate frequency calculation
#define DDS_TUNING_STEP	1000
#define DDS_STEP_MAX 100000
#define DDS_SWING	1000UL

//initial values
#define DDS_INIT_STEP 	10000UL
#define DDS_INIT_FREQ	3500000UL
#define DDS_INIT_IFREQ	48000000UL

void *DDS_Update(void *arg);
void DDS_outZero (void);
void DDS_outOne (void);
void DDS_outByte(unsigned char byte);
void DDS_frequency (unsigned long frequency);
void DDS_hwinit (void);

typedef struct DDS
{
unsigned long frequency;
unsigned long ifrequency;
unsigned long step;
} DDS;

extern struct DDS dds;
#endif
