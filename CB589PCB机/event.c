#include "system.h"
#include "XN31202.h"
#include "mode_wireless.h"
#include "event.h"
#include "vco.h"
#include "uart.h"
#include "memory.h"
#include "m62429.h"
#include "KB4815.h"

extern Channel channel;
u8 mOldRssi = 255;
xdata uint cSqSum;
u8 isautoRGF=0;
u8 autoRFG=0;
u8 isSendRSSI=0;
u8 sengRssiCount=0;
u8 is4815Sleep=0;
code uint asq_open_table[] = 
{ 
  ASQ_OPEN_LEVEL_127DB,	ASQ_OPEN_LEVEL_126DB,	ASQ_OPEN_LEVEL_125DB,	ASQ_OPEN_LEVEL_124DB,	ASQ_OPEN_LEVEL_123DB,	ASQ_OPEN_LEVEL_122DB,  //                 3
	ASQ_OPEN_LEVEL_121DB,	ASQ_OPEN_LEVEL_120DB,	ASQ_OPEN_LEVEL_119DB,	ASQ_OPEN_LEVEL_118DB,	ASQ_OPEN_LEVEL_117DB,  
	ASQ_OPEN_LEVEL_116DB,	ASQ_OPEN_LEVEL_115DB,	ASQ_OPEN_LEVEL_114DB,	ASQ_OPEN_LEVEL_113DB,	ASQ_OPEN_LEVEL_112DB,  
	ASQ_OPEN_LEVEL_111DB,	ASQ_OPEN_LEVEL_110DB,	ASQ_OPEN_LEVEL_109DB                                       
};
code uint asq_close_table[] = 
{ 
  ASQ_CLOSE_LEVEL_127DB,	ASQ_CLOSE_LEVEL_126DB,	ASQ_CLOSE_LEVEL_125DB,	ASQ_CLOSE_LEVEL_124DB,	ASQ_CLOSE_LEVEL_123DB,	ASQ_CLOSE_LEVEL_122DB,  //                 3
	ASQ_CLOSE_LEVEL_121DB,	ASQ_CLOSE_LEVEL_120DB,	ASQ_CLOSE_LEVEL_119DB,	ASQ_CLOSE_LEVEL_118DB,	ASQ_CLOSE_LEVEL_117DB,  
	ASQ_CLOSE_LEVEL_116DB,	ASQ_CLOSE_LEVEL_115DB,	ASQ_CLOSE_LEVEL_114DB,	ASQ_CLOSE_LEVEL_113DB,	ASQ_CLOSE_LEVEL_112DB,  
	ASQ_CLOSE_LEVEL_111DB,	ASQ_CLOSE_LEVEL_110DB,	ASQ_CLOSE_LEVEL_109DB                                       
};
/*
code uint asq_table[] = {	ASQ_LEVEL_121DB,	ASQ_LEVEL_119DB,	ASQ_LEVEL_117DB,	ASQ_LEVEL_115DB,	ASQ_LEVEL_113DB};
*/

code uint sq_open_table[] = 
{
	SQ_OPEN_124DB,	SQ_OPEN_123DB,	SQ_OPEN_122DB,	SQ_OPEN_121DB,	SQ_OPEN_120DB,	SQ_OPEN_119DB,	SQ_OPEN_118DB,	SQ_OPEN_117DB,	SQ_OPEN_116DB,	SQ_OPEN_115DB,     //3 5 7 9
	SQ_OPEN_114DB,	SQ_OPEN_113DB,	SQ_OPEN_112DB,	SQ_OPEN_111DB,	SQ_OPEN_110DB,	SQ_OPEN_109DB,	SQ_OPEN_108DB,	SQ_OPEN_107DB,	SQ_OPEN_106DB,	SQ_OPEN_105DB,     //11 13 15 17 19
	SQ_OPEN_104DB,	SQ_OPEN_103DB,	SQ_OPEN_102DB,	SQ_OPEN_101DB,	SQ_OPEN_100DB,	SQ_OPEN_99DB,	SQ_OPEN_98DB,	SQ_OPEN_97DB,	SQ_OPEN_96DB,	SQ_OPEN_95DB,      //21 23 25 27 29
	SQ_OPEN_94DB,	SQ_OPEN_93DB,	SQ_OPEN_92DB,	SQ_OPEN_91DB,	SQ_OPEN_90DB,	SQ_OPEN_89DB,	SQ_OPEN_88DB,	SQ_OPEN_87DB,	SQ_OPEN_86DB,	SQ_OPEN_85DB,      //31 33 35 37 39
	SQ_OPEN_84DB,	SQ_OPEN_83DB,	SQ_OPEN_82DB,	SQ_OPEN_81DB,	SQ_OPEN_80DB,	SQ_OPEN_79DB,	SQ_OPEN_78DB,	SQ_OPEN_77DB,	SQ_OPEN_76DB,	SQ_OPEN_75DB,      //41 43 45 47 
	SQ_OPEN_74DB,	SQ_OPEN_73DB,	SQ_OPEN_72DB,	SQ_OPEN_71DB,	SQ_OPEN_70DB,	SQ_OPEN_69DB,	SQ_OPEN_68DB,	SQ_OPEN_67DB,	SQ_OPEN_66DB,	SQ_OPEN_65DB,      //51
	SQ_OPEN_64DB,	SQ_OPEN_63DB,	SQ_OPEN_62DB,	SQ_OPEN_61DB,	SQ_OPEN_60DB,	SQ_OPEN_59DB,	SQ_OPEN_58DB,	SQ_OPEN_57DB,	SQ_OPEN_56DB,	SQ_OPEN_55DB,      //61
	SQ_OPEN_54DB,	SQ_OPEN_53DB,	SQ_OPEN_52DB,	SQ_OPEN_51DB,	SQ_OPEN_50DB,	SQ_OPEN_49DB,	SQ_OPEN_48DB,	SQ_OPEN_47DB,	SQ_OPEN_46DB,	SQ_OPEN_45DB,      //71 77
	SQ_OPEN_44DB,	SQ_OPEN_43DB,	SQ_OPEN_42DB,	SQ_OPEN_41DB,	SQ_OPEN_40DB                                                         //82
};

