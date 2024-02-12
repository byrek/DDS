#include "controls.h"

//menu selection
#define MOD_TUNING	1
#define MOD_STEP	2
#define MOD_IF_TUNING	3
#define MOD_VFO_SEL	4

//vfo selection
#define VFO_DIRECT	1
#define VFO_IF_PLUS	2
#define VFO_IF_MINUS	3


/* 0-70MHz DDS generator on AD9851 for Raspberry Pi
               Michal Byrecki SP5BYK
		   License: GPL					*/

//   HARDDWARE WIING FOR RASPBERRY PI-2 with 40-pole connector	*/
// Now, the GPIO addresses. This part may be quite confusing, as libwiringPi uses a different
// GPIO descriptors than the pictures published on the Internet (i.e. at RS Components).
// But as far as we're using the libwiringPi, we will stick to the libwiringPi coding.
// The coding way of libwiringPi can be found by executing "pintest" program, that will
// result with a nice, semi-graphic table showing names & states of each GPIO pin.

unsigned int QUADR_read(void)
{
	unsigned char temp;
	temp=digitalRead(QUADR_SWITCH);
	temp=temp<<1;
	temp|=digitalRead(QUADR_IN2);
	temp=temp<<1;
	temp|=digitalRead(QUADR_IN1);
	return temp;
}

unsigned int QUADR_hwinit(void)
{
	pinMode(QUADR_IN1,INPUT);
	pinMode(QUADR_IN2,INPUT);
	pinMode(QUADR_SWITCH,INPUT);
	return QUADR_read();
}

void *Controls (void * arg )
{
while (1)
{
//tuning step selection procedure
quadr_state_new=QUADR_read();
if ((0x01&(quadr_state_new>>QUADR_STATE_SW))==QUADR_SW_ON && (0x01&(quadr_state>>QUADR_STATE_SW))==QUADR_SW_OFF)
	{
		switch (mode) {
				case MOD_TUNING: mode=MOD_STEP; break;
				case MOD_STEP: mode=MOD_IF_TUNING; break;
				case MOD_IF_TUNING: mode=MOD_VFO_SEL; break;
				case MOD_VFO_SEL: mode=MOD_TUNING; break;
				}
	}
if (mode==MOD_TUNING)
{
    if (((0x03&quadr_state)==0x00 && (0x03&quadr_state_new)==0x01) || ((0x03&quadr_state)==0x03 && (0x03&quadr_state_new)==0x02))
		{
		//turn left
		switch (vfo_mode)
		    {
		    case VFO_DIRECT: if ((dds.frequency+dds.step)<DDS_FREQ_MAX) dds.frequency+=dds.step; else dds.frequency=DDS_FREQ_MAX; break;
		    case VFO_IF_PLUS: if ((dds.frequency+dds.step)<DDS_FREQ_MAX) dds.frequency+=dds.step; else dds.frequency=DDS_FREQ_MAX; break;
		    case VFO_IF_MINUS: if (dds.frequency>dds.step) dds.frequency-=dds.step; else dds.frequency=0; break;
		    }
		}
    if (((0x03&quadr_state)==0x02 && (0x03&quadr_state_new)==0x03) || ((0x03&quadr_state)==0x01 && (0x03&quadr_state_new)==0x00))
		{
		//turn right
		switch (vfo_mode)
		    {
		    case VFO_DIRECT: if ((dds.frequency>dds.step)) dds.frequency-=dds.step; else dds.frequency=0; break;
		    case VFO_IF_PLUS: if (dds.frequency>dds.step) dds.frequency-=dds.step; else dds.frequency=0; break;
		    case VFO_IF_MINUS: if ((dds.frequency+dds.step<dds.ifrequency)) dds.frequency+=dds.step; else dds.frequency=dds.ifrequency; break;
		    }
		}
    }
if (mode==MOD_IF_TUNING)
{
    if (((0x03&quadr_state)==0x00 && (0x03&quadr_state_new)==0x01) || ((0x03&quadr_state)==0x03 && (0x03&quadr_state_new)==0x02))
		{
		if ((dds.ifrequency+dds.step)<DDS_IF_MAX) dds.ifrequency+=dds.step; else dds.ifrequency=DDS_IF_MAX;
		}
    if (((0x03&quadr_state)==0x02 && (0x03&quadr_state_new)==0x03) || ((0x03&quadr_state)==0x01 && (0x03&quadr_state_new)==0x00))
		{
		if (dds.ifrequency>dds.step) dds.ifrequency-=dds.step; else dds.ifrequency=0;		
		}
    }
if (mode==MOD_STEP)
{
    if (((0x03&quadr_state)==0x00 && (0x03&quadr_state_new)==0x01) || ((0x03&quadr_state)==0x03 && (0x03&quadr_state_new)==0x02))
		{
		if (dds.step<DDS_STEP_MAX) dds.step*=10; else dds.step=1;
		}
    if (((0x03&quadr_state)==0x02 && (0x03&quadr_state_new)==0x03) || ((0x03&quadr_state)==0x01 && (0x03&quadr_state_new)==0x00))
		{
		if (dds.step>=10) dds.step/=10; else dds.step=DDS_STEP_MAX;
		}
    }
if (mode==MOD_VFO_SEL)
{
    if (((0x03&quadr_state)==0x00 && (0x03&quadr_state_new)==0x01) || ((0x03&quadr_state)==0x03 && (0x03&quadr_state_new)==0x02))
		{
		switch (vfo_mode)
		    {
		    case VFO_DIRECT: vfo_mode=VFO_IF_PLUS; break;
		    case VFO_IF_PLUS: vfo_mode=VFO_IF_MINUS; break;
		    case VFO_IF_MINUS: vfo_mode=VFO_DIRECT; break;
		    }
		}
    if (((0x03&quadr_state)==0x02 && (0x03&quadr_state_new)==0x03) || ((0x03&quadr_state)==0x01 && (0x03&quadr_state_new)==0x00))
		{
		switch (vfo_mode)
		    {
		    case VFO_DIRECT: vfo_mode=VFO_IF_MINUS; break;
		    case VFO_IF_MINUS: vfo_mode=VFO_IF_PLUS; break;
		    case VFO_IF_PLUS: vfo_mode=VFO_DIRECT; break;
		    }
		}
    }
quadr_state=quadr_state_new;
    }
    return 0;
}

