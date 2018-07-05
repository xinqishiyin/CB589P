#include "system.h"
#include "KB4815.h"
#include "TM1722.h"
#include "lcd.h"



/*--------------全局变量-------------------------*/
u16 g_reg0_15_buff[16];
xdata u16 reg0_18[]=
{
 0x4816,  0x0044,0x0019,0x0027,0xB641,0xF770,
	0xF274,0x08F0,0xFF33,0xC3FA,0xA2A3,
	0x8800,0x0603,0x03FD,0x5817,0x9091,
	0x88F9,0x5800,0x415C
};
xdata u16 ramp_table[]=
{
  0x0000,0x0420,0x083F,0x0C5E,0x107C,0x149A,0x18B7,0x1CD4,
	0x20F0,0x250B,0x2927,0x2D41,0x315C,0x3575,0x398F,0x3DA7,
	0x41C0,0x45D7,0x49EF,0x4E05,0x521B,0x5631,0x5A46,0x5E5B,
	0x626F,0x6683,0x6A96,0x6EA9,0x72BB,0x76CD,0x7ADE,0x7EEF,
	0x82FF,0x870F,0x8B1E,0x8F2D,0x933B,0x9749,0x9B56,0x9F63,
	0xA36F,0xA77B,0xAB86,0xAF91,0xB39B,0xB7A5,0xBBAE,0xBFB7,
	0xC3BF,0xC7C7,0xCBCE,0xCFD5,0xD3DB,0xD7E1,0xDBE6,0xDFEB,
	0xE3EF,0xE7F3,0xEBF6,0xEFF9,0xF3FB,0xF7FD,0xFBFE,0xFFFF
};
code u16 RecvDtmfLowArr[]= 
{
0x3ab, 0x2B8, 0x2B8, 0x2B8, 0x301, 0x301, 0x301, 0x353, 
0x353, 0x353, 0x2B8, 0x301, 0x353, 0x3AB, 0x3AB, 0x3AB
};
code u16 RecvDtmfHighArr[]=
{
0x536, 0x4B7, 0x536, 0x5C2, 0x4B7, 0x536, 0x5C2, 0x4B7,
0x536, 0x5C2, 0x65E, 0x65E, 0x65E, 0x65E, 0x4B7, 0x5C2
};
code u16 SendDtmfLowArr[]= 
{
0xD0C, 0x9AA, 0x9AA, 0x9AA, 0xAAD, 0xAAD, 0xAAD, 0xBD0,
0xBD0, 0xBD0, 0x9AA, 0xAAD, 0xBD0, 0xD0C, 0xD0C, 0xD0C
};

code u16 SendDtmfHighArr[]=
{
0x1285,0x10C3,0x1285,0x147A,0x10C3,0x1285,0x147A,0x10C3,
0x1285,0x147A,0x16A4,0x16A4,0x16A4,0x16A4,0x10C3,0x147A
};

