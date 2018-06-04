#define SYSTEM_H
#ifdef SYSTEM_H
#include <SC51F2832.H>

#define uint	unsigned int
#define uchar 	unsigned char
#define u8	  	unsigned char
#define u16		unsigned int
#define u32		unsigned long


sbit ADGO = ADCON^2;
#define Channel_Start_Adress  0x03e8

#define MAX_TX_BYTES	32	//串口最大发送字节数
#define MAX_RX_BYTES	32	//串口最大接收字节数

#define Batt_AD  0
#define Key1     1
#define Key2     2


#define SendDtmfTime    40

/***************************************
*	             值映射                *
***************************************/
//LED开关值映射
#define ON			  	1 
#define OFF			  	0 
#define LIGHT_ON		1
#define LIGHT_OFF		0
#define OPEN			  1 
#define CLOSE		  	0

#define FM          1
#define AM          0

#define YES         1
#define NO        	0

#define SQ_val      0x00
#define ASQ_val     0x20

#define TRY_TIMES 	3		//最多发送TRY_TIMES次指令

#define POWER_HIGH 	2
#define POWER_LOW	  1
#define POWER_1W	  0

//国家列表
#define COUNTRY_EU	0
#define COUNTRY_CE	1
#define COUNTRY_UK	2
#define COUNTRY_PL	3
#define COUNTRY_I0	4
#define COUNTRY_I2	5
#define COUNTRY_DE	6
#define COUNTRY_IN	7

//Table 2
#define COUNTRY_RU	8
#define COUNTRY_PC	9
#define COUNTRY_PX	10

//Table 3
#define COUNTRY_AU	11
#define COUNTRY_NL	12
#define COUNTRY_AM	13



/*---------------------时间宏定义--------------------------*/
#define POWER_ON_DELAY             2000				 //开机延时启动时间
#define POWER_ON_ALLLCD_TIME       1000        //开机后全屏显示LCD时长
#define POWER_ON_SHOW_CONTRY       700        //开机显示国家时长

#define KEY_BEEP_TIME              120        //按键音时长

#define MENU_UP_DOWN_SPEED         100         //菜单连续加减延时
#define BACK_TIME                  400          //返回主界面时间

#define SHOW_TWINKLE_TIME          23           //闪烁时间间隔

#define BUTTON_LED_TIME            200          //按键灯时长

#define HOULD_TIME                 20        //扫描及守候接收间隔时长

#define BUTTON_TIME                5

#define SCAN_SPEED_DELAY           4         //扫描/双频守候间隔时间

#define KEY_COMBOlIMIT             40         //x组合按件时

/*---------------------引脚宏定义--------------------------*/
#define AT24C08_SCL                P02
#define AT24C08_SDA                P03
#define POW_OUT                    P15
#define LCD_EN                     P05
#define LCD_CLK                    P06
#define LCD_DAT                    P07
#define PPT_KEY                  	 P24
#define EN_MIC                     P17
#define SPK_EN                     P16
#define LED_TX                     P13
#define LED_RX                     P14
#define LIGHT_B                    P10
#define LIGHT_G                    P11
#define LIGHT_R                    P12
#define LCD_LED                    P04
#define AT24C08_SCL                P02
#define AT24C08_SDA                P03

typedef struct
{
	unsigned char CountryTable;	  //国家列表
	unsigned char Country;		    //国家
	unsigned char Band;			      //频段
	unsigned char Modu;			      //调制
	unsigned char TxPower;		    //发射功率
	unsigned char Sq;			        //SQ级别
	unsigned char VolLevel;		    //音量级别
	unsigned char RfgLevel;		    //RFG级别
	unsigned char  Channel;		    //通道
	unsigned char  LastChannel;    //最后使用通道
	
	unsigned char UartTxBuf[MAX_TX_BYTES];
}tLCDParam;	


enum
{
	SYS_MODE_LINE,
	SYS_MODE_WIRELESS,
	SYS_MODE_INTERPHONE
} ;


/*--------------------------------------*/

typedef struct
{	 
	unsigned char FreqCal;		  //系统时钟校准
	unsigned char CountryTable;	//国家列表
	unsigned char Country;		  //国家
	unsigned char Band;			    //频段
	unsigned char Modu;			    //调制
	unsigned char TxPower;		  //发射功率
	unsigned char Sq;			      //SQ级别
	unsigned char VolLevel;		  //音量级别
	unsigned char RfgLevel;		  //RFG级别
	unsigned char  Channel;		  //通道
	unsigned char  LastChannel;  //最后使用通道
	unsigned char LastBand;

	unsigned char forEMGChannel;
	unsigned char UartTxBuf[MAX_TX_BYTES];
}tCbParam;										//需要发送到CB机的数据

