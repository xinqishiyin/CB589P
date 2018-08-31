#define SYSTEM_H
#ifdef SYSTEM_H
#include <SC51F2832.H>

#define uint	unsigned int
#define uchar 	unsigned char
#define ulong		unsigned long
#define u8	  	unsigned char
#define u16		unsigned int
#define u32		unsigned long

#define CTCV  66

#define MAX_TX_BYTES	32	//串口最大发送字节数
#define MAX_RX_BYTES	32	//串口最大接收字节数
#define MAGIC 0xFE

enum
{
	MSG_SYNC,	
	MSG_HEADER,	
	MSG_DATA,	
};

typedef struct
{
	unsigned char IsAsq;	//是否为ASQ
	unsigned char AsqLevel;	//ASQ级别
	unsigned char SqLevel;	//SQ级别
	char AsqOpenSet[6];
	char AsqCloseSet[6];
	char SqOpenSet[28];
	char SqCloseSet[28];	
}tSqParam;		//SQ模式

typedef struct
{	 
	unsigned char FreqCal;		//系统时钟校准
	unsigned char Country;		//国家
	unsigned char Band;			//频段            //ABCDEFGHIL
	unsigned char Modu;			//调制
	unsigned char TxPower;		//发射功率
	unsigned char Sq;			//SQ级别
	unsigned char VolLevel;		//音量级别
	unsigned char RfgLevel;		//RFG级别
	unsigned char Channel;		//通道
	unsigned char UartTxBuf[MAX_TX_BYTES];
}tCbParam;		//需要发送到CB机的数据

typedef struct
{
	unsigned char RecvStatus;		//接收状态
	unsigned char RecvCount;	
	unsigned char RecvBuf[MAX_RX_BYTES];       //0:接收标识   1：接收指令码  2：接收数据长度  3：接收数据值  4：
	unsigned char InRecvBuf[MAX_RX_BYTES];
}tReceivePackage;	

typedef struct
{
	unsigned char RecvStatus;
	unsigned char RecvCount;
	unsigned char Sussece;
	unsigned char Errer;
	unsigned char Code;
}tRecive;


typedef struct
{
	unsigned char CountPublic;
	unsigned char CountButtonLed;
	unsigned char CountLcdLed;
	unsigned char CountEmg;
	unsigned char CountBeep;
	unsigned char CountLcdBlink;
}tTimer0;

typedef struct
{
	unsigned char SqOpen 		:1;
	unsigned char SqOpenButMute	:1;
	unsigned char Mute			:1;
	unsigned char VcoIdle		:1;
	unsigned char SysMode   :1;
	unsigned char SpkOpen4815 	:1;
	unsigned char CbInit;
}tFlag;

typedef struct
{
	u8	open;
	u8	close;
}tSq;
enum
{
	SYS_MODE_LINE,
	SYS_MODE_WIRELESS,
	SYS_MODE_INTERPHONE
};

typedef struct
{
	unsigned char mRecive;
	unsigned char mUartCmd;
	unsigned char mOpenSqDbLevel;
	unsigned char mLastOpenSqDbLevel;
	unsigned int mRssi;
	unsigned long int mXn31202Ch1_Tx;
	unsigned long int mXn31202Ch1_Rx;
	unsigned long int mReferenceFreq;
	unsigned long int mCurrentFreq;
	unsigned int mAsqVoltage;
	unsigned int mRssiVoltage;
	unsigned int mAgcaVoltage;
	unsigned char cTxLength;
	unsigned int cSqSum;
	unsigned char isautoRGF;
	unsigned char autoRFG;
	unsigned char isSendRSSI;
	unsigned char sengRssiCount;
	unsigned char is4815Sleep;
		unsigned char isCheckRssi;
	unsigned char CheckRssi;
}tParameter;
/*-------------------------------------------------------------------------
 *      宽窄带
 *-----------------------------------------------------------------------*/
typedef enum
{
  wide,
  narrow
}Channel_band ;      

/*-------------------------------------------------------------------------
 *      信号类型
 *-----------------------------------------------------------------------*/
typedef enum 
{
  SPEECH,
  DTMF,
  SELCALL,
  FSK,
  CTCSS,
  CDCSS  
}Signaltype; 

/*-------------------------------------------------------------------------
 *      亚音频类型
 *-----------------------------------------------------------------------*/
typedef enum
{
  NONE,
  CTCS,
  CDCS
}CTCSType; 

/*-------------------------------------------------------------------------
 *      数字亚音频相位
 *-----------------------------------------------------------------------*/
typedef enum 
{
		Normal=0,
		Inverse=1
}CDC_MODE;

/*-------------------------------------------------------------------------
 *      通道信息
 *-----------------------------------------------------------------------*/