code uint sq_close_table[] = 
{
	SQ_CLOSE_127DB, SQ_CLOSE_126DB, SQ_CLOSE_125DB, SQ_CLOSE_124DB,	SQ_CLOSE_123DB,	SQ_CLOSE_122DB,	SQ_CLOSE_121DB,	SQ_CLOSE_120DB,	SQ_CLOSE_119DB,	SQ_CLOSE_118DB,	SQ_CLOSE_117DB,	SQ_CLOSE_116DB,	SQ_CLOSE_115DB,
	SQ_CLOSE_114DB,	SQ_CLOSE_113DB,	SQ_CLOSE_112DB,	SQ_CLOSE_111DB,	SQ_CLOSE_110DB,	SQ_CLOSE_109DB,	SQ_CLOSE_108DB,	SQ_CLOSE_107DB,	SQ_CLOSE_106DB,	SQ_CLOSE_105DB,
	SQ_CLOSE_104DB,	SQ_CLOSE_103DB,	SQ_CLOSE_102DB,	SQ_CLOSE_101DB,	SQ_CLOSE_100DB,	SQ_CLOSE_99DB,	SQ_CLOSE_98DB,	SQ_CLOSE_97DB,	SQ_CLOSE_96DB,	SQ_CLOSE_95DB,
	SQ_CLOSE_94DB,	SQ_CLOSE_93DB,	SQ_CLOSE_92DB,	SQ_CLOSE_91DB,	SQ_CLOSE_90DB,	SQ_CLOSE_89DB,	SQ_CLOSE_88DB,	SQ_CLOSE_87DB,	SQ_CLOSE_86DB,	SQ_CLOSE_85DB,
	SQ_CLOSE_84DB,	SQ_CLOSE_83DB,	SQ_CLOSE_82DB,	SQ_CLOSE_81DB,	SQ_CLOSE_80DB,	SQ_CLOSE_79DB,	SQ_CLOSE_78DB,	SQ_CLOSE_77DB,	SQ_CLOSE_76DB,	SQ_CLOSE_75DB,
	SQ_CLOSE_74DB,	SQ_CLOSE_73DB,	SQ_CLOSE_72DB,	SQ_CLOSE_71DB,	SQ_CLOSE_70DB,	SQ_CLOSE_69DB,	SQ_CLOSE_68DB,	SQ_CLOSE_67DB,	SQ_CLOSE_66DB,	SQ_CLOSE_65DB,
	SQ_CLOSE_64DB,	SQ_CLOSE_63DB,	SQ_CLOSE_62DB,	SQ_CLOSE_61DB,	SQ_CLOSE_60DB,	SQ_CLOSE_59DB,	SQ_CLOSE_58DB,	SQ_CLOSE_57DB,	SQ_CLOSE_56DB,	SQ_CLOSE_55DB,
	SQ_CLOSE_54DB,	SQ_CLOSE_53DB,	SQ_CLOSE_52DB,	SQ_CLOSE_51DB,	SQ_CLOSE_50DB,	SQ_CLOSE_49DB,	SQ_CLOSE_48DB,	SQ_CLOSE_47DB,	SQ_CLOSE_46DB,	SQ_CLOSE_45DB,
	SQ_CLOSE_44DB,	SQ_CLOSE_43DB,	SQ_CLOSE_42DB,	SQ_CLOSE_41DB,	SQ_CLOSE_40DB
};


