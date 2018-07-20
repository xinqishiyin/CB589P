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

#define MAX_TX_BYTES	32	//����������ֽ���
#define MAX_RX_BYTES	32	//�����������ֽ���

#define Batt_AD  0
#define Key1     1
#define Key2     2


#define SendDtmfTime    10

/***************************************
*	             ֵӳ��                *
***************************************/
//LED����ֵӳ��
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

#define TRY_TIMES 	3		//��෢��TRY_TIMES��ָ��

#define POWER_HIGH 	2
#define POWER_LOW	  1
#define POWER_1W	  0

//�����б�
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



/*---------------------ʱ��궨��--------------------------*/
//#define POWER_ON_DELAY             2000				 //������ʱ����ʱ��
#define POWER_ON_ALLLCD_TIME       1000        //������ȫ����ʾLCDʱ��
#define POWER_ON_SHOW_CONTRY       700        //������ʾ����ʱ��

//#define KEY_BEEP_TIME              120        //������ʱ��

#define MENU_UP_DOWN_SPEED         100         //�˵������Ӽ���ʱ
#define BACK_TIME                  2000          //����������ʱ��

#define SHOW_TWINKLE_TIME          120           //��˸ʱ����

#define BUTTON_LED_TIME            2000          //������ʱ��

#define HOULD_TIME                 20        //ɨ�輰�غ���ռ��ʱ��

#define BUTTON_TIME                4
#define SHUT_DOWN_TIME             
#define SCAN_SPEED_DELAY           6         //ɨ��/˫Ƶ�غ���ʱ��
#define DW_SPEED_DELAY             7     
#define KEY_COMBOlIMIT             200         //x��ϰ���ʱ
#define KEY_COMBOlIMIT_LONG        2500        //�л�����ϰ���ʱ
#define KEY_LONG_MUTE_TIME          50        //


/*---------------------���ź궨��--------------------------*/
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
	unsigned char CountryTable;	  //�����б�
	unsigned char Country;		    //����
	unsigned char Band;			      //Ƶ��
	unsigned char Modu;			      //����
	unsigned char TxPower;		    //���书��
	unsigned char Sq;			        //SQ����
	unsigned char VolLevel;		    //��������
	unsigned char RfgLevel;		    //RFG����
	unsigned char  Channel;		    //ͨ��
	unsigned char  LastChannel;    //���ʹ��ͨ��
	
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
	unsigned char FreqCal;		  //ϵͳʱ��У׼
	unsigned char CountryTable;	//�����б�
	unsigned char Country;		  //����
	unsigned char Band;			    //Ƶ��
	unsigned char Modu;			    //����
	unsigned char TxPower;		  //���书��
	unsigned char Sq;			      //SQ����
	unsigned char VolLevel;		  //��������
	unsigned char RfgLevel;		  //RFG����
	unsigned char  Channel;		  //ͨ��
	unsigned char  LastChannel;  //���ʹ��ͨ��
	unsigned char LastBand;

	unsigned char forEMGChannel;
	unsigned char UartTxBuf[MAX_TX_BYTES];
}tCbParam;										//��Ҫ���͵�CB��������


typedef struct
{
	unsigned char IsAsq;			  //�Ƿ�ΪASQ
	unsigned char AsqLevel;	    //ASQ����
	unsigned char SqLevel;	    //SQ����
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
	
}tSqParam;										//SQģʽ


typedef struct
{
	unsigned char LcdColor;					//LCD��������ɫ
	unsigned char ButtonLedSwitch;	//����ָʾ�ƿ���
	unsigned char ButtonToneSwitch;	//������ʾ������
	unsigned char SpkerSwitch;
	unsigned char isCheckHitPower;
  unsigned char isPowLow;
	unsigned int PowLowShutTime;
}tHmSetting;											//����������Ҫ���õ�����

typedef struct
{
	unsigned char Rssi;
	unsigned char RssiChange;
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
	unsigned int  KeyComboLimit;
	unsigned int  KeyComboTableLimit;
	unsigned char isKeyCombo;
	unsigned char isKeyComTable;
	unsigned char isUK;
	unsigned char DWChannel;
	unsigned char DWBand;
	unsigned char DWModu;
	unsigned int  ButtonLED;
	unsigned char TXorRXLEDChange;
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
	unsigned char Mute			:1;
	unsigned char ChangeTxPower :1;
}tFlag;

     

typedef struct
{
	unsigned char RecvStatus;		//����״̬
	unsigned char RecvCount;	
	unsigned char RecvBuf[MAX_RX_BYTES];
}tReceivePackage;	
typedef struct
{
	u8	open;
	u8	close;
}tSq;
/*---------------------------------------*/


