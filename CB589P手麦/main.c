#include "SC51F2832.H"
#include "system.h"
#include "CB_Line.h"
#include "memory.h"
#include "TM1722.h"
#include "lcd.h"
#include "uart.h"
#include "handler.h"
#include "Menu.h"   
#include "Keys.h"
#include "KB4815.h"
extern tSysParam mSysParam;
extern tCbParam mCbParam;
extern tHmSetting mHmSetting;

u8 sendDtmfT=SendDtmfTime;
u8 ButtonToneTime=BUTTON_TIME;
u8 isButtonTone=0;
u8 isScanInrupt=SCAN_SPEED_DELAY;
u16 timePowOn=0;
/*-------------------------------------------------------------------------
*函数：irq_timer1  50ms定时中断
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void irq_timer1(void) interrupt 1
{  
	sendDtmfT--;
	if(0==sendDtmfT)
	{		
		 isSendDtmf=1;
	   sendDtmfT=SendDtmfTime;
	}
	if(isButtonTone==1)
	{
			ButtonToneTime--;
		  if(ButtonToneTime==0)
			{
				xPWMCN &= ~0x10;
        if(mFlag.SpkOpen==0||mHmSetting.SpkerSwitch==0)
				{					
					SPK_EN=0;
				}
				isButtonTone=0;
			}
  }
	
	if(mSqParam.DWSet==2||mSqParam.Scan!=1)
	{
		if(isScanInrupt>0)
		{
			isScanInrupt--;
		}
	}		
		TR0 = 0;
		TH0 = (65535 - 3125) / 256;	
		TL0 = (65535 - 3125) % 256;	//1ms		
  	TR0 = 1; 
}


void waitPowerOn(void)
{
	while(isPowerOn==0)
	{
		if(POW_IN)
		{
			timePowOn++;
			delayms(30);
			if(timePowOn>50)
			{
				isPowerOn=1;
			}
		}
		else
		{
			timePowOn=0;
		}
	}
	
	
	POW_OUT=1;
		
	//delayms(100);
	//CHANNEL_FUC();
	//checkTxPower();
}

void CheckHitPowerPress()
{
	u8 i;
	i=PPT_KEY;
	mHmSetting.isCheckHitPower=0;
	if(	mCbParam.CountryTable==2||mCbParam.CountryTable==3)//&&PPT_KEY==0&&Get_AD(Key2)==0x0a&&Get_AD(Key1)==0x03)
	{
		if(PPT_KEY==0)
		{			
		if(Get_AD(Key2)==0x0a||Get_AD(Key2)==0x09)
		{
			if(Get_AD(Key1)==0x03)
			{
					mHmSetting.isCheckHitPower=1;
			}}}
	}		
}

void main()
{
	 u8 dat;
		SystemInit();
	  EA=0;
    isPowerOn=0;	
		initMemory();
		isSendDtmf=0;
	  CheckHitPowerPress();
		waitPowerOn();		
	  LoadLCDBeep();		
	  initLCD();		
		SetBK4815Pragram();	  
		InitKey();
		InitMenu();
		IE |=0X10;        //开串口中断	
	  EA=1;
	  while(POW_IN);
    while(1)
		{
			initHandler();
			evenHandler();
			if(mFlag.SpkOpen==0&&mFlag.SysMode == SYS_MODE_LINE)
			{
				dat=xP4;
				if(VCC_BATT==0)
				{
					//LED_TX=ON;
					LCD_BATT(1);
				}
				else
				{
					//LED_TX=OFF;
					LCD_BATT(0);
				}
			}
		}
}
void	INT0_Irq(void)	interrupt 2
{
 
	mRecive=MRECIVE_BK4815_INTERUPT;
	

    EINTCS0 &= 0xfc;	
                     //建议2次清除中断标志 或者在清0前后各读一次标志位
    EINTCS0 &= 0xfc;
    //P01=!P01;
}
