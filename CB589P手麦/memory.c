#include"system.h"
#include"at24c08.h"
#include"memory.h"
#include"handler.h"
#include"KB4815.h"
 

extern tCbParam  	mCbParam;
extern tSqParam  	mSqParam;
extern tHmSetting  	mHmSetting;
extern tSysParam	    mSysParam;

/*--------EEPROM中读出的数据----------*/
extern Channel channel;


/*--------------------------------------------*/


/*-------------------------------------------------------------------------
*函数：saveData  保存数据
*参数：addr 地址  数据  
*返回值：无
*-------------------------------------------------------------------------*/
void saveData(u16 addr, u8 dat)
{
	u8 page;

	page = addr/256;
	switch(page)
	{
		case 0:page = AT24C08_PAGE0;break;
		case 1:page = AT24C08_PAGE1;break;
		case 2:page = AT24C08_PAGE2;break;
		case 3:page = AT24C08_PAGE3;break;
	}

	EA = 0;
	eepromWriteByte(page,addr%256,dat);
	EA = 1;	
}

/*-------------------------------------------------------------------------
*函数：loadData  读取数据
*参数：addr 地址   
*返回值：数据
*-------------------------------------------------------------------------*/
unsigned char loadData(u16 addr)
{
	u8 dat,page;

	page = addr/256;
	switch(page)
	{
		case 0:page = AT24C08_PAGE0;break;
		case 1:page = AT24C08_PAGE1;break;
		case 2:page = AT24C08_PAGE2;break;
		case 3:page = AT24C08_PAGE3;break;
	}

	EA = 0;
	dat = eepromReadByte(page,addr%256);
	EA = 1;	
	return dat;	
}
/*-------------------------------------------------------------------------
*函数：saveMenuParam  读取手麦设置信息
*参数： 无  
*返回值：无
*-------------------------------------------------------------------------*/
void saveMenuParam(void)
{
	saveData(EEP_LCD_COLOR,mHmSetting.LcdColor);	         //0x000E 保存LCD背景颜色 
	saveData(EEP_LED_SW,mHmSetting.ButtonLedSwitch);		   //0x000F 保存LED开关
	saveData(EEP_TONE_SW,mHmSetting.ButtonToneSwitch);     //0x0010 保存按键音
	saveData(EEP_SPK_SW,mHmSetting.SpkerSwitch);	         //0x0011 保存
}

