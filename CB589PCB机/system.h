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

#define MAX_TX_BYTES	32	//����������ֽ���
#define MAX_RX_BYTES	32	//�����������ֽ���
#define MAGIC 0xFE

enum
{
	MSG_SYNC,	
	MSG_HEADER,	
	MSG_DATA,	
};

typedef struct
{
	unsigned char IsAsq;	//�Ƿ�ΪASQ
	unsigned char AsqLevel;	//ASQ����
	unsigned char SqLevel;	//SQ����
	char AsqOpenSet[6];
	char AsqCloseSet[6];
	char SqOpenSet[28];
	char SqCloseSet[28];	
}tSqParam;		//SQģʽ

typedef struct
{	 
	unsigned char FreqCal;		//ϵͳʱ��У׼
	unsigned char Country;		//����
	unsigned char Band;			//Ƶ��            //ABCDEFGHIL
	unsigned char Modu;			//����
	unsigned char TxPower;		//���书��
	unsigned char Sq;			//SQ����
	unsigned char VolLevel;		//��������
	unsigned char RfgLevel;		//RFG����
	unsigned char Channel;		//ͨ��
	unsigned char UartTxBuf[MAX_TX_BYTES];
}tCbParam;		//��Ҫ���͵�CB��������

typedef struct
{
	unsigned char RecvStatus;		//����״̬
	unsigned char RecvCount;	
	unsigned char RecvBuf[MAX_RX_BYTES];       //0:���ձ�ʶ   1������ָ����  2���������ݳ���  3����������ֵ  4��
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
 *      ��խ��
 *-----------------------------------------------------------------------*/
typedef enum
{
  wide,
  narrow
}Channel_band ;      

/*-------------------------------------------------------------------------
 *      �ź�����
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
 *      ����Ƶ����
 *-----------------------------------------------------------------------*/
typedef enum
{
  NONE,
  CTCS,
  CDCS
}CTCSType; 

/*-------------------------------------------------------------------------
 *      ��������Ƶ��λ
 *-----------------------------------------------------------------------*/
typedef enum 
{
		Normal=0,
		Inverse=1
}CDC_MODE;

/*-------------------------------------------------------------------------
 *      ͨ����Ϣ
 *-----------------------------------------------------------------------*/
typedef struct
{
	Channel_band band;                //��խ�� wideΪ�� narrowΪխ 	
	
	u8 isVOXOpen;											//�Ƿ���VOX
	u8 VOXLevel;											//VOX�ȼ�        0-256
	u8 VOX_Gain_Level;                //VOX��˷�����   0-64
	
  float TX_Freq;									  //����Ƶ��
  CTCSType TXCTCStype;              //����Ƶ����
  float TX_CTCSS_Freq;						  //����ģ������Ƶ��
	CDC_MODE TX_CDCS_Mode;            //������������Ƶ������
  u16 TX_CDCSS_Freq;							  //������������Ƶ��
		
	float Old_Freq;
  float RX_Freq;                    //����Ƶ��
  CTCSType RXCTCStype;					    //��������Ƶ����
  float RX_CTCSS_Freq;							//����ģ������Ƶ��
  CDC_MODE RX_CDCS_Mode;				    //������������Ƶ������
	u16 RX_CDCSS_Freq;                //������������Ƶ��
	
	
	
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
/*���ڽ���*/
//extern uchar mUartCmd;
extern tReceivePackage mReceivePackage;
extern Channel channel;

/*SQ���*/
//extern xdata uchar mDebugSqLevel;
//extern xdata uchar mOpenSqDbLevel;
//extern xdata uchar mLastOpenSqDbLevel;

/*����VCO���*/
//extern xdata uchar mRssi;   
//extern xdata uchar mLastRssi;	//�ϴδ����RSSI
//extern xdata ulong mXn31202Ch1_Tx;
//extern xdata ulong mXn31202Ch1_Rx;
//extern xdata ulong mReferenceFreq;	//���ڼ���R�Ĵ����Ϳ��ƼĴ���
//extern xdata ulong mChannelBaseFreq;	//��1�ŵ���Ƶ��
//extern xdata ulong mCurrentFreq;		//��ǰ�ŵ���Ƶ�ʣ�ͨ���ŵ�1�Ͳ���Ƶ�ʼ���
//extern xdata uint  mUkReferenceFreq[40];
//extern xdata uint mAsqVoltage;  //5V
//extern xdata uint mRssiVoltage; //5V
//extern xdata uint mAgcaVoltage; //5V

//extern u8 isSendRSSI;
//extern u8 sengRssiCount;



#define OPEN_SQ  0x026c
#define CLOSE_SQ 0x0260

#define MODDA 		xPWM0D	//ռ�ձȣ�modda
#define RFG   		xPWM0D	//ռ�ձȣ�rfg
#define SYS_FREQ   	xPWM1D	//ռ�ձȣ�ϵͳʱ��У׼


/***************************************
*	            �ⲿ����               *
***************************************/
#define EXTERNAL_CRYSTAL 10250000	//10.25MHz
/***************************************
*	           ��������              *
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

//����
#define PLL_LD		P07
#define PC_RX		P11
#define POWER_ON	P17
#define HM_DET		P34

#define AT24C08_SCL P02
#define AT24C08_SDA P03

#define PWM_MODDA_OPEN	 xPWMCN |= 0x10
#define PWM_MODDA_CLOSE	 xPWMCN &= ~0x10
#define PWM_RFG_OPEN	 xPWMCN |= 0x10
#define PWM_RFG_CLOSE	 xPWMCN &= ~0x10   //RFG��MODDAΪͬһ�ţ�ǰ�߽�����������߷������
#define PWM_FRQ_OPEN	 xPWMCN |= 0x20
#define PWM_FRQ_CLOSE	 xPWMCN &= ~0x20

/***************************************
*	             ֵӳ��                *
***************************************/

#define MESSAGE_RETURN_RSSI	 (0x0001 << 0)
#define MRECIVE_NONE         0
#define MRECIVE_BK4815_INTERUPT   1


//rssi�źŲɼ�
#define ADC_ASQ		    0x08	
#define ADC_RSSI		0x28
#define ADC_AGCA		0x38
//AD������ʼ
#define ADC_START		0x04

#define FM 1
#define AM 0

#define YES 1
#define NO	0

#define TRY_TIMES 	3		//��෢��TRY_TIMES��ָ��

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