/*

code uint sq_close_table[] = 
{
	SQ_124DB, SQ_122DB,	SQ_120DB,	SQ_119DB,	SQ_116DB,	SQ_114DB,	SQ_112DB,	SQ_110DB,	SQ_108DB,	SQ_106DB,	
	SQ_104DB,	SQ_103DB,	SQ_100DB,	SQ_99DB,	SQ_96DB,	SQ_94DB,	SQ_92DB,	SQ_91DB,	SQ_88DB,	SQ_86DB,	
	SQ_83DB,	SQ_82DB,	SQ_80DB,	SQ_76DB,	SQ_66DB,	SQ_56DB,	SQ_50DB,	SQ_45DB
};
*/
/*-------------------------------------------------------------------------
*函数：irq_timer0  100ms定时中断
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void irq_timer0(void) interrupt 1
{  
	
	
	
		TR0 = 0;
		TH0 = (65535 - 1000) / 256;	
		TL0 = (65535 - 1000) % 256;	//100ms		
  	TR0 = 1; 
	sengRssiCount++;
	if(sengRssiCount>2)
	{	  
		if(POWER_ON == 1&&(HM_DET==0))	
		{		

		    isSendRSSI=1;
		}
		sengRssiCount=0;
  }

}
/*-------------------------------------------------------------------------
*函数：setFrqCal  设置frq的占空比
*参数：dat    
*返回值：无
*-------------------------------------------------------------------------*/
void setFrqCal(uchar dat)
{
	PWM_FRQ_CLOSE;
	xPWM1D = dat;
	PWM_FRQ_OPEN;
}