typedef struct
{
	unsigned char IsAsq;			  //是否为ASQ
	unsigned char AsqLevel;	    //ASQ级别
	unsigned char SqLevel;	    //SQ级别
	unsigned char Scan			;
	unsigned char ScanDir		;
	unsigned char ScanHould ;
	unsigned char DWSet;
	unsigned char DWHould;
	unsigned int HouldTime;
	unsigned char DisHould;
	unsigned char OpenOrClose;
	char  OpenSet;
	char  CloseSet;
	
}tSqParam;										//SQ模式


typedef struct
{
	unsigned char LcdColor;					//LCD背景灯颜色
	unsigned char ButtonLedSwitch;	//按键指示灯开关
	unsigned char ButtonToneSwitch;	//按键提示音开关
	unsigned char SpkerSwitch;
	unsigned char isCheckHitPower;

}tHmSetting;											//手咪上面需要设置的数据

typedef struct
{
	unsigned char Rssi;
	unsigned char Ack;
	unsigned char ActivityChannel;
	unsigned char DebugSqLevel;
	unsigned char MaxChannel;
	unsigned char MinChannel;
	unsigned char LastChannel;
	unsigned char LastBand;
	unsigned char BackChannel;
	unsigned char BackBand;
	unsigned char ThisChannel;
	unsigned char ThisBand;
	unsigned char MaxCountry;
	unsigned char MinCountry;
	unsigned char HitPower;
	unsigned char isMute;
	unsigned char DWChannel1;
	unsigned char DWBand1;
	unsigned char DWModu1;
	unsigned char DWChannel2;
	unsigned char DWBand2;
	unsigned char DWModu2;
	unsigned char isLastChannel;
	unsigned char KeyComboLimit;
	unsigned char isKeyCombo;
	unsigned char isUK;
}tSysParam;                      //
enum 
{
	MSG_SYNC,	
	MSG_HEADER,	
	MSG_DATA
};
typedef struct
{
	unsigned char DtmfRecvStatus;
	unsigned char DtmfRecvCount;
	unsigned char DtmfSussece;
	unsigned char DtmfErrer;
  unsigned char dtmfCode;
}tDtmfRecive;


typedef struct
{
	unsigned char SysMode		:2;
	unsigned char Emg			:2;
	unsigned char WhatBlink		:2;
	unsigned char Uart0Sending	:1;
	unsigned char HaveBand		:1;
	unsigned char BandChanged	:1;
	unsigned char SpkOpen 		:1;
	unsigned char InMainFace	:1;
	unsigned char InMenu		:1;
	unsigned char MenuSetOk		:1;
	
	unsigned char NeedRefreshShow:1;
	unsigned char NeedSaveData	:1;
	unsigned char NumberShow	:1;
	unsigned char Mute			:1;
	unsigned char ChangeTxPower :1;
}tFlag;

     

typedef struct
{
	unsigned char RecvStatus;		//接收状态
	unsigned char RecvCount;	
	unsigned char RecvBuf[MAX_RX_BYTES];
}tReceivePackage;	
typedef struct
{
	u8	open;
	u8	close;
}tSq;
/*---------------------------------------*/




#define SetBit(VAR,Place)         ( (VAR) |= (u8)((u8)1<<(u8)(Place)) )
#define ClrBit(VAR,Place)         ( (VAR) &= (u8)((u8)((u8)1<<(u8)(Place))^(u8)255) )

#define SET_AT24C08_SDA_OUT      {P0MDL &= 0x3F; P0MDL |= 0x80;}
#define SET_AT24C08_SDA_IN       {P0MDL &= 0x3F; P0MDL |= 0x40;}

#define MRECIVE_NONE         0
#define MRECIVE_BK4815_INTERUPT   1


#define POW_IN                    ((xP4&0x04)==0? 0:1)
#define VCC_DET                   ((xP4&0x02)==0? 0:1)
#define VCC_DET1                  ((xP4&0x01)==0? 0:1)
#define VCC_BATT                  ((xP4&0x80)==0? 0:1)

extern xdata unsigned long 	mMessage;
extern xdata tCbParam  		mCbParam;
extern xdata tSqParam  		mSqParam;
extern xdata tHmSetting  	mHmSetting;
extern xdata tSysParam	    mSysParam;
extern xdata tFlag  		mFlag;
extern xdata uchar     mRecive;
extern xdata tSq			mSq;
extern xdata uchar   isSendDtmf;
extern xdata uchar     mRecive;
extern xdata tDtmfRecive mDtmfRecive;
extern xdata u16  dtmfNum;
extern xdata u8  isPowerOn;
extern u8 sendDtmfT;
void SystemInit(void);
void delayms(u16 time);
void delayus(u8 i);
void UART0_Init();
void IO_Init();

/*-------------------------------------------------------------------------
*函数：Get_AD  AD值采集
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
u8 Get_AD(u8 AD_IO);
/*-------------------------------------------------------------------------
*函数：Get_BATT_AD  AD值采集
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
u16 Get_BATT_AD();
#endif