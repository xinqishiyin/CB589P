#include "SC51F2832.H"
#include "system.h"
#include "XN31202.h"
#include "event.h"
#include "m62429.h"
#include "vco.h"
#include "KB4815.h"
#include "memory.h"


void main()
{	
	mFlag.VcoIdle=1;

	mFlag.CbInit=0;
	SystemInit();	
	//ini=BK4815_INIT;
	initM62429();
	initXN31202();
  SetBK4815Pragram();
	initMemory();
	CLS_RX_EN;
	CLS_TX_EN;
	Set_Mute();
	setEmission(0);
	delayms(1000);
	POWER_ON_EN = 1;
	
	while(1)
	{
		initHandler();
		eventHandler();
	}
}
