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



/*-------------------------------------------------------------------------
*函数：irq_timer1  50ms定时中断
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void irq_timer1(void) interrupt 1
{  
	mParameter.sendDtmfT--;
	if(0==mParameter.sendDtmfT)
	{		
		 mParameter.isSendDtmf=1;
	   mParameter.sendDtmfT=SendDtmfTime;
	}
	mParameter.CheckRssi++;
	if(mParameter.CheckRssi>2)
	{
	 mParameter.isCheckRssi=1;
		mParameter.CheckRssi=0;
	}
	if(mParameter.isButtonTone==1)
	{
			mParameter.ButtonToneTime--;
		  if(mParameter.ButtonToneTime==0)
			{
				xPWMCN &= ~0x10;
        if(mFlag.SpkOpen==0||mHmSetting.SpkerSwitch==0)
				{					
					SPK_EN=0;
				}
				mParameter.isButtonTone=0;
			}
  }
	
	if(mSqParam.DWSet==2||mSqParam.Scan!=1)
	{
		if(mParameter.isScanInrupt>0)
		{
			mParameter.isScanInrupt--;
		}
	}
	if(mKey.Pow_Press==1)
	{
		if(mKey.Pow_Press_Time>0)
		{
			mKey.Pow_Press_Time--;
		}
	}
	if(mHmSetting.isPowLow==1)
	{
		mHmSetting.PowLowShutTime++;
	}
	
	if(mRecive.Sussece==1)
	{
		mRecive.RecvCount ++;
		if(mRecive.RecvCount>10)
		{
			mRecive.Errer	= 1;		
		}
	}
	else
	{
		mRecive.RecvCount = 0;
	}
	
	
		TR0 = 0;
		TH0 = (65535 - 3125) / 256;	
		TL0 = (65535 - 3125) % 256;	//1ms		
  	TR0 = 1; 
}


void waitPowerOn(void)
{
	while(mParameter.isPowerOn==0)
	{
		if(POW_IN)
		{
			mParameter.timePowOn++;
			delayms(30);
			if(mParameter.timePowOn>50)
			{
				mParameter.isPowerOn=1;
			}
		}
		else
		{
			mParameter.timePowOn=0;
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
		SystemInit();
	  EA=0;
    mParameter.isPowerOn=0;	
		initMemory();	
	  
	  InitKey();
		waitPowerOn();
    	CheckHitPowerPress();
	  LoadLCDBeep();		
	  initLCD();		
		SetBK4815Pragram();	  
		
		InitMenu();
		IE |=0X10;        //开串口中断	
	  EA=1;
	  while(POW_IN);
    while(1)
		{
			initHandler();
			evenHandler();
			if(mFlag.SysMode == SYS_MODE_LINE)
			{				
				if(VCC_BATT==0&&mParameter.isBattShow==0)
				{
					mParameter.isBattShow=1;
					//LED_TX=ON;
					LCD_BATT(1);
				}
				else if(VCC_BATT==1&&mParameter.isBattShow==1)
				{
					mParameter.isBattShow=0;
					//LED_TX=OFF;
					LCD_BATT(0);
				}
			}
		}
}