/*-------------�˵�����---------------------------*/
typedef struct
{
	unsigned char isBussy;	      //æ ���ص���
	unsigned char MenuIndex;		  //�˵�����
	unsigned char BackMenuIndex;   //���ز˵������洢
	unsigned int Back_Channel_Time;	//�������˵�ʱ��
	unsigned char isLock;			      //�Ƿ�����
	unsigned char emgIndex;		    //�����ŵ���ʶ
	unsigned char isTx;			        //����״̬
	
}tMenu;	
typedef struct
{
	unsigned char KeyIndex1;	      //KEY1 ADֵ ����
	unsigned char KeyIndex2;		  //KEY2 ADֵ ����
	unsigned int ShortPressLimit1;	//����1�̰�ʱ��
	unsigned int ShortPressLimit2;	//����2�̰�ʱ��
	unsigned int Power_Time;	//��Դ������ʱ��
	unsigned int DoublePress_Limit;	//˫��ʱ��
	unsigned char DoublePress_Count;  //˫������
	unsigned char Press_Count;          //��������
	unsigned int  MutePress_Timelimit;  //��������Դ������ʱ��
	unsigned int  MuteDoublePress_Timelimit;  //��������Դ��˫��ʱ��
	unsigned int LongSq_Time;          //����SQ�л�ʱ��
	unsigned int MutePress_Count;      //��Դ������
	
	
	unsigned char key_CombleFAF;
 	unsigned char	key_CombleFUPAFRFGSCAN;
 	unsigned char	key_UnLockcount;
 	unsigned char	key_CombleFDN;
 	unsigned char	key_CombleFUPEMG;
	unsigned char key_CombleSQSet;
	unsigned char Pow_Press;
	unsigned int Pow_Press_Time;
}tKey;
/*-------------------------------------------------------------------------
 *      ͨ����Ϣ
 *-----------------------------------------------------------------------*/
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
typedef struct
{
	Channel_band band;                //��խ�� wideΪ�� narrowΪխ 	
	
	u8 isVOXOpen;											//�Ƿ���VOX
	u8 VOXLevel;											//VOX�ȼ�        0-256
	u8 VOX_Gain_Level;                //VOX��˷�����   0-64
	u8 Sqleve;                        //SQ�ȼ�
	
  float TX_Freq;									  //����Ƶ��
  CTCSType TXCTCStype;              //����Ƶ����
  float TX_CTCSS_Freq;						  //����ģ������Ƶ��
	CDC_MODE TX_CDCS_Mode;            //������������Ƶ������
  u16 TX_CDCSS_Freq;							  //������������Ƶ��
		
	
  float RX_Freq;                    //����Ƶ��
  CTCSType RXCTCStype;					    //��������Ƶ����
  float RX_CTCSS_Freq;							//����ģ������Ƶ��
  CDC_MODE RX_CDCS_Mode;				    //������������Ƶ������
	u16 RX_CDCSS_Freq;                //������������Ƶ��
	
	
}Channel; 
typedef struct
{
	unsigned char sendDtmfT;
	unsigned char isButtonTone;
	unsigned char isScanInrupt;
	unsigned char isDWInrupt;
	unsigned char ButtonToneTime;
	unsigned char timePowOn;
	unsigned char isSendDtmf;
	unsigned char isPowerOn;
	unsigned char PPTpress;
	unsigned char LONG_UPDN_PRESS;
	unsigned char inDex2;
	unsigned char inDex1;
	unsigned char inDexMute;
	unsigned char scandwPPTPress;
	unsigned char mTxLength;
	unsigned char LCD_twinkle_Show;
	unsigned char LCD_Twinkle_tag;
	unsigned char isBK4815_Set;
	unsigned char isConnect;
	unsigned char changeDtmf;
	unsigned int Time_Space_POWLow_Show;
	unsigned char 	key_SQSetIndex;
	unsigned char isBattShow;
	unsigned char isCheckRssi;
	unsigned char CheckRssi;
}tParameter;



#define SetBit(VAR,Place)         ( (VAR) |= (u8)((u8)1<<(u8)(Place)) )
#define ClrBit(VAR,Place)         ( (VAR) &= (u8)((u8)((u8)1<<(u8)(Place))^(u8)255) )

#define SET_AT24C08_SDA_OUT      {P0MDL &= 0x3F; P0MDL |= 0x80;}
#define SET_AT24C08_SDA_IN       {P0MDL &= 0x3F; P0MDL |= 0x40;}

//#define MRECIVE_NONE         0
//#define MRECIVE_BK4815_INTERUPT   1


#define POW_IN                    ((xP4&0x04)==0? 0:1)
#define VCC_DET                   ((xP4&0x02)==0? 0:1)
#define VCC_DET1                  ((xP4&0x01)==0? 0:1)
#define VCC_BATT                  ((xP4&0x80)==0? 0:1)

//extern unsigned long 	mMessage;
extern tCbParam  		mCbParam;
extern tSqParam  		mSqParam;
extern tHmSetting  	mHmSetting;
extern tSysParam	    mSysParam;
extern tFlag  		mFlag;
//extern uchar     mRecive;
extern tSq			mSq;
//extern uchar   isSendDtmf;
//extern uchar     mRecive;
extern tDtmfRecive mDtmfRecive;
extern tKey mKey;
extern tParameter mParameter;


//extern u16  dtmfNum;
//extern u8  isPowerOn;
//extern u16 sendDtmfT;
extern tMenu mMenu;
extern Channel channel;






void SystemInit(void);
void delayms(u16 time);
void delayus(u8 i);
void UART0_Init();
void IO_Init();

/*-------------------------------------------------------------------------
*������Get_AD  ADֵ�ɼ�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
u8 Get_AD(u8 AD_IO);
/*-------------------------------------------------------------------------
*������Get_BATT_AD  ADֵ�ɼ�
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
u16 Get_BATT_AD();
#endif