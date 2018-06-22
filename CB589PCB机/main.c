#include "SC51F2832.H"
#include "system.h"
#include "XN31202.h"
#include "event.h"
#include "m62429.h"
#include "vco.h"
#include "KB4815.h"
#include "memory.h"


u8 ini;
u8 isPowerOn=0;
u16 timePowOn=0;
void main()
{	
	mFlag.VcoIdle=1;

	mFlag.CbInit=0;
	SystemInit();	
	ini=BK4815_INIT;
	initM62429();
	initXN31202();
  SetBK4815Pragram();
	initMemory();
	CLS_RX_EN;
	CLS_TX_EN;
	Set_Mute();
	setEmission(0);
	//delayms(1000);
	while(1)
	{
		initHandler();
		eventHandler();
	}
}

void	INT0_Irq(void)	interrupt 0
{

	mRecive=MRECIVE_BK4815_INTERUPT;
	
	
	ACC = EINTCS0;
    EINTCS0 &= 0xfc;	
    ACC = EINTCS0;                 //建议2次清除中断标志 或者在清0前后各读一次标志位
    EINTCS0 &= 0xfc;
    //P01=!P01;
}