/*-------------------------------------------------------------------------
*函数：BK_Read_Reg  寄存器读出
*参数：reg 寄存器地址  
*返回值：无
*-------------------------------------------------------------------------*/
u16 BK_Read_Reg(u8 reg)
{
	u8 i;
	u16 dat;	
	reg <<= 1;
	reg |= 0x01;  //读的地址
	BK4815_SCK = 0;
	BK4815_SCN = 0;  
	for(i=0; i<8; i++)
	{
		if(reg & 0x80) OUT_BK4815_SDA = 1;	else OUT_BK4815_SDA = 0;  
		BK4815_SCK = 1;
		BK4815_SCK = 0;
		reg <<= 1;
	}
	KB4815_SDA_ToInt
	for(i=0; i<16; i++)
	{
		dat <<= 1;		
		dat |= IN_BK4815_SDA;	
		BK4815_SCK = 1;
		BK4815_SCK = 0;
	}
       
	BK4815_SCN = 1; 
	KB4815_SDA_ToOut
	return dat;
}
/*-------------------------------------------------------------------------
*函数：BK_Write_Reg  写入寄存器
*参数：reg 寄存器地址  dat 写入的值 
*返回值：无
*-------------------------------------------------------------------------*/
void BK_Write_Reg(u8 reg, u16 dat)
{
	u8 i;	
	reg <<= 1;	 //写的地址

	BK4815_SCK = 0;
	BK4815_SCN = 0;   
	
	for(i=0; i<8; i++)
	{
		if(reg & 0x80) OUT_BK4815_SDA = 1;
		else OUT_BK4815_SDA = 0;
		BK4815_SCK = 1;
		BK4815_SCK = 0;
		reg <<= 1;
	}
	for(i=0; i<16; i++)
	{
		if(dat & 0x8000) OUT_BK4815_SDA = 1;
		else OUT_BK4815_SDA = 0;
		BK4815_SCK = 1;
		BK4815_SCK = 0;
		dat <<= 1;
	}
	BK4815_SCN = 1; 
}
void BK_Ramp_Table_Init()
{
	u8 i;
	for(i=0;i<64;i++)
		BK_Write_Reg(42,ramp_table[i]);
}
/*-------------------------------------------------------------------------
*函数：initKB4815  4815初使化设置
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void initBK4815(void)
{
	
	u8 i;
	u16 val=0;

	BK4815_SCN = 1; 
	delayus(2);
	BK4815_SCN = 1;
	delayms(1);
	
	//BK_Write_Reg(117,0x0492);                 //RX Audio:SelCall    GPIO1 :Output low
	IDLE
	delayms(200);
	BK_Write_Reg(12,0xfbab);                   //
	delayus(5);
//	BK_Write_Reg(115, 0x8400);//updated           //DTMF interrupt
//		BK_Write_Reg(116, 0x0000);//updated
	for(i=1;i<=15;i++)
	{
		g_reg0_15_buff[i]=reg0_18[i];
		BK_Write_Reg(i, g_reg0_15_buff[i]);
	}
	//for(i=0;i<64;i++)
	//	BK_Write_Reg(42,ramp_table[i]);
	
	BK_Ramp_Table_Init();

	BK_Write_Reg(16, 0x88f9);//0x3684:IF=88K,0x4B80: IF=121.875kHz, 0x05CF: IF=9.375kHz
	BK_Write_Reg(17, 0x5800);//updated 4812
	BK_Write_Reg(18, 0x415c);//updated 4812
	BK_Write_Reg(19, 0x08a0);//updated 4812
	BK_Write_Reg(20, 0x0000);//updated

	BK_Write_Reg(21, 0x002e);
	BK_Write_Reg(22, 0x3200);//updated
	//BK_Write_Reg(23, 0x0000);//updated
	BK_Write_Reg(24, 0x086c);
	BK_Write_Reg(25, 0x13ba);
	BK_Write_Reg(26, 0x0000);
	//BK_Write_Reg(27, 0x0000);
	BK_Write_Reg(28, 0x0000);
	BK_Write_Reg(29, 0x0000);
	BK_Write_Reg(30, 0x0000);

	BK_Write_Reg(31, 0x0000);
	BK_Write_Reg(32, 0x0000);
	BK_Write_Reg(33, 0x0000);
	BK_Write_Reg(34, 0x0740);
	BK_Write_Reg(35, 0x0000);
	BK_Write_Reg(36, 0x8000);//updated
	BK_Write_Reg(37, 0x04D5);//updated
	BK_Write_Reg(38, 0xE000);//updated,bit13  0: 23 bits CDCSS,1: 24 bits CDCSS ,121203
	BK_Write_Reg(39, 0x0013);//updated
	BK_Write_Reg(40, 0x8820);//updated

	BK_Write_Reg(41, 0x2050);
	//BK_Write_Reg(42, 0x0000);//updated
	BK_Write_Reg(43, 0x403F);//updated
	BK_Write_Reg(44, 0x8a2e);//updated 4812
	BK_Write_Reg(45, 0x1b80);//updated 4812
	BK_Write_Reg(46, 0x0000);
	BK_Write_Reg(47, 0x0000);

        BK_Write_Reg(48, 0x6c0d);
        BK_Write_Reg(49, 0xd1d5);
        BK_Write_Reg(50, 0xf717);
        BK_Write_Reg(51, 0x0735);
        BK_Write_Reg(52, 0x6d72);
        BK_Write_Reg(53, 0xcd0b);
        BK_Write_Reg(54, 0xddac);
        BK_Write_Reg(55, 0x15d8);
        BK_Write_Reg(56, 0x6f82);
        BK_Write_Reg(57, 0xc723);
        BK_Write_Reg(58, 0xc7a5);
        BK_Write_Reg(59, 0x2206);
        BK_Write_Reg(60, 0x721b);
        BK_Write_Reg(61, 0xc235);
        BK_Write_Reg(62, 0x1100);
        BK_Write_Reg(63, 0x0000);

        
        
        
	BK_Write_Reg(64, 0x8000);
	BK_Write_Reg(65, 0xc000);
	BK_Write_Reg(66, 0xd083);//updated
	BK_Write_Reg(67, 0x2000);//updated
	BK_Write_Reg(68, 0x8000);
	BK_Write_Reg(69, 0x27FF);//updated
	BK_Write_Reg(70, 0x40fe);

	BK_Write_Reg(71, 0x0a18);//updated
	BK_Write_Reg(72, 0xe002);//updated
	
	BK_Write_Reg(73, 0x1a0a);//updated
	BK_Write_Reg(74, 0x0000);//updated
	BK_Write_Reg(75, 0x7a80);
	BK_Write_Reg(76, 0xE204);
	BK_Write_Reg(77, 0x0000);
	BK_Write_Reg(78, 0x0000);
	BK_Write_Reg(79, 0x0000);
	BK_Write_Reg(80, 0x0000);

	BK_Write_Reg(81, 0xB200);
	BK_Write_Reg(82, 0x0000);
	BK_Write_Reg(83, 0x8000);
	BK_Write_Reg(84, 0xFC40);
	BK_Write_Reg(85, 0x70a6);
	BK_Write_Reg(86, 0x0000);
	BK_Write_Reg(87, 0x0000);
	BK_Write_Reg(88, 0x0000);
	BK_Write_Reg(89, 0xff81);//updated
	BK_Write_Reg(90, 0x7D26);//updated
	
	BK_Write_Reg(91, 0x0000);//updated
	BK_Write_Reg(92, 0x0000);//updated
	BK_Write_Reg(93, 0x0000);
	BK_Write_Reg(94, 0x00e8);//updated
	BK_Write_Reg(95, 0x0237);
	BK_Write_Reg(96, 0x0080);
	BK_Write_Reg(97, 0x0000);
	BK_Write_Reg(98, 0x0005);
	BK_Write_Reg(99, 0x0000);
	BK_Write_Reg(100, 0x0080);

	BK_Write_Reg(101, 0x0000);
	BK_Write_Reg(102, 0x0000);
	BK_Write_Reg(103, 0x431f);
	BK_Write_Reg(104, 0x0000);
	BK_Write_Reg(105, 0x0000);
	BK_Write_Reg(106, 0xcc31);//Firmware Version
	BK_Write_Reg(107, 0x3415);
	BK_Write_Reg(108, 0x6927);//updated
	BK_Write_Reg(109, 0x6600);//updated
	BK_Write_Reg(110, 0x0000);//updated,121203

	BK_Write_Reg(111, 0x0000);
	BK_Write_Reg(112, 0xA000);//updated
	BK_Write_Reg(113, 0x893b);//updated
	BK_Write_Reg(114, 0x91b9);//updated
	

	BK_Write_Reg(117, 0x0408);//updated           //DTMF
	//BK_Write_Reg(118, 0x0400);
	BK_Write_Reg(119, 0x02b4);
	BK_Write_Reg(120, 0x1103);

	BK_Write_Reg(121, 0x2014);
	BK_Write_Reg(122, 0x46a3 );
	BK_Write_Reg(123, 0x0002);//updated
	BK_Write_Reg(124, 0xf3ac);//updated
	BK_Write_Reg(125, 0x76b5);
	BK_Write_Reg(126, 0xfff5);//updated
	BK_Write_Reg(127, 0x3568);//updated



	/*---------------------------------宽窄带---------------------------------*/
	val = BK_Read_Reg(1);
	val &= 0x7FFF;	
	if( channel.band== wide)	val |= 0x8000;
	BK_Write_Reg(1, val);	


	
  //BK_DTMF_SET_CODE(mDtmfRecive.dtmfCode[0]);
	/*---------------------------------VOX设置---------------------------------*/
	if(channel.isVOXOpen==1)
	{
		val=0;
		val|=0x8000;
		val|=(channel.VOXLevel<<6);
		val|=channel.VOX_Gain_Level;
		BK_Write_Reg(22,val);
	}	
	//BK_DTMF_SET_CODE(mDtmfRecive.dtmfCode[0]);

	
