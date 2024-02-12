#include <math.h>
#include <unistd.h>
#include "../wiringPi/wiringPi/wiringPi.h"
#include "dds.h"
#include "trx.h"
#include "controls.h"

struct DDS dds;

void *DDS_Update (void *arg)
{
while (1) {
    if (trx.state==RX && vfo_mode==VFO_DIRECT && (dds.frequency>DDS_SWING)) DDS_frequency(dds.frequency-DDS_SWING);
    else DDS_frequency(dds.frequency);
    usleep(10);
    }
return 0;
}
void DDS_outZero (void)
{
digitalWrite (DDS_CLK,0);
digitalWrite (DDS_DATA,0);
digitalWrite (DDS_CLK,1);
}

void DDS_outOne (void)
{
digitalWrite(DDS_CLK,0);
digitalWrite(DDS_DATA,1);
digitalWrite(DDS_CLK,1);
digitalWrite(DDS_DATA,0);
}

void DDS_outByte(unsigned char byte)
{
int i;
for (i=0;i<8;i++)
    {
    if ((byte&1)==1) DDS_outOne();
    else DDS_outZero();
    byte=byte>>1;
    }

}

void DDS_frequency (unsigned long frequency)
{
unsigned long tuning_word;
int i;
    if (frequency>DDS_MUL_TRIGGER) tuning_word=((frequency/6)*pow(2,32))/DDS_CLOCK;
    else tuning_word=((frequency)*pow(2,32))/DDS_CLOCK;
    digitalWrite(DDS_ENA,0);
    for (i=0;i<32;i++)
	{
	if ((tuning_word & 1)==1) DDS_outOne();
	else DDS_outZero();
	tuning_word=tuning_word>>1;

	}
if (frequency>DDS_MUL_TRIGGER) DDS_outByte (0x09);
else DDS_outByte(0x0);
digitalWrite(DDS_ENA,1);
}

void DDS_hwinit (void)
{
pinMode (DDS_DATA,OUTPUT);
pinMode (DDS_CLK,OUTPUT);
pinMode (DDS_ENA,OUTPUT);
pinMode (DDS_RST,OUTPUT);
digitalWrite(DDS_RST,1);
digitalWrite(DDS_RST,0);
dds.frequency=DDS_INIT_FREQ;
dds.ifrequency=DDS_INIT_IFREQ;
dds.step=DDS_INIT_STEP;
}