/*-------------------------------------------------------------------------
*函数：closeSq  关闭SQ
*参数：dat    
*返回值：无
*-------------------------------------------------------------------------*/
void closeSq(void)
{
	Set_Mute(); //关闭喇叭
	mFlag.SqOpen = 0;
	mFlag.SqOpenButMute = 0;
}
/*-------------------------------------------------------------------------
*函数：setModda  设置频偏
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void setModda(void)
{
	uchar modda;
	PWM_MODDA_CLOSE;
	if(mCurrentFreq < 26315000)
		modda=115;
	else if(mCurrentFreq <26765000) 
		modda=100;
	else if(mCurrentFreq<27205000)  
		modda=90;
	//else if(mCurrentFreq < 27415000) modda = 80;
	else if(mCurrentFreq < 27785000) 
		modda = 80;               //中间频率
	else if(mCurrentFreq < 28285000) 
		modda = 70;
	else if(mCurrentFreq < 28725500) 
		modda = 50;
	else if(mCurrentFreq < 29605000)
		modda = 144;
	else if(mCurrentFreq < 31000000) 
		modda = 148;
	if(modda != 0)
	{
		MODDA = modda;
		PWM_MODDA_OPEN;
	}  
}

/*-------------------------------------------------------------------------
*函数：setRfg  设置Rfg
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void setRfg(uchar RfgLevel)
{
	uchar rfg;
	 
	PWM_RFG_CLOSE;;
	
	
		if(RfgLevel == 0) rfg = 0;
		else if(RfgLevel == 1) rfg = 116;
		else if(RfgLevel == 2) rfg = 121;
		else if(RfgLevel == 3) rfg = 128;
		else if(RfgLevel == 4) rfg = 135;
		else if(RfgLevel == 5) rfg = 142;
		else if(RfgLevel == 6) rfg = 149;
		else if(RfgLevel == 7) rfg = 158;
		else if(RfgLevel == 8) rfg = 180;
		else if(RfgLevel == 9) rfg = 200;
	
		if(rfg != 0)
		{
			RFG = rfg;
			PWM_RFG_OPEN;
		}	
	
}
/*-------------------------------------------------------------------------
*函数：setModulation  设置调频
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void setModulation(void)
{
	if(mCbParam.Modu) SET_AM_FM;
	else CLS_AM_FM;
}
/*-------------------------------------------------------------------------
*函数：setPower  设置功率
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void setPower(void)
{
	if(mCbParam.TxPower == POWER_LOW)
	{
		PL = POWER_LOW;
		PL_1W = 0;
	}
	else if(mCbParam.TxPower == POWER_HIGH)
	{
		PL = 0;
		PL_1W = 0;
	}
	else
	{
		PL = 0;
		PL_1W = 1;
	}
}
/*-------------------------------------------------------------------------
*函数：setSQ  设置SQ等级
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void setSQ(void)  //SQ 28级
{
	//closeSq();
	mOpenSqDbLevel = mCbParam.Sq & 0x1f;
	
	if(mCbParam.Sq > 0x1f)	//Asq标志位在mCbParam.Sq的0x20位置
	{
		mSqParam.IsAsq = 1;
	}
	else
	{
		mSqParam.IsAsq = 0;
	}
}
void setSQSet()
{
	
}

/*-------------------------------------------------------------------------
*函数：setVol  设置音量
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void setVol(void)
{
	if(mCbParam.VolLevel == 0)
	{
		Set_Mute();
	}
	else 
	{
		setVolumeLevel(mCbParam.VolLevel );
		if(mFlag.SqOpen&&(mFlag.Mute == 0))Cls_Mute();
	}
}
void checkRssi(void)
{
	u8 i,j;
	uint temp_asq,temp_rssi,temp_agca,temp;
	u16 buf_asq[10],buf_rssi[10],buf_agca[10];
	temp_asq = 0;
	temp_rssi = 0;
	temp_agca = 0;
	for(i=0;i<10;i++)
	{
		buf_asq[i]= readVoltage(ADC_ASQ);
		buf_rssi[i]= readVoltage(ADC_RSSI);
		buf_agca[i]= readVoltage(ADC_AGCA);	
  }
  for(i=0;i<10;i++)   
  {
    for(j=0;j<10-i;j++)
    {
      if(buf_asq[j]>buf_asq[j+1])
      { 
        temp = buf_asq[j];
        buf_asq[j] = buf_asq[j+1];
        buf_asq[j+1] = temp;                 
      }
			if(buf_rssi[j]>buf_rssi[j+1])
      { 
        temp = buf_rssi[j];
        buf_rssi[j] = buf_rssi[j+1];
        buf_rssi[j+1] = temp;                 
      }
			if(buf_agca[j]>buf_agca[j+1])
      { 
        temp = buf_agca[j];
        buf_agca[j] = buf_agca[j+1];
        buf_agca[j+1] = temp;                 
      }
    }
  }
	for(i=4;i<6;i++)	
	{
		temp_asq+=buf_asq[i];
		temp_rssi+=buf_rssi[i];
		temp_agca+=buf_agca[i];
	}	
	mAsqVoltage = temp_asq/2;
	mRssiVoltage = temp_rssi/2;
	mAgcaVoltage = temp_agca/2;

	if(mAgcaVoltage<0x009f)
	{
		isautoRGF=1;
		if((autoRFG+mCbParam.RfgLevel)<9)
		{
			autoRFG+=1;
			setRfg(autoRFG+mCbParam.RfgLevel);
		}
		
	}
	else if(mAgcaVoltage>0x00c2)
	{
		if(autoRFG>0)
		{
			autoRFG-=1;
			setRfg(autoRFG+mCbParam.RfgLevel);
		}
		else isautoRGF=0;
		
	}
		
	if(mAgcaVoltage<0x0190)
  {
		cSqSum= mRssiVoltage + (0x0200 - mAgcaVoltage);
	}
	else
	{
		cSqSum= mRssiVoltage ;
	}
	mRssi = 0;
//	if(cSqSum >= sq_table[28])		mRssi = 5;
//	else if(cSqSum >= sq_table[17])	mRssi = 4;
//	else if(cSqSum >= sq_table[10]) mRssi = 3;
//	else if(cSqSum >= sq_table[6]) 	mRssi = 2;
//	else if(cSqSum >= sq_table[1]) 	mRssi = 1;	
	
		if(cSqSum >= sq_open_table[82+mSqParam.SqOpenSet[27]])		mRssi = 28;
		else if(cSqSum >= sq_open_table[77+mSqParam.SqOpenSet[26]])	mRssi =27;
		else if(cSqSum >= sq_open_table[71+mSqParam.SqOpenSet[25]])	mRssi =26;
		else if(cSqSum >= sq_open_table[61+mSqParam.SqOpenSet[24]])	mRssi =25;
		else if(cSqSum >= sq_open_table[51+mSqParam.SqOpenSet[23]])	mRssi =24;
		else if(cSqSum >= sq_open_table[47+mSqParam.SqOpenSet[22]])	mRssi =23;
		else if(cSqSum >= sq_open_table[45+mSqParam.SqOpenSet[21]])	mRssi =22;
		else if(cSqSum >= sq_open_table[43+mSqParam.SqOpenSet[20]])	mRssi =21;
		else if(cSqSum >= sq_open_table[41+mSqParam.SqOpenSet[19]])	mRssi =20;
		else if(cSqSum >= sq_open_table[39+mSqParam.SqOpenSet[18]])	mRssi =19;
		else if(cSqSum >= sq_open_table[37+mSqParam.SqOpenSet[17]])	mRssi =18;
		else if(cSqSum >= sq_open_table[35+mSqParam.SqOpenSet[16]])	mRssi =17;
		else if(cSqSum >= sq_open_table[33+mSqParam.SqOpenSet[15]])	mRssi =16;
		else if(cSqSum >= sq_open_table[31+mSqParam.SqOpenSet[14]])	mRssi =15;
		else if(cSqSum >= sq_open_table[29+mSqParam.SqOpenSet[13]])	mRssi =14;
		else if(cSqSum >= sq_open_table[27+mSqParam.SqOpenSet[12]])	mRssi =13;
		else if(cSqSum >= sq_open_table[25+mSqParam.SqOpenSet[11]])	mRssi =12;
		else if(cSqSum >= sq_open_table[23+mSqParam.SqOpenSet[10]])	mRssi =11;
		else if(cSqSum >= sq_open_table[21+mSqParam.SqOpenSet[9]])	mRssi =10;
		else if(cSqSum >= sq_open_table[19+mSqParam.SqOpenSet[8]])	mRssi =9;
		else if(cSqSum >= sq_open_table[17+mSqParam.SqOpenSet[7]])	mRssi =8;
		else if(cSqSum >= sq_open_table[15+mSqParam.SqOpenSet[6]])	mRssi =7;
		else if(cSqSum >= sq_open_table[13+mSqParam.SqOpenSet[5]])	mRssi =6;
		else if(cSqSum >= sq_open_table[11+mSqParam.SqOpenSet[4]])	mRssi =5;
		else if(cSqSum >= sq_open_table[9+mSqParam.SqOpenSet[3]])	mRssi =4;
		else if(cSqSum >= sq_open_table[7+mSqParam.SqOpenSet[2]])	mRssi =3;
		else if(cSqSum >= sq_open_table[5+mSqParam.SqOpenSet[1]])	mRssi =2;
		else if(cSqSum >= sq_open_table[3+mSqParam.SqOpenSet[0]])	mRssi =1;
		else mRssi = 0;
}
void Set_Mute()
{
		
	SET_SPK_MUTE; 
	//setVolumeLevel(mCbParam.VolLevel );
}
void Cls_Mute()
{
	if(mFlag.SqOpen==1)
	{
		CLS_SPK_MUTE;	
	}		
}

void checkIsSqOpen(void)
{


	char val_db;
  u8 sq_db_count;

	if(mCbParam.Sq == 0)
	{
		mFlag.SqOpen = 1;
		//sendCommand(CMD_GET_RSSI);
		if((mFlag.Mute == 0)&&(mCbParam.VolLevel != 0)) Cls_Mute();
		return;
	}
	if(mSqParam.IsAsq)	//判ASQ
	{		
		if(mFlag.SqOpen == 0)
		{
			switch(mOpenSqDbLevel)
		 {
			 case 1:sq_db_count=6;				 break;
			 case 2:sq_db_count=8;				 break;
			 case 3:sq_db_count=10;				 break;
			 case 4:sq_db_count=12;				 break;
			 case 5:sq_db_count=14;				 break;			
			 default:
				 break;
		 }
		 val_db=sq_db_count+mSqParam.AsqOpenSet[mOpenSqDbLevel-1];
		if(val_db<0)val_db=0;
		else if(val_db>17) val_db=17;			 
			//SET_SPK_MUTE; //关闭喇叭
			if(mAsqVoltage <= asq_open_table[val_db])	//因为db是负值，所以相反
			{
				
				mFlag.SqOpen = 1;				
				//sendCommand(CMD_GET_RSSI);				
				setEmission(0);				//CLS_SPK_MUTE;	
			
				mLastOpenSqDbLevel = mOpenSqDbLevel;
				//delayms(200);
				if((mFlag.Mute == 0)&&(mCbParam.VolLevel != 0)) Cls_Mute();	//打开喇叭
				else mFlag.SqOpenButMute = 1;
			}
		}
		else 
		{		
			 switch(mOpenSqDbLevel)
			 {
				 case 1:sq_db_count=3;				 break;
				 case 2:sq_db_count=5;				 break;
				 case 3:sq_db_count=7;				 break;
				 case 4:sq_db_count=9;				 break;
				 case 5:sq_db_count=11;				 break;				
				 default:
					 break;
			 }
			 val_db=sq_db_count+mSqParam.AsqCloseSet[mOpenSqDbLevel-1];
			 if(val_db<0)val_db=0;
			
				if(mAsqVoltage >= asq_close_table[val_db])
				{
					closeSq();					
					sendCommand(CMD_GET_RSSI);
					setEmission(0);					
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
		switch(mOpenSqDbLevel)
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
		val_db=sq_db_count+mSqParam.SqOpenSet[mOpenSqDbLevel-1];
		if(val_db<0)val_db=0;
		else if(val_db>84) val_db=84;		
		
		if(mFlag.SqOpen == 0)
		{
			//SET_SPK_MUTE; //关闭喇叭
			if(cSqSum >= sq_open_table[val_db])	//因为db是负值，所以相反
			{			
				mFlag.SqOpen = 1; 
				
				//sendCommand(CMD_GET_RSSI);							
				setEmission(0);
				mLastOpenSqDbLevel = mOpenSqDbLevel;
				//CLS_SPK_MUTE;
				
				if((mFlag.Mute == 0)&&(mCbParam.VolLevel != 0)) Cls_Mute();	//打开喇叭
				else mFlag.SqOpenButMute = 1;
			}					
		}
		else
		{
			switch(mOpenSqDbLevel)
		{
			case 1:sq_db_count=4;  break;
			case 2:sq_db_count=6;  break;
			case 3:sq_db_count=8;  break;	
			case 4:sq_db_count=10;  break;		
			case 5:sq_db_count=12;  break;				
			case 6:sq_db_count=14;  break;				
			case 7:sq_db_count=16;  break;				
			case 8:sq_db_count=18;  break;			
			case 9:sq_db_count=20;  break;				
			case 10:sq_db_count=22;  break;			
			case 11:sq_db_count=24;  break;				
			case 12:sq_db_count=26;  break;			
			case 13:sq_db_count=28;  break;			
			case 14:sq_db_count=30;  break;		
			case 15:sq_db_count=32;  break;		
			case 16:sq_db_count=34;  break;			
			case 17:sq_db_count=36;  break;			
			case 18:sq_db_count=38;  break;			
			case 19:sq_db_count=40;  break;			
			case 20:sq_db_count=42;  break;			
			case 21:sq_db_count=44;  break;			
			case 22:sq_db_count=46;  break;			
			case 23:sq_db_count=48;  break;				
			case 24:sq_db_count=52;  break;			
			case 25:sq_db_count=62;  break;			
			case 26:sq_db_count=72;  break;		
			case 27:sq_db_count=78;  break;			
			case 28:sq_db_count=82;  break;				
			default:
				break;
		}
			
			val_db=sq_db_count+mSqParam.SqCloseSet[mOpenSqDbLevel-1];
		if(val_db<0)val_db=0;
		else if(val_db>84) val_db=84;	
				if(cSqSum <= sq_close_table[val_db])
				{	
						closeSq();
						sendCommand(CMD_GET_RSSI);					
						setEmission(0);
				}
				else if(mFlag.Mute||(mCbParam.VolLevel == 0))mFlag.SqOpenButMute = 1;
				else
				{				
					if((mFlag.Mute == 0)&&(mCbParam.VolLevel != 0)) Cls_Mute();	//打开喇叭				
				}
				/*
			}
			
			else
			{
				
				if(cSqSum > asq_table[mOpenSqDbLevel-1])
				{
					closeSq();
					setEmission(0);
				}
				else if(mFlag.Mute||(mCbParam.VolLevel == 0))mFlag.SqOpenButMute = 1;
			}
			*/
		}		
	}
}
/*-------------------------------------------------------------------------
*函数：checkSq  CB机接收
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void checkSq(void)
{
	checkRssi();
	checkIsSqOpen();

//	delayms(20);
}
/*-------------------------------------------------------------------------
*函数：start1Rx  CB机接收
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void start1Rx(void)
{
	CLS_TX_EN;
	SET_VCO_EN;
	SET_MIC_MUTE;
	SET_AT_MUTE;
	SET_FT_MUTE;
	//Set_Mute();
	mFlag.VcoIdle=0;
	setFreq(mXn31202Ch1_Rx);	
	setRfg(autoRFG+mCbParam.RfgLevel);
	while(PLL_LD == 0);
	//delayms(300);
	SET_RX_EN; 
}
/*-------------------------------------------------------------------------
*函数：start1Tx  CB机发射
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void start1Tx(void)
{
	
	//Set_Mute();
	CLS_VCO_EN;
	CLS_RX_EN;
	CLS_TX_EN;
	//mFlag.SqOpen = 0;
	setFreq(mXn31202Ch1_Tx);
	setModda();
	mFlag.VcoIdle=1;
	if(mCbParam.Modu == FM) 
	{
	 	CLS_FT_MUTE;
	 	SET_AT_MUTE;
	}
	else 
	{
		SET_FT_MUTE;
	 	CLS_AT_MUTE;
	}

	while(PLL_LD == 0);

	//delayms(300);
	SET_TX_EN;
	CLS_MIC_MUTE;
}
/*-------------------------------------------------------------------------
*函数：setEmission  设置发射接收
*参数：tx_rx       0：接收  1：发射    
*返回值：无
*-------------------------------------------------------------------------*/
void setEmission(uchar tx_rx)
{
	mFlag.VcoIdle=0;
	if(tx_rx)		start1Tx();
	else start1Rx();
	
}

