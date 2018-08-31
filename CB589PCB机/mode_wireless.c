#include "system.h"
#include "mode_wireless.h"
#include "KB4815.h"
#include "vco.h"
#include "event.h"

#define SNR_RSSI_CHECK_TIME 5

//extern xdata uint cSqSum;

extern code uint asq_open_table[];
extern code uint asq_close_table[];
extern code uint sq_open_table[];
extern code uint sq_close_table[];

/*-------------------------------------------------------------------------
*函数：initSysModeWireless  无线模式
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void initSysModeWireless(void)
{
	initBK4815();
  mFlag.SpkOpen4815 = 0;
	mFlag.SqOpen = 0;
	mSq.open = DB118;
	mSq.close = DB120;
}
u16 Get4815Rssi()
{
	u8 i;
	u16 snr,rssi;
	u16 snr_rssi;

	snr = 0;
	rssi = 0;
	
	for(i=0; i<SNR_RSSI_CHECK_TIME; i++)
	{
		snr_rssi = BK4815RssiAndSnr();
		snr += (snr_rssi >> 8);	
		rssi += snr_rssi & 0x00ff;																																																																			
		delayus(10);
	}
	snr /= SNR_RSSI_CHECK_TIME;
	   //读5次  取平均值
	rssi /= SNR_RSSI_CHECK_TIME;     
  return rssi;
}
/*-------------------------------------------------------------------------
*函数：wirelessCheckRec  接收
*参数：freq    频率  
*返回值：无
*-------------------------------------------------------------------------*/
void wirelessCheckRec(void)
{
	u16 rssi;
	u16 count=0;
	rssi=Get4815Rssi();
	while(rssi == 0 && count < 10) 
	{
		delayms(10);
		rssi=Get4815Rssi();
		count++;		
	}
	if(count >= 10 )
	{
		initBK4815();
		return;
	}
	if(mFlag.SpkOpen4815 == 0)
	{		
			if(rssi >= mSq.open )
			{
				
				if(mRecive.Sussece==1)
				{
					mFlag.SpkOpen4815 = 1;
					BK4815RxAudio();
					setEmission(1);
				}
								
			}
	}
	else 
	{
		if(rssi <= mSq.close || (mRecive.Errer	== 1))
		{
			mRecive.Errer = 0;
				mFlag.SpkOpen4815 = 0;
				EnterBK4815RX();
				setEmission(0);				
		}
	}
}