/*-------------------------------------------------------------------------
*函数：saveAllParam  保存功能信息
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void saveAllParam(void)
{
	u32 fre=(u32)(channel.RX_Freq*1000);
	saveData(EEP_BASE,0xa5);
	saveData(EEP_FREQ_CAL,mCbParam.FreqCal);
	saveData(EEP_COUNTRY_TB,mCbParam.CountryTable);


	saveData(EEP_COUNTRY,mCbParam.Country);				
	saveData(EEP_BAND,mCbParam.Band);		
	saveData(EEP_CHANNEL,mCbParam.Channel);		
	saveData(EEP_MODU,mCbParam.Modu);		
	saveData(EEP_POWER,mCbParam.TxPower);		
	saveData(EEP_RFG,mCbParam.RfgLevel);			
	saveData(EEP_VOL,mCbParam.VolLevel);
	saveData(EEP_LAST_CH,mCbParam.LastChannel);
	
	saveData(EEP_LCD_COLOR,mHmSetting.LcdColor);	
	saveData(EEP_LED_SW,mHmSetting.ButtonLedSwitch);		
	saveData(EEP_TONE_SW,mHmSetting.ButtonToneSwitch);
	saveData(EEP_SPK_SW,mHmSetting.SpkerSwitch);	

	saveData(EEP_IS_ASQ,mSqParam.IsAsq);		
	saveData(EEP_SQ_LEVEL,mSqParam.SqLevel);			
	saveData(EEP_ASQ_LEVEL,mSqParam.AsqLevel);	

	saveData(EEP_HIT_POWER,mSysParam.HitPower);	
	saveData(EEP_DTMF,mDtmfRecive.dtmfCode);

	saveData(EEP_FRE,(u8)(fre>>24));
	saveData(EEP_FRE+1,(u8)((fre&0x00ff0000)>>16));
	saveData(EEP_FRE+2,(u8)((fre&0x0000ff00)>>8));
	saveData(EEP_FRE+3,(u8)(fre&0x000000ff));
	saveData(EEP_ISUK,mSysParam.isUK);
}

void saveDtmf()
{
	u32 fre=(u32)(channel.RX_Freq*1000);
	saveData(EEP_DTMF,mDtmfRecive.dtmfCode);
	saveData(EEP_FRE,(u8)(fre>>24));
	saveData(EEP_FRE+1,(u8)((fre&0x00ff0000)>>16));
	saveData(EEP_FRE+2,(u8)((fre&0x0000ff00)>>8));
	saveData(EEP_FRE+3,(u8)(fre&0x000000ff));
}
void loadToBK4815(void)
{
	
}



/*-------------------------------------------------------------------------
*函数：loadAllParam  加载功能信息
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void loadAllParam(void)
{
	u32 fre=0;
	if(loadData(EEP_BASE) != 0xa5)
	{
		setDefaultParam();
		saveAllParam();
	
		saveDtmf();
	}
	else
	{
		mCbParam.FreqCal = loadData(EEP_FREQ_CAL);
		mCbParam.CountryTable = loadData(EEP_COUNTRY_TB);
		mCbParam.Country = loadData(EEP_COUNTRY);
		mCbParam.Band = loadData(EEP_BAND);
		mCbParam.LastBand=mCbParam.Band;
		
		mCbParam.Channel = loadData(EEP_CHANNEL);
		mSysParam.LastChannel=mCbParam.Channel;
		mCbParam.LastChannel=0x09 ;
		mCbParam.Modu = loadData(EEP_MODU);
		mCbParam.RfgLevel = loadData(EEP_RFG);
		mCbParam.TxPower = loadData(EEP_POWER);
		mCbParam.VolLevel = loadData(EEP_VOL);
		mSqParam.IsAsq = loadData(EEP_IS_ASQ);
		mSqParam.AsqLevel = loadData(EEP_ASQ_LEVEL);
		mSqParam.SqLevel = loadData(EEP_SQ_LEVEL);
		if(mSqParam.IsAsq == ASQ_val)mCbParam.Sq = (mSqParam.AsqLevel | mSqParam.IsAsq);
		else mCbParam.Sq = mSqParam.SqLevel;
	
		mHmSetting.SpkerSwitch = loadData(EEP_SPK_SW);
		mHmSetting.LcdColor = loadData(EEP_LCD_COLOR);
		mHmSetting.ButtonLedSwitch = loadData(EEP_LED_SW);
		mHmSetting.ButtonToneSwitch = loadData(EEP_TONE_SW);
    
		mSysParam.LastChannel = loadData(EEP_LAST_CH);
		mSysParam.HitPower=loadData(EEP_HIT_POWER);
		mDtmfRecive.dtmfCode=loadData(EEP_DTMF);
		mSysParam.isUK=loadData(EEP_ISUK);
		fre=(((u32)loadData(EEP_FRE))<<24)|(((u32)loadData(EEP_FRE+1))<<16)|(((u32)loadData(EEP_FRE+2))<<8)|((u32)loadData(EEP_FRE+3));
		channel.RX_Freq=((float)fre/1000);				
	}
	mSqParam.Scan=1;
	mSqParam.ScanHould=1;		
	checkAllParam();
	
}
/*-------------------------------------------------------------------------
*函数：initMemory  初使加载功能信息
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void initMemory(void)
{
	initAt24c08();
	loadAllParam();	
}
