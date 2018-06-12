#include"system.h"
#include"at24c08.h"
#include"memory.h"

#include"KB4815.h"
#include "XN31202.H"
#include "vco.h"
#include "event.h"
extern tCbParam  	mCbParam;
extern tSqParam  	mSqParam;



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
*函数：saveAllParam  保存功能信息
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void saveAllParam(void)
{	
	u32 fre=(u32)(channel.RX_Freq*1000);
	saveData(EEP_BASE,0xa5);
	saveData(EEP_FREQ_CAL,mCbParam.FreqCal);
	saveData(EEP_COUNTRY,mCbParam.Country);				
	saveData(EEP_BAND,mCbParam.Band);		
	saveData(EEP_CHANNEL,mCbParam.Channel);		
	saveData(EEP_MODU,mCbParam.Modu);		
	saveData(EEP_POWER,mCbParam.TxPower);		
	saveData(EEP_RFG,mCbParam.RfgLevel);			
	saveData(EEP_VOL,mCbParam.VolLevel);	
	
	saveData(EEP_IS_ASQ,mSqParam.IsAsq);		
	saveData(EEP_SQ_LEVEL,mSqParam.SqLevel);			
	saveData(EEP_ASQ_LEVEL,mSqParam.AsqLevel);			
	saveData(EEP_DTMF,mDtmfRecive.dtmfCode);
	saveData(EEP_FRE,(u8)(fre>>24));
	saveData(EEP_FRE+1,(u8)((fre&0x00ff0000)>>16));
	saveData(EEP_FRE+2,(u8)((fre&0x0000ff00)>>8));
	saveData(EEP_FRE+3,(u8)(fre&0x000000ff));

	
}
void saveSQSet()
{
	u8 i;
	for(i=0;i<28;i++)
	{
		saveData(EEP_SQ_OPEN_SET+i,mSqParam.SqOpenSet[i]);
		saveData(EEP_SQ_CLOSE_SET+i,mSqParam.SqCloseSet[i]);
	}	
	
	saveData(EEP_ASQ_OPEN_SET,mSqParam.AsqOpenSet[0]);
	saveData(EEP_ASQ_OPEN_SET+1,mSqParam.AsqOpenSet[1]);
	saveData(EEP_ASQ_OPEN_SET+2,mSqParam.AsqOpenSet[2]);
	saveData(EEP_ASQ_OPEN_SET+3,mSqParam.AsqOpenSet[3]);
	saveData(EEP_ASQ_OPEN_SET+4,mSqParam.AsqOpenSet[4]);
	saveData(EEP_ASQ_OPEN_SET+5,mSqParam.AsqOpenSet[5]);
	
	saveData(EEP_ASQ_CLOSE_SET,mSqParam.AsqCloseSet[0]);
	saveData(EEP_ASQ_CLOSE_SET+1,mSqParam.AsqCloseSet[1]);
	saveData(EEP_ASQ_CLOSE_SET+2,mSqParam.AsqCloseSet[2]);
	saveData(EEP_ASQ_CLOSE_SET+3,mSqParam.AsqCloseSet[3]);
	saveData(EEP_ASQ_CLOSE_SET+4,mSqParam.AsqCloseSet[4]);
	saveData(EEP_ASQ_CLOSE_SET+5,mSqParam.AsqCloseSet[5]);
	
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
*函数：checkAllParam  验证加载信息
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void checkAllParam(void)
{
	u8 i;
	u8 go_flag=1;
	if(mCbParam.VolLevel > 9) mCbParam.VolLevel = 5;
	
	if(mCbParam.RfgLevel > 54)mCbParam.RfgLevel = 54;
	if(mCbParam.RfgLevel%6 != 0)mCbParam.RfgLevel = 0;

	if(mCbParam.TxPower > POWER_HIGH)mCbParam.TxPower = POWER_LOW;
	if((mSqParam.IsAsq != 0) &&(mSqParam.IsAsq != 0x20))mSqParam.IsAsq = 0;
	if(mSqParam.AsqLevel > 5) mSqParam.AsqLevel = 1;
	if(mSqParam.SqLevel > 28) mSqParam.SqLevel = 10;
	if(mSqParam.IsAsq) mCbParam.Sq = mSqParam.AsqLevel | 0x20;
	else mCbParam.Sq = mSqParam.SqLevel;
	
	
	mCbParam.UartTxBuf[0] = 0xFE;
	mCbParam.FreqCal = 110;
	for(i=0;i<28;i++)
	{
		if(mSqParam.SqOpenSet[i]>20||mSqParam.SqOpenSet[i]<-20)
		{
			mSqParam.SqOpenSet[i]=0;
		}	
		if(mSqParam.SqCloseSet[i]>20||mSqParam.SqCloseSet[i]<-20)
		{
			mSqParam.SqCloseSet[i]=0;
		}		
	
	}
	for(i=0;i<5;i++)
	{
		if(mSqParam.AsqOpenSet[i]>20||mSqParam.AsqOpenSet[i]<-20)
		{
			mSqParam.AsqOpenSet[i]=0;
		}	
		if(mSqParam.AsqCloseSet[i]>20||mSqParam.AsqCloseSet[i]<-20)
		{
			mSqParam.AsqCloseSet[i]=0;
		}	

	}
	
}


/*-------------------------------------------------------------------------
*函数：setDefaultParam  默认设置
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void setDefaultParam(void)
{
 u8 i;
	//mCbParam.FreqCal = loadData(EEP_FREQ_CAL);

	mCbParam.Country = COUNTRY_EU;
	mCbParam.Band = 0;
	mCbParam.Channel = 9;
	mCbParam.Modu = FM;
	mCbParam.RfgLevel = 0;
	mCbParam.TxPower = POWER_LOW;
	mCbParam.VolLevel = 5;
	channel.RX_Freq=200.015;
	mSqParam.IsAsq = 0x00;
	mSqParam.AsqLevel = 3;
	mSqParam.SqLevel = 10;
	mCbParam.Sq = (mSqParam.AsqLevel | mSqParam.IsAsq);	
	mDtmfRecive.dtmfCode=10;

	
for(i=0;i<28;i++)
{
	mSqParam.SqOpenSet[i]=0;
	mSqParam.SqCloseSet[i]=0;
}
	
	mSqParam.AsqOpenSet[0]=0;
	mSqParam.AsqOpenSet[1]=0;
	mSqParam.AsqOpenSet[2]=0;
	mSqParam.AsqOpenSet[3]=0;
	mSqParam.AsqOpenSet[4]=0;
	mSqParam.AsqCloseSet[0]=0;
	mSqParam.AsqCloseSet[1]=0;
	mSqParam.AsqCloseSet[2]=0;
	mSqParam.AsqCloseSet[3]=0;
	mSqParam.AsqCloseSet[4]=0;
	

}


/*-------------------------------------------------------------------------
*函数：loadAllParam  加载功能信息
*参数：无   
*返回值：无
*-------------------------------------------------------------------------*/
void loadAllParam(void)
{
	
	u8 i;
	u32 fre=0;
	u8 ddt=loadData(EEP_BASE);
	if(loadData(EEP_BASE) != 0xa5)
	{
		setDefaultParam();
		saveAllParam();
		saveSQSet();
	}
	else
	{
		
		mCbParam.FreqCal = loadData(EEP_FREQ_CAL);
		
		mCbParam.Band = loadData(EEP_BAND);
		mCbParam.Channel = loadData(EEP_CHANNEL);
		mCbParam.Modu = loadData(EEP_MODU);
		mCbParam.RfgLevel = loadData(EEP_RFG);
		mCbParam.TxPower = loadData(EEP_POWER);
		mCbParam.VolLevel = loadData(EEP_VOL);
		mCbParam.Country=loadData(EEP_COUNTRY);
		
		mSqParam.IsAsq = loadData(EEP_IS_ASQ);
		mSqParam.AsqLevel = loadData(EEP_ASQ_LEVEL);
		mSqParam.SqLevel = loadData(EEP_SQ_LEVEL);
		if(mSqParam.IsAsq == 1)mCbParam.Sq = (mSqParam.AsqLevel | mSqParam.IsAsq);
		else mCbParam.Sq = mSqParam.SqLevel;
		mDtmfRecive.dtmfCode=loadData(EEP_DTMF);
		

	
		
		fre=(((u32)loadData(EEP_FRE))<<24)|(((u32)loadData(EEP_FRE+1))<<16)|(((u32)loadData(EEP_FRE+2))<<8)|((u32)loadData(EEP_FRE+3));
		channel.RX_Freq=((float)fre/1000);
		
		for(i=0;i<28;i++)
		{
			mSqParam.SqOpenSet[i]=loadData(EEP_SQ_OPEN_SET+i);
			mSqParam.SqCloseSet[i]=loadData(EEP_SQ_CLOSE_SET+i);
		}
		
		mSqParam.AsqOpenSet[0]=loadData(EEP_ASQ_OPEN_SET);
		mSqParam.AsqOpenSet[1]=loadData(EEP_ASQ_OPEN_SET+1);
		mSqParam.AsqOpenSet[2]=loadData(EEP_ASQ_OPEN_SET+2);
		mSqParam.AsqOpenSet[3]=loadData(EEP_ASQ_OPEN_SET+3);
		mSqParam.AsqOpenSet[4]=loadData(EEP_ASQ_OPEN_SET+4);
		mSqParam.AsqOpenSet[5]=loadData(EEP_ASQ_OPEN_SET+5);
		
		mSqParam.AsqCloseSet[0]=loadData(EEP_ASQ_CLOSE_SET);
		mSqParam.AsqCloseSet[1]=loadData(EEP_ASQ_CLOSE_SET+1);
		mSqParam.AsqCloseSet[2]=loadData(EEP_ASQ_CLOSE_SET+2);
		mSqParam.AsqCloseSet[3]=loadData(EEP_ASQ_CLOSE_SET+3);
		mSqParam.AsqCloseSet[4]=loadData(EEP_ASQ_CLOSE_SET+4);
		mSqParam.AsqCloseSet[5]=loadData(EEP_ASQ_CLOSE_SET+5);

	}
	checkAllParam();

}

void Power_On_Rx()
{							
					
			Set_XN31202(0x02c4,14);
			Set_XN31202(0x3000 + (EXTERNAL_CRYSTAL/mReferenceFreq/2),14);
			mFlag.Mute=0;
			calculateFreq();
			if(mCbParam.Country==COUNTRY_UK) 
			{
				setFrqCal(mCbParam.FreqCal+170);		
			}
			else
			{
				setFrqCal(mCbParam.FreqCal);		
			}
			setSQ();
			//setVol();
			setPower();
			setModulation();
	    Set_Mute();
			//setEmission(0);
			saveAllParam();
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
	 Power_On_Rx();
}
