#include "trx.h"
#include "controls.h"
struct transceiver trx;

void *TRX_loop (void *arg)
{
while (1) {
	if (digitalRead(TRX)) trx.state=RX; else trx.state=TX;
	}
return 0;
}

void TRX_hwinit(void)
{
	pinMode(TRX,INPUT);
}
