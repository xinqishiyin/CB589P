#include "system.h"
#include "TM1722.h"
#include "handler.h"
#include "KB4815.h"
extern unsigned long   	mMessage;
extern tFlag  	      	mFlag;
extern tHmSetting     	mHmSetting;

extern tSqParam       	mSqParam;
extern tCbParam       	mCbParam;
extern tSysParam	      mSysParam;
extern Channel channel;
void initHandler(void)
{

	if(VCC_DET)
	{
		IE |=0X10;        //开串口中断	
		mFlag.SysMode = SYS_MODE_LINE;
	}
	else 
	{
		
		mFlag.SysMode = SYS_MODE_WIRELESS;	
		IE &=~0X10;        //关串口中断	
	}
}

void clearFlag(void)
{
	mFlag.Mute = 0;

	mFlag.Emg = 0;
	mFlag.SpkOpen = 0;
	mFlag.ChangeTxPower = 0;
	mFlag.BandChanged = 0;
	mMessage = 0;
}
/*-------------------------------------------------------------------------
*函数：showLcdLed  显示背景颜色
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void showLcdLed(void)
{
	P0 &= 0x1f;
	P0 |= mHmSetting.LcdColor<<5;
}
/*-------------------------------------------------------------------------
*函数：showButtonLed  显示按键灯
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void showButtonLed(void)
{
	LCD_LED = mHmSetting.ButtonLedSwitch;
}



/*-------------------------------------------------------------------------
*函数：isModuConstant  AM/FM调制
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
uchar isModuConstant(void) //调制是固定的，不能变
{
	if(mCbParam.CountryTable == 2)
	{
		mCbParam.Modu = AM;
		return 1;
	}
	else if(mCbParam.Country == COUNTRY_CE || mCbParam.Country == COUNTRY_UK)
	{
		mCbParam.Modu = FM;
		return 1;
	}
	else if(mCbParam.Country == COUNTRY_DE)
	{
		if(mCbParam.Channel > 40) 
		{
			mCbParam.Modu = FM;
			return 1;
		}
	}
	return 0;
}

/*-------------------------------------------------------------------------
*函数：setDefaultParam  默认设置
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void setDefaultParam(void)
{
	clearFlag();
	mCbParam.FreqCal = 20;
	mCbParam.CountryTable = 1;
	mCbParam.Country = COUNTRY_EU;
	mCbParam.Band = 0;
	mCbParam.Channel = 9;
	mCbParam.Modu = FM;
	mCbParam.RfgLevel = 0;
	mCbParam.TxPower = POWER_LOW;
	mCbParam.VolLevel = 5;
	mCbParam.Sq=10;
	mCbParam.LastChannel=mCbParam.Channel ;
	mSqParam.IsAsq = SQ_val;
	mSqParam.AsqLevel = 3;
	mSqParam.SqLevel = 10;
	channel.RX_Freq=260.015;
	mSqParam.Scan=0;
	mSqParam.ScanDir=0;
	mSqParam.ScanHould=1;
	mSqParam.DWSet=0;
	//mSystem.isMute=0;
  mSysParam.isUK=0;
	mHmSetting.SpkerSwitch = 1;
	mHmSetting.LcdColor = 7;
	mHmSetting.ButtonLedSwitch = 1;
	mHmSetting.ButtonToneSwitch = 1;
	
	mSysParam.LastChannel = mCbParam.Channel;
	mSysParam.HitPower=0;
	
  
	
	
	
}
/*-------------------------------------------------------------------------
*函数：checkAllParam  验证加载信息
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void checkAllParam(void)
{
	
	mSysParam.MinChannel = 1;
	switch(mCbParam.Country)
	{
		case COUNTRY_EU: mSysParam.MaxChannel = 40;mCbParam.Band=0;mSysParam.isUK=0;break;
		case COUNTRY_CE: mSysParam.MaxChannel = 40;mCbParam.Band=0;break;
		case COUNTRY_UK: mSysParam.MaxChannel = 40;mCbParam.Band=0;mSysParam.isUK=0;break;
		case COUNTRY_PL: mSysParam.MaxChannel = 40;mCbParam.Band=0;mSysParam.isUK=0;break;
		case COUNTRY_I0: mSysParam.MaxChannel = 40;mCbParam.Band=0;mSysParam.isUK=0;break;
		case COUNTRY_AU: mSysParam.MaxChannel = 40;mCbParam.Band=0;mSysParam.isUK=0;break;
		case COUNTRY_NL: mSysParam.MaxChannel = 40;mCbParam.Band=0;mSysParam.isUK=0;break;
		case COUNTRY_RU:
		{		
			if(mCbParam.Band>9) mCbParam.Band=0;
			mSysParam.MaxChannel = 40;
			mSysParam.isUK=0;
		}
			break;
		case COUNTRY_PX: 
			{
				if(mCbParam.Band>9) mCbParam.Band=0;
				mSysParam.MaxChannel = 40;
				mSysParam.isUK=0;
			}
			break;
		case COUNTRY_I2: mSysParam.MaxChannel = 34;mCbParam.Band=0;mSysParam.isUK=0;break;
		case COUNTRY_DE: mSysParam.MaxChannel = 80;mCbParam.Band=0;mSysParam.isUK=0;break;
		case COUNTRY_IN: mSysParam.MaxChannel = 27;mCbParam.Band=0;mSysParam.isUK=0;break;
		case COUNTRY_PC: mSysParam.MaxChannel = 50;mCbParam.Band=0;mSysParam.isUK=0;break;
		case COUNTRY_AM: mSysParam.MaxChannel = 10;mCbParam.Band=0;mSysParam.isUK=0;break;
	}
	
	if(mSysParam.isUK>1)mSysParam.isUK=0;
if(mCbParam.CountryTable > 3)mCbParam.CountryTable = 1;	
else if (mCbParam.CountryTable==0) mCbParam.CountryTable = 1;	
	if((mCbParam.Country > 13) || (mCbParam.Country < 0))mCbParam.Country = 0;	 

		
	mSysParam.ActivityChannel = mCbParam.Channel;
	if((mCbParam.Channel > mSysParam.MaxChannel) || (mCbParam.Channel < mSysParam.MinChannel))mCbParam.Channel = mSysParam.MinChannel;
	

	if((mSysParam.LastChannel < mSysParam.MinChannel) || (mSysParam.LastChannel > mSysParam.MaxChannel))
	{
		mSysParam.LastChannel = mCbParam.Channel;
		mSysParam.ActivityChannel= mSysParam.LastChannel;
	}
	
	if(mCbParam.VolLevel > 9) mCbParam.VolLevel = 5;
	
	if(mCbParam.RfgLevel > 9)mCbParam.RfgLevel =9;	

	if(mCbParam.TxPower > POWER_HIGH)mCbParam.TxPower = POWER_HIGH;

	if((mSqParam.IsAsq > 1) )mSqParam.IsAsq = 0;
	if(mSqParam.AsqLevel > 5) mSqParam.AsqLevel = 1;
	if(mSqParam.SqLevel > 28) mSqParam.SqLevel = 10;
	if(mSqParam.IsAsq==1) mCbParam.Sq = mSqParam.AsqLevel | 0x20;
	else mCbParam.Sq = mSqParam.SqLevel;
 
	if(channel.RX_Freq<260)channel.RX_Freq=260;
	else if(channel.RX_Freq>380)channel.RX_Freq=260;
	if(mHmSetting.LcdColor > 7) mHmSetting.LcdColor = 7;
	
	if((mDtmfRecive.dtmfCode&0xf0)==0) mDtmfRecive.dtmfCode|=0x1f;
  if(mSysParam.HitPower>0) mSysParam.HitPower=1;
	mCbParam.UartTxBuf[0] = 0xFE;
	mSysParam.isMute=0;
	mCbParam.FreqCal =20;
	mSysParam.isLastChannel=0;
	mSysParam.Rssi=0;
	mFlag.SpkOpen=0;
	mDtmfRecive.DtmfSussece=0;
	
}

void CheckTxPower()
{
	if(mCbParam.CountryTable==1&&mCbParam.Country==COUNTRY_EU&&mCbParam.Modu==AM)
	{
		mCbParam.TxPower=POWER_1W;
	}
	else if((mCbParam.CountryTable==2||mCbParam.CountryTable==3)&&mSysParam.HitPower==1)
	{
		mCbParam.TxPower=POWER_HIGH;
	}
	else mCbParam.TxPower=POWER_LOW;
}