//EX1 = 1; 		                      //使能INT1中断	                      //使能INT1中断
	
//			/*-----------------允许DTMF接收-----------------------*/
//	val=BK_Read_Reg(66);
//	val|=(0x0001<<11);
//	BK_Write_Reg(66,val);	

//	val=BK_Read_Reg(78);
//	val&=0xf000;
//	val|=0x010f;
//	BK_Write_Reg(78,val);
//	
//	val=BK_Read_Reg(115);
//	val|=(0x0001<<10);
//	BK_Write_Reg(115,val);
//	

//		for(i = 0; i < 16; i++)
//	{
//		BK_Write_Reg(77, ((i) << 12) | 
//					 ((1  << 11)) |
//					 RecvDtmfHighArr[i]);
//		
//		BK_Write_Reg(77, ((i) << 12) |
//					 RecvDtmfLowArr[i]);
//	}
//	

	EnterBK4815RX();
}

/*-------------------------------------------------------------------------
*函数：BK4815Sleep  4815睡眠模式
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void BK4815Sleep(void)
{
	BK_Write_Reg(12,	0xFFFF);
	OUT_APC = 0;
	IDLE;
}
u16 BK4815RssiAndSnr()
{
	return (BK_Read_Reg(68)&0x3f7f);
}
/*----------------------------------------------------------------
  ??:BK_Enable_TX_InbandSignal ??TX???? 
  ??:?
	??:?
*----------------------------------------------------------------*/
void BK_Enable_TX_InbandSignal()
{
	u16 val = 0;
	val = BK_Read_Reg(40);
	val |= 0x8000;
	
	BK_Write_Reg(40, val);
}
/*----------------------------------------------------------------
  ??:BK_Disable_TX_InbandSignal ??TX???? 
  ??:
	??:?
*----------------------------------------------------------------*/
void BK_Disable_TX_InbandSignal()
{
	u16 val = 0;
	val = BK_Read_Reg(40);
	val &= 0x7FFF;	
	BK_Write_Reg(40, val);
}
///*-------------------------------------------------------------------------
//*函数：BK_DTMF_RX_Read  读DTMF编码
//*参数：无
//*返回值：DTMF编码
//*-------------------------------------------------------------------------*/
//u8 BK_DTMF_RX_Read()
//{
//	u8 dtmf_addr=0;
//	u16 val=0;
//	val=BK_Read_Reg(78);
//	dtmf_addr=(val>>4)&0x0f;
//	return dtmf_addr;
//}
/*----------------------------------------------------------------
  ??:BK_RX2TX ?????
  ??:
	??:?
*----------------------------------------------------------------*/
void BK_RX2TX()
{
	u16 val;
	g_reg0_15_buff[12]=0xf823;//reg7<0>=1, High supply LDO
  BK_Write_Reg(12, g_reg0_15_buff[12]);
  val=BK_Read_Reg(112);
  val|=0x2000;
  BK_Write_Reg(112,0x4000);
  BK_Write_Reg(112,0xe000);
  OUT_APC=1;	
}
/*----------------------------------------------------------------
  ??:BK_TX2RX ?????
  ??:
	??:?
*----------------------------------------------------------------*/
void BK_TX2RX()
{	
	BK_Write_Reg(112,0x0000);
  BK_Write_Reg(112,0xa000);
  //val=BK_Read_Reg(112);
  //val&=0xdfff;
  //BK_Write_Reg(112,val);
  g_reg0_15_buff[12]=0x0603;//reg7<0>=1, High supply LDO
  BK_Write_Reg(12, g_reg0_15_buff[12]);
	OUT_APC=0;
}
//void BK_DTMF_INTERUPT_CLEAR()
//{
//	u16 val;
//	
//	
//	val=BK_Read_Reg(116);
//	val|=0x0400;
//	BK_Write_Reg(116,val);
//	val=BK_Read_Reg(78);
//	val&=0xff0f;
//	BK_Write_Reg(78,val);
//}
/*
void BK_DTMF_SET_CODE(u8 dat)
{
	u16 val;
	val=BK_Read_Reg(78);
	val&=0xfff0;
	val|=dat;
	BK_Write_Reg(78,val);
}
*/
///*-------------------------------------------------------------------------
//*??:BK_DTMF_TX  DTMF??
//*??:  *buf DTMF??   len ???? 
//*???:?
//*-------------------------------------------------------------------------*/
//void BK_DTMF_TX(u8 buf)
//{	
//	u16 val;