typedef struct
{
	Channel_band band;                //宽窄带 wide为宽 narrow为窄 	
	
	u8 isVOXOpen;											//是否开启VOX
	u8 VOXLevel;											//VOX等级        0-256
	u8 VOX_Gain_Level;                //VOX麦克风增益   0-64
	
  float TX_Freq;									  //发射频率
  CTCSType TXCTCStype;              //亚音频类型
  float TX_CTCSS_Freq;						  //发射模拟亚音频率
	CDC_MODE TX_CDCS_Mode;            //发射数字亚音频正反相
  u16 TX_CDCSS_Freq;							  //发射数字亚音频率
		
	float Old_Freq;
  float RX_Freq;                    //接收频率
  CTCSType RXCTCStype;					    //接收亚音频类型
  float RX_CTCSS_Freq;							//接收模拟亚音频率
  CDC_MODE RX_CDCS_Mode;				    //接收数字亚音频正反相
	u16 RX_CDCSS_Freq;                //接收数字亚音频率
	
	
	
}Channel; 
//extern xdata uchar     mRecive;
//extern xdata uint		mMessage;
extern xdata tCbParam  	mCbParam;
extern xdata tSqParam  	mSqParam;
extern xdata tFlag  	mFlag;
extern xdata tTimer0	mTimer0;
extern xdata tSq			mSq;
extern xdata tRecive mRecive;

extern tParameter mParameter;
/*串口接收*/
//extern uchar mUartCmd;
extern tReceivePackage mReceivePackage;
extern Channel channel;

/*SQ相关*/
//extern xdata uchar mDebugSqLevel;
//extern xdata uchar mOpenSqDbLevel;
//extern xdata uchar mLastOpenSqDbLevel;

/*以下VCO相关*/
//extern xdata uchar mRssi;   
//extern xdata uchar mLastRssi;	//上次传输的RSSI
//extern xdata ulong mXn31202Ch1_Tx;
//extern xdata ulong mXn31202Ch1_Rx;
//extern xdata ulong mReferenceFreq;	//用于计算R寄存器和控制寄存器
//extern xdata ulong mChannelBaseFreq;	//第1信道的频率
//extern xdata ulong mCurrentFreq;		//当前信道的频率，通过信道1和步进频率计算
//extern xdata uint  mUkReferenceFreq[40];
//extern xdata uint mAsqVoltage;  //5V
//extern xdata uint mRssiVoltage; //5V
//extern xdata uint mAgcaVoltage; //5V

//extern u8 isSendRSSI;
//extern u8 sengRssiCount;



#define OPEN_SQ  0x026c
#define CLOSE_SQ 0x0260

#define MODDA 		xPWM0D	//占空比，modda
#define RFG   		xPWM0D	//占空比，rfg
#define SYS_FREQ   	xPWM1D	//占空比，系统时钟校准


/***************************************
*	            外部晶振               *
***************************************/
#define EXTERNAL_CRYSTAL 10250000	//10.25MHz
/***************************************
*	           控制引脚              *
***************************************/
#define SET_VCO_EN		(xP4|=0x01)
#define SET_TX_EN		(xP4|=0x02)
#define SET_AM_FM		(xP4|=0x04)
#define SET_RX_EN		(xP4|=0x08)
#define SET_MIC_MUTE	(xP4|=0x10)	
#define SET_SPK_MUTE	(xP4|=0x20)	
#define SET_FT_MUTE		(xP4|=0x40)	
#define SET_AT_MUTE		(xP4|=0x80)

#define CLS_VCO_EN		(xP4&=~0x01)
#define CLS_TX_EN		(xP4&=~0x02)
#define CLS_AM_FM		(xP4&=~0x04)
#define CLS_RX_EN		(xP4&=~0x08)
#define CLS_MIC_MUTE	(xP4&=~0x10)	
#define CLS_SPK_MUTE	(xP4&=~0x20)	
#define CLS_FT_MUTE		(xP4&=~0x40)	
#define CLS_AT_MUTE		(xP4&=~0x80)


#define PC_TX		P10
#define BEEP		P12
//#define VTX			P14
#define PL_1W		P15
#define POWER_ON_EN	P16
//#define VRX			P22
#define PL			P24

//检测脚
#define PLL_LD		P07
#define PC_RX		P11
#define POWER_ON	P17
#define HM_DET		P34

#define AT24C08_SCL P02
#define AT24C08_SDA P03

#define PWM_MODDA_OPEN	 xPWMCN |= 0x10
#define PWM_MODDA_CLOSE	 xPWMCN &= ~0x10
#define PWM_RFG_OPEN	 xPWMCN |= 0x10
#define PWM_RFG_CLOSE	 xPWMCN &= ~0x10   //RFG和MODDA为同一脚，前者接收输出，后者发射输出
#define PWM_FRQ_OPEN	 xPWMCN |= 0x20
#define PWM_FRQ_CLOSE	 xPWMCN &= ~0x20

/***************************************
*	             值映射                *
***************************************/

#define MESSAGE_RETURN_RSSI	 (0x0001 << 0)
#define MRECIVE_NONE         0
#define MRECIVE_BK4815_INTERUPT   1


//rssi信号采集
#define ADC_ASQ		    0x08	
#define ADC_RSSI		0x28
#define ADC_AGCA		0x38
//AD采样开始
#define ADC_START		0x04

#define FM 1
#define AM 0

#define YES 1
#define NO	0

#define TRY_TIMES 	3		//最多发送TRY_TIMES次指令

#define POWER_HIGH 	2
#define POWER_LOW	1
#define POWER_1W	0

//Table 1
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

#define SetBit(VAR,Place)         ( (VAR) |= (u8)((u8)1<<(u8)(Place)) )
#define ClrBit(VAR,Place)         ( (VAR) &= (u8)((u8)((u8)1<<(u8)(Place))^(u8)255) )


void SystemInit(void);
void delayms(u16 time);
void delayus(u8 i);
void Set_Mute();
void Cls_Mute();
void setVolumeLevel(uchar level);
#endif