void checkCBRadioRec(void)
{
		uchar sq_db_count;
	char val_db;
	uint temp_asq,temp_rssi,temp_agca;
	u16 asq_close_db;
	temp_asq = 0;
	temp_rssi = 0;
	temp_agca = 0;

	temp_asq += readVoltage(ADC_ASQ);
	temp_rssi += readVoltage(ADC_RSSI);
	temp_agca += readVoltage(ADC_AGCA);
	mParameter.mAsqVoltage = temp_asq;
	mParameter.mRssiVoltage = temp_rssi;
	mParameter.mAgcaVoltage = temp_agca;	
	if(mParameter.mAgcaVoltage<0x0190)
    {
		mParameter.cSqSum= mParameter.mRssiVoltage + (0x0200 - mParameter.mAgcaVoltage);
	}
	else
	{
		mParameter.cSqSum= mParameter.mRssiVoltage ;
	}
    
	if((mCbParam.Sq&0x1f) == 0)
	{
		if(mFlag.Mute == 0) Cls_Mute();
		if(mFlag.SqOpen == 0)
		{		
			StartBK4815TX();
			delayms(50);
		}
		return;
	}
	
    if(mSqParam.IsAsq)	//判ASQ
	{
		if(mFlag.SqOpen == 0)
		{
			switch(mParameter.mOpenSqDbLevel)
		 {
			 case 1:sq_db_count=6;				 break;
			 case 2:sq_db_count=8;				 break;
			 case 3:sq_db_count=10;				 break;
			 case 4:sq_db_count=12;				 break;
			 case 5:sq_db_count=14;				 break;			
			 default:
				 break;
		 }
		 val_db=sq_db_count+mSqParam.AsqOpenSet[mParameter.mOpenSqDbLevel-1];
		if(val_db<0)val_db=0;
		else if(val_db>17) val_db=17;		
			//SET_SPK_MUTE; //关闭喇叭
			if(mParameter.mAsqVoltage <= asq_open_table[val_db])	//因为db是负值，所以相反
			{				
				delayms(50);
				checkRssi();
				if(mParameter.mAsqVoltage <= asq_open_table[val_db])	//因为db是负值，所以相反
				{	
					if((mFlag.Mute == 0)&&(mCbParam.VolLevel != 0)) Cls_Mute();	//打开喇叭
					else mFlag.SqOpenButMute = 1;				
					StartBK4815TX();
					mParameter.mLastOpenSqDbLevel = mParameter.mOpenSqDbLevel;
				}
			}
		}
		else 
		{
				switch(mParameter.mOpenSqDbLevel)
			 {
				 case 1:sq_db_count=3;				 break;
				 case 2:sq_db_count=5;				 break;
				 case 3:sq_db_count=7;				 break;
				 case 4:sq_db_count=9;				 break;
				 case 5:sq_db_count=11;				 break;				
				 default:
					 break;
			 }
			 val_db=sq_db_count+mSqParam.AsqCloseSet[mParameter.mOpenSqDbLevel-1];
			if(val_db<0)val_db=0;
			
				if(mParameter.mAsqVoltage >= asq_close_table[val_db])
				{
					delayms(50);
					checkRssi();
					if(mParameter.mAsqVoltage >= asq_close_table[val_db])
					{
						closeSq();
						EnterBK4815RX();
					}
				}
				else if(mFlag.Mute||(mCbParam.VolLevel == 0))mFlag.SqOpenButMute = 1;
				else
				{				
					if((mFlag.Mute == 0)&&(mCbParam.VolLevel != 0)) Cls_Mute();	//打开喇叭				
				}
		}
	}
	else
	{
		switch(mParameter.mOpenSqDbLevel)
		{
			case 1:sq_db_count=3;  break;
			case 2:sq_db_count=5;  break;
			case 3:sq_db_count=7;  break;	
			case 4:sq_db_count=9;  break;		
			case 5:sq_db_count=11;  break;				
			case 6:sq_db_count=13;  break;				
			case 7:sq_db_count=15;  break;				
			case 8:sq_db_count=17;  break;			
			case 9:sq_db_count=19;  break;				
			case 10:sq_db_count=21;  break;			
			case 11:sq_db_count=23;  break;				
			case 12:sq_db_count=25;  break;			
			case 13:sq_db_count=27;  break;			
			case 14:sq_db_count=29;  break;		
			case 15:sq_db_count=31;  break;		
			case 16:sq_db_count=33;  break;			
			case 17:sq_db_count=35;  break;			
			case 18:sq_db_count=37;  break;			
			case 19:sq_db_count=39;  break;			
			case 20:sq_db_count=41;  break;			
			case 21:sq_db_count=43;  break;			
			case 22:sq_db_count=45;  break;			
			case 23:sq_db_count=47;  break;				
			case 24:sq_db_count=51;  break;			
			case 25:sq_db_count=61;  break;			
			case 26:sq_db_count=71;  break;		
			case 27:sq_db_count=77;  break;			
			case 28:sq_db_count=82;  break;				
			default:
				break;
		}
		val_db=sq_db_count+mSqParam.SqOpenSet[mParameter.mOpenSqDbLevel-1];
		if(val_db<0)val_db=0;
		else if(val_db>84) val_db=84;		
		
		if(mFlag.SqOpen == 0)
		{
			//SET_SPK_MUTE; //关闭喇叭
			if(mParameter.cSqSum >= sq_open_table[val_db])	//因为db是负值，所以相反
			{			
				delayms(50);
					checkRssi();
				
				if(mParameter.cSqSum >= sq_open_table[val_db])	//因为db是负值，所以相反
				{		
					StartBK4815TX();
					mParameter.mLastOpenSqDbLevel = mParameter.mOpenSqDbLevel;
					if(mFlag.SqOpen == 0)
					{
						if((mFlag.Mute == 0)&&(mCbParam.VolLevel != 0)) Cls_Mute();	//打开喇叭
					else mFlag.SqOpenButMute = 1;
					}
				}
			}					
		}
		else
		{
			switch(mParameter.mOpenSqDbLevel)
		{
			case 1:sq_db_count=3;  break;
			case 2:sq_db_count=5;  break;
			case 3:sq_db_count=7;  break;	
			case 4:sq_db_count=8;  break;		
			case 5:sq_db_count=11;  break;				
			case 6:sq_db_count=13;  break;				
			case 7:sq_db_count=15;  break;				
			case 8:sq_db_count=17;  break;			
			case 9:sq_db_count=19;  break;				
			case 10:sq_db_count=21;  break;			
			case 11:sq_db_count=23;  break;				
			case 12:sq_db_count=24;  break;			
			case 13:sq_db_count=27;  break;			
			case 14:sq_db_count=28;  break;		
			case 15:sq_db_count=31;  break;		
			case 16:sq_db_count=33;  break;			
			case 17:sq_db_count=35;  break;			
			case 18:sq_db_count=36;  break;			
			case 19:sq_db_count=39;  break;			
			case 20:sq_db_count=41;  break;			
			case 21:sq_db_count=44;  break;			
			case 22:sq_db_count=45;  break;			
			case 23:sq_db_count=47;  break;				
			case 24:sq_db_count=51;  break;			
			case 25:sq_db_count=61;  break;			
			case 26:sq_db_count=71;  break;		
			case 27:sq_db_count=77;  break;			
			case 28:sq_db_count=82;  break;				
			default:
				break;
		}
		val_db=sq_db_count+mSqParam.SqCloseSet[mParameter.mOpenSqDbLevel-1];
		if(val_db<0)val_db=0;
		else if(val_db>87) val_db=87;	 

				asq_close_db=sq_close_table[val_db];				
				if(mParameter.cSqSum <= asq_close_db)
				{
					delayms(50);
					checkRssi();
					if(mParameter.cSqSum <= asq_close_db)
					{
						closeSq();
						EnterBK4815RX();
					}					
				}
				else if(mFlag.Mute||(mCbParam.VolLevel == 0))mFlag.SqOpenButMute = 1;		
				else
				{				
					if((mFlag.Mute == 0)&&(mCbParam.VolLevel != 0)) Cls_Mute();	//打开喇叭				
				}
		}		
	}
	
//	wirelessEventPTT();
}
/*-------------------------------------------------------------------------
*函数：sysModeWireless  无线模式
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void sysModeWireless(void)
{
	mFlag.SpkOpen4815=0;
	delayms(200);
	while((HM_DET==1)&&(POWER_ON==0))
	{			

		if(mParameter.isCheckRssi==1)
		{		
			if(mFlag.SqOpen == 0)
			{
				wirelessCheckRec();	
			}
			mParameter.isCheckRssi=0;
			if(mFlag.SpkOpen4815 == 0)
			{
				checkCBRadioRec();
			}
		}
		if(mRecive.RecvStatus==MRECIVE_BK4815_INTERUPT)
		{
			bk4815Initerrupt();
			mRecive.RecvStatus=MRECIVE_NONE;
		}
	}
  delayms(150);
	BK4815Sleep();
}