void initHandler(void)
{
	//clearFlag();
	if(HM_DET==1)
	{
		//IE &= 0xef;	     //dis  ES0             关闭串口中断
		mFlag.SysMode = SYS_MODE_WIRELESS;
	}
	else 
	{
		//IE |= 0x10;	     //en  ES0              开启串口中断
		mFlag.SysMode = SYS_MODE_LINE;
	}
}


/*-------------------------------------------------------------------------
*函数：eventHandler  启动程序 
*参数：无    
*返回值：无
*-------------------------------------------------------------------------*/

void eventHandler(void)
{
	u8 event;
	u32 fre;
	if(mFlag.SysMode == SYS_MODE_WIRELESS)
	{
		is4815Sleep=0;
		calculateFreq();
	  setEmission(0);  
		saveAllParam();
		mReceivePackage.RecvCount=0;
		mFlag.SpkOpen4815=0;
		initSysModeWireless();
		sysModeWireless();
		mOldRssi = 0;
		
	}
	else
	{				
		if(is4815Sleep==0)
		{
			BK4815Sleep();
			is4815Sleep=1;
		}
		if(POWER_ON == 1)
		{
			event=mUartCmd;
			if(event !=0)
			{
				switch(event)
				{
					case CMD_TRANSMIT: 
						{
							calculateFreq();
							setEmission(1);        					  		 //发射
						}
						break;
					case CMD_RECEIVE: 
						{
							calculateFreq();
							setEmission(0);       								   //接收
							delayms(75);
						}
						break;
					case CMD_REQUEST_RSSI: sendCommand(CMD_GET_RSSI);  				 //信号强度
						break;
				
					case CMD_SQ_SET:
						if(mSqParam.IsAsq == 0)
						{
							if(mReceivePackage.RecvBuf[3]==0)
							{
								if((mReceivePackage.RecvBuf[4]&0x20)!=0)
								{
									mSqParam.SqOpenSet[mOpenSqDbLevel-1]=(mReceivePackage.RecvBuf[4]&~0x20);
									mSqParam.SqOpenSet[mOpenSqDbLevel-1]=-mSqParam.SqOpenSet[mOpenSqDbLevel-1];
								}
								else mSqParam.SqOpenSet[mOpenSqDbLevel-1]=mReceivePackage.RecvBuf[4];
							}
							else
							{
								if((mReceivePackage.RecvBuf[4]&0x20)!=0)
								{
									mSqParam.SqCloseSet[mOpenSqDbLevel-1]=(mReceivePackage.RecvBuf[4]&~0x20);
									mSqParam.SqCloseSet[mOpenSqDbLevel-1]=-mSqParam.SqCloseSet[mOpenSqDbLevel-1];
								}
								else mSqParam.SqCloseSet[mOpenSqDbLevel-1]=mReceivePackage.RecvBuf[4];
							
							}							
						}
						else
						{
							if(mReceivePackage.RecvBuf[3]==0)
							{
								if((mReceivePackage.RecvBuf[4]&0x20)!=0)
								{
									mSqParam.AsqOpenSet[mOpenSqDbLevel-1]=(mReceivePackage.RecvBuf[4]&~0x20);
									mSqParam.AsqOpenSet[mOpenSqDbLevel-1]=-mSqParam.AsqOpenSet[mOpenSqDbLevel-1];
								}
								else mSqParam.AsqOpenSet[mOpenSqDbLevel-1]=mReceivePackage.RecvBuf[4];
							}
							else
							{
								if((mReceivePackage.RecvBuf[4]&0x20)!=0)
								{
									mSqParam.AsqCloseSet[mOpenSqDbLevel-1]=(mReceivePackage.RecvBuf[4]&~0x20);
									mSqParam.AsqCloseSet[mOpenSqDbLevel-1]=-mSqParam.AsqCloseSet[mOpenSqDbLevel-1];
								}
								else mSqParam.AsqCloseSet[mOpenSqDbLevel-1]=mReceivePackage.RecvBuf[4];							
							}
						}
						saveSQSet();
						break;
					case CMD_SET_CHANNEL:
						mCbParam.Channel =mReceivePackage.RecvBuf[3] ;            //信道设置
					  mCbParam.Band=mReceivePackage.RecvBuf[4]; 
					  mCbParam.Modu=mReceivePackage.RecvBuf[5];
					  mCbParam.TxPower = mReceivePackage.RecvBuf[6];						
						setPower();
					  setModulation();
						calculateFreq();
						setEmission(0);					
						delayms(150);
					
						break;
					case CMD_SET_SQ_ASQ:                                        //SQ设置
						mCbParam.Sq=mReceivePackage.RecvBuf[3];
						setSQ();
						checkSq();
						break;
					case CMD_SET_RFG:																						//RFG设置
						mCbParam.RfgLevel=mReceivePackage.RecvBuf[3];
						setRfg(autoRFG+mCbParam.RfgLevel);
						break;
					case CMD_SET_VOL:																						//声音设置
						mCbParam.VolLevel=mReceivePackage.RecvBuf[3];
						setVol();
						break;
					case CMD_SET_TX_POWER:																			//发射功率设置
						mCbParam.TxPower = mReceivePackage.RecvBuf[3];
						setPower();
						break;
					case CMD_SET_MODULATION:																		//模式切换
						mCbParam.Modu = mReceivePackage.RecvBuf[3];
						mCbParam.TxPower = mReceivePackage.RecvBuf[4];
						setPower();
						setModulation();
						setEmission(0);
						break;
					case CMD_SET_BAND:                                          //频段
						mCbParam.Band= mReceivePackage.RecvBuf[3];
						calculateFreq();
						setEmission(0);
						break;
					case CMD_SET_DTMF:
						mDtmfRecive.dtmfCode= mReceivePackage.RecvBuf[3]<<4|mReceivePackage.RecvBuf[4];
						fre=(((u32)mReceivePackage.RecvBuf[5])<<28)|(((u32)mReceivePackage.RecvBuf[6])<<21)|(((u32)mReceivePackage.RecvBuf[7])<<14)|((u32)mReceivePackage.RecvBuf[8]<<7)|((u32)mReceivePackage.RecvBuf[9]);
						channel.RX_Freq=((float)fre/1000);	 	  
	          //if(channel.RX_Freq<200||channel.RX_Freq>400)channel.RX_Freq=300;
						break;
					case CMD_SET_ALL:			
						mCbParam.FreqCal = mReceivePackage.RecvBuf[3];
						mCbParam.Country = mReceivePackage.RecvBuf[4];
						mCbParam.Band = mReceivePackage.RecvBuf[5];
						mCbParam.Modu = mReceivePackage.RecvBuf[6];
						mCbParam.Channel = mReceivePackage.RecvBuf[7];
					
						mCbParam.TxPower = mReceivePackage.RecvBuf[8];
						mCbParam.Sq = mReceivePackage.RecvBuf[9];
						mCbParam.RfgLevel = mReceivePackage.RecvBuf[10];
						mCbParam.VolLevel = mReceivePackage.RecvBuf[11];				
						mDtmfRecive.dtmfCode= mReceivePackage.RecvBuf[12];
						mDtmfRecive.dtmfCode= mReceivePackage.RecvBuf[12]<<4|mReceivePackage.RecvBuf[13];
						fre=(((u32)mReceivePackage.RecvBuf[14])<<28)|(((u32)mReceivePackage.RecvBuf[15])<<21)|(((u32)mReceivePackage.RecvBuf[16])<<14)|((u32)mReceivePackage.RecvBuf[17]<<7)|((u32)mReceivePackage.RecvBuf[18]);
						channel.RX_Freq=((float)fre/1000);	
//						if(channel.RX_Freq<200||channel.RX_Freq>400)channel.RX_Freq=300;
						Set_XN31202(0x02c4,14);
						Set_XN31202(0x3000 + (EXTERNAL_CRYSTAL/mReferenceFreq/2),14);
						
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
						setVol();
						setPower();
						setModulation();
						setEmission(0);
						saveAllParam();
					break;
					case CMD_SET_FREQ_CAL:			
						mCbParam.FreqCal = mReceivePackage.RecvBuf[3];
					if(mCbParam.Country==COUNTRY_UK) 
					{
						setFrqCal(mCbParam.FreqCal+170);		
					}
					else
					{
						setFrqCal(mCbParam.FreqCal);		
					}	
					break;
			
					case CMD_IDLE:		
						CLS_RX_EN;
						CLS_TX_EN;
						Set_Mute(); //关闭喇叭
						mFlag.SqOpen = 0;
						mFlag.VcoIdle = 1;
						break;	
					case CMD_SET_SQ_DB:			
						mDebugSqLevel = mReceivePackage.RecvBuf[3];
						//saveDbValue();
						break;	
         case CMD_REQUEST_SQ_SET:
					 sendCommand(CMD_REQUEST_SQ_SET);
					 break;
				 case CMD_MUTE:
					mFlag.Mute=mReceivePackage.RecvBuf[3];
				  if(mFlag.Mute==1)
					{
						Set_Mute();
						SET_SPK_MUTE; //关闭喇叭
					}
				  else if(mFlag.SqOpen == 1)Cls_Mute(); 
					
					 break;
					default:
						break;		
				}
				mUartCmd=0;
			}
			
			if(mFlag.VcoIdle==0)
			{
				
				checkSq();
				
			}
			if(isSendRSSI==1)
			{						
				sendCommand(CMD_GET_RSSI);
				isSendRSSI=0;
			}
		}
		else
		{
			if(HM_DET==0)
			{
				BK4815Sleep();
				delayms(70);
				if(HM_DET==0)
				{
				  saveAllParam();					
					POWER_ON_EN=0;
					CLS_RX_EN;
					CLS_TX_EN;
					//Set_Mute();
					mFlag.SqOpen=0;
					mFlag.VcoIdle=1;
					mReceivePackage.RecvCount=0;
					EA=0;
					while(POWER_ON==0);;
					EA=1;
				}
			}
		}
	}
}
