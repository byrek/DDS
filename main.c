
/* 0-70MHz DDS generator on AD9851 for Raspberry Pi
               Michal Byrecki SP5BYK
		   License: GPL					*/

//   HARDDWARE WIING FOR RASPBERRY PI-2 with 40-pole connector	*/
// Now, the GPIO addresses. This part may be quite confusing, as libwiringPi uses a different
// GPIO descriptors than the pictures published on the Internet (i.e. at RS Components).
// But as far as we're using the libwiringPi, we will stick to the libwiringPi coding.
// The coding way of libwiringPi can be found by executing "pintest" program, that will
// result with a nice, semi-graphic table showing names & states of each GPIO pin.

#include <stdio.h>
#include <pthread.h>
#include "controls.h"
#include "dds.h"
#include "trx.h"

#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
int xmax, ymax;

//menu selection
#define MOD_TUNING	1
#define MOD_STEP	2
#define MOD_IF_TUNING	3
#define MOD_VFO_SEL	4

//vfo selection
#define VFO_DIRECT	1
#define VFO_IF_PLUS	2
#define VFO_IF_MINUS	3

#define NUM_THREADS	3
pthread_t	threads[NUM_THREADS];
int	td_creation_result;

//ncurses
int xmax,ymax;
const char win_top[]="Frequency synthesizer by Michal Byrecki SP5BYK";


int main (int argc, char *argv[])
{
vfo_mode=VFO_DIRECT;
mode=MOD_TUNING;

if (wiringPiSetup () == -1) { printf ("Error: error initializing Raspi GPIO\n"); return -1; }

//Hardware initialisation
//	DDS
DDS_hwinit();
//Quadrature encoder
quadr_state=QUADR_hwinit();
//Transceiver keying of RX/TX
TRX_hwinit();


// Thread creation that will controll of the operator's panel
td_creation_result=pthread_create(&threads[0],NULL,Controls,(void *)NULL);
if (td_creation_result) 
	{
	perror ("Error creating thread: %d, exiting");
	return -1;
	}
// Thread creation that will update DDS synthesizer
td_creation_result=pthread_create(&threads[1],NULL,DDS_Update,(void *)NULL);
if (td_creation_result) 
	{
	perror ("Error creating thread: %d, exiting");
	return -1;
	}
// Thread creation that will process RX/TX keying
td_creation_result=pthread_create(&threads[2],NULL,TRX_loop,(void *)NULL);
if (td_creation_result) 
	{
	perror ("Error creating thread: %d, exiting");
	return -1;
	}
initscr();
getmaxyx(stdscr,ymax,xmax);
while (1)
{
clear();
mvprintw(1,(xmax-sizeof(win_top))/2,"%s",win_top);
if (vfo_mode==VFO_DIRECT) mvprintw(2,1,"FREQ: %02lu.%03lu.%03lu Hz",(dds.frequency/1000000),(dds.frequency/1000)%1000,dds.frequency%1000);
if (vfo_mode==VFO_IF_PLUS) mvprintw(2,1,"FREQ: %02lu.%03lu.%03lu Hz",((dds.ifrequency+dds.frequency)/1000000),((dds.ifrequency+dds.frequency)/1000)%1000,(dds.ifrequency+dds.frequency)%1000);
if (vfo_mode==VFO_IF_MINUS) mvprintw(2,1,"FREQ: %02lu.%03lu.%03lu Hz",((dds.ifrequency-dds.frequency)/1000000),((dds.ifrequency-dds.frequency)/1000)%1000,(dds.ifrequency-dds.frequency)%1000);
if (mode==MOD_IF_TUNING) mvprintw(2,1,"IF FREQ: %02lu.%03lu.%03lu Hz",(dds.ifrequency/1000000),(dds.ifrequency/1000)%1000,dds.ifrequency%1000);
if (dds.step>=1000) mvprintw(3,1,"STEP: %lu kHz",dds.step/1000); else mvprintw(3,1,"STEP: %lu Hz",dds.step);
switch (vfo_mode)
    {
    case VFO_DIRECT:
	mvprintw(5,1,"VFO: DIRECT\tSWING:%lu Hz",DDS_SWING); break;
    case VFO_IF_PLUS:
	mvprintw(5,1,"VFO: IF PLUS FREQ"); break;
    case VFO_IF_MINUS:
	mvprintw(5,1, "VFO: IF MINUS FREQ"); break;

    }
switch (mode)
    {
    case MOD_TUNING:
	mvprintw(8,1,"MODE: TUNING"); break;
    case MOD_STEP:
	mvprintw(8,1,"MODE: STEP SELECTION"); break;
    case MOD_IF_TUNING:
	mvprintw(8,1,"MODE: IF FREQUENCY"); break;
    case MOD_VFO_SEL:
	mvprintw(8,1,"MODE: VFO SELECTION"); break;

    }
if (trx.state==RX) mvprintw(12,1,"RX"); else mvprintw(12,1,"TX");

mvprintw(10,1,"IFREQ: %lu FREQ: %lu A-B=%lu",dds.ifrequency,dds.frequency,dds.ifrequency-dds.frequency);
refresh();
usleep(250);
}
pthread_exit(NULL);
clear();

return 0 ;
}