//	IDLE;
//	OPEN_TX	
//	val = BK_Read_Reg(40);
//	val &= 0x1FFF;
//	val |= 0x2000;				/* bit14,13 :01 */  //revised 2009.12.09, inband send was enabled in BK_DTMF_TX
//	BK_Write_Reg(40, val);		/* DTMF */	
//	BK_RX2TX();
//	BK_Disable_TX_InbandSignal();
//	OUT_APC=1;
//	BK_Write_Reg(24, SendDtmfLowArr[buf]);
//	BK_Write_Reg(25, SendDtmfHighArr[buf]);
//	BK_Enable_TX_InbandSignal();		
//	delayms(70); /* DelayMs 40ms */
//	BK_Disable_TX_InbandSignal();			
//	
//}

//u8 isDtmfSendOK() 
//{
//	u8 dat;
//	while(PPT_KEY==0)
//	{
//		mRecive=MRECIVE_NONE;		
//		OUT_APC=1;
//		BK_DTMF_INTERUPT_CLEAR();
//		BK_DTMF_TX((mDtmfRecive.dtmfCode&0xf0)>>4);		
//		OUT_APC=0;
//		EnterBK4815RX();		
//		delayms(70);
//		dat=BK_DTMF_RX_Read();
//		
//		if((dat==(mDtmfRecive.dtmfCode&0x0f))&&(mRecive==MRECIVE_BK4815_INTERUPT)) 		
//		{
//			mRecive=MRECIVE_NONE;		
//			return 1;
//		}	
//	}
//	return 0;
//}

/*-------------------------------------------------------------------------
*函数：StartTx  发射
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void StartBK4815TX(void)
{	
	u16 val = 0;	
	u16 if_h=0;
	u16 if_l=0;
	u32 val1;

	u8 freqband=0;
	OUT_APC = 0;
	IDLE;
	EN_MIC=0;
	//SPK_EN=0;
	OPEN_TX		
		/*---------------------------------接收频率---------------------------------*/ 
		g_reg0_15_buff[4]&=0xffe7;
		g_reg0_15_buff[4]|=0x0100;freqband=12;if_l=0xd01c;if_h=0xffef;

	BK_Write_Reg(4,g_reg0_15_buff[4]);
 	
  val1=(u32)(channel.RX_Freq*freqband*(645277.54));

  BK_Write_Reg(113,(u16)(val1/65536));
  BK_Write_Reg(114,(u16)(val1%65536));
	BK_Write_Reg(125,0x76b5);
	BK_Write_Reg(126,if_h);
	BK_Write_Reg(127,if_l);

  delayms(10);
	
		val = BK_Read_Reg(40);
		val &= 0x1FFF;
		val |= 0x2000;				/* bit14,13 :01 */  //revised 2009.12.09, inband send was enabled in BK_DTMF_TX
		BK_Write_Reg(40, val);		/* DTMF */

	
	
	
	BK_Write_Reg(112,0x4000);                //RX TO TX	
	BK_Write_Reg(112,0xe000);                //RX TO TX	
	BK_Write_Reg(12, 0xf823);                //TX POWER UP	
//	if(isDtmfSendOK())
//	{		
//		
		BK_Write_Reg(44, 0x0017);//updated 4812
		BK_Write_Reg(17, 0x5800);//updated 4812
		val = BK_Read_Reg(40);
			val &= 0x9FFF;
			val |= 0x8000;				/* inband type bit14,13 :00 */
			BK_Write_Reg(40, val);		/* Speech */
		BK_RX2TX();
		LCD_TX(1);  	
		IDLE;
	  OPEN_TX	
		if(mCbParam.TxPower==POWER_HIGH)		LCD_STRENGTH(5);	
						else if(mCbParam.TxPower==POWER_1W) LCD_STRENGTH(1);
						else  LCD_STRENGTH(3);			
		OUT_APC=1;
		EN_MIC = 1;
		mMenu.isTx=1;
		LED_TX = 1;	
		EN_MIC=1;
//	}
//	else
//	{
//		EnterBK4815RX();
//	}
}
/*-------------------------------------------------------------------------
*函数：EnterRX  接收
*参数：无
*返回值：无
*-------------------------------------------------------------------------*/
void EnterBK4815RX(void)
{

	u16 val = 0;	
	u16 if_h=0;
	u16 if_l=0;
	u32 val1;
	u8 freqband=0;
	OUT_APC = 0;
	IDLE;
	EN_MIC=0;
	OPEN_RX

		/*---------------------------------接收频率---------------------------------*/ 
		g_reg0_15_buff[4]&=0xffe7;
	
	g_reg0_15_buff[4]|=0x0100;freqband=12;if_l=0xd01c;if_h=0xffef;
	BK_Write_Reg(4,g_reg0_15_buff[4]);
 	
  val1=(u32)(channel.RX_Freq*freqband*(645277.54));

  BK_Write_Reg(113,(u16)(val1/65536));
  BK_Write_Reg(114,(u16)(val1%65536));
	BK_Write_Reg(125,0x76b5);
	BK_Write_Reg(126,if_h);
	BK_Write_Reg(127,if_l);

  delayms(10);
	
	//EX1 = 1; 		                      //使能INT1中断	                      //使能INT1中断		
  BK_Write_Reg(112,0xa000); 
  BK_Write_Reg(12, 0x0603);
}

//void BK_DTMF_RECIVE()
//{
//	uchar dat=0;
//	uchar i=0;  
//	dat=BK_DTMF_RX_Read();

//	if(dat == ((mDtmfRecive.dtmfCode&0xf0)>>4))
//	{

//		delayms(10);
//		BK_DTMF_TX(mDtmfRecive.dtmfCode&0x0f);					
//		mDtmfRecive.DtmfSussece=1;
//		mDtmfRecive.DtmfRecvCount=0;
//		EnterBK4815RX();
//		BK_TX2RX();
//		delayms(50);					
//	}  			
//	else
//	{
//		mDtmfRecive.DtmfSussece=0;
//					
//	}
//	BK_DTMF_INTERUPT_CLEAR();
//}