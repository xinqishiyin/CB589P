#include "system.h"

xdata uchar     mRecive;
xdata uint			mMessage;
xdata tCbParam  	mCbParam;
xdata tSqParam  	mSqParam;
xdata tFlag  		mFlag;
xdata tTimer0		mTimer0;
xdata tSq			mSq;
xdata tDtmfRecive mDtmfRecive;
/*���ڽ���*/
uchar mUartCmd = 0;
tReceivePackage mReceivePackage;

/*SQ���*/
xdata uchar mDebugSqLevel;
xdata uchar mOpenSqDbLevel;
xdata uchar mLastOpenSqDbLevel;

/*����VCO���*/		
xdata uchar mRssi;   
xdata uchar mLastRssi;	//�ϴδ����RSSI
xdata ulong mXn31202Ch1_Tx;
xdata ulong mXn31202Ch1_Rx;
xdata ulong mReferenceFreq = 2500;	//���ڼ���R�Ĵ����Ϳ��ƼĴ���
xdata ulong mChannelBaseFreq = 26965000;	//��1�ŵ���Ƶ��
xdata ulong mCurrentFreq = 0;		//��ǰ�ŵ���Ƶ�ʣ�ͨ���ŵ�1�Ͳ���Ƶ�ʼ���
xdata uint  mUkReferenceFreq[40] =
{
	1349,1279,1278,1277,1316,1303,1271,1286,1285,1266,
	1336,1312,1262,1300,1277,1261,1286,1303,1302,1308,
	1271,1337,1257,1257,1257,1257,1257,1299,1266,1266,
	1354,1313,1287,1272,1286,1271,1308,1262,1262,1288
};
xdata uint mAsqVoltage;  //5V
xdata uint mRssiVoltage; //5V
xdata uint mAgcaVoltage; //5V







void systemCLK_init()
{
WPKEY = 0x37;
	CLKEN &= 0xFB;                 	// P90��P91���óɾ��������
	CLKEN |= 0x08;	              	// ��CRYH
	while((MCKSEL&0x40) != 0x40);  	// �鿴CRYH�ȶ���־λ�Ƿ�����
	WPKEY  = 0x37;
   	MCKSEL = 0x01;                	// �л���CRYH
	MCKDIV = 0x03;	//����Ƶ
}

void initTimer0(void) //��ʱ10ms
{
    //��ʼ����ʱ��
  	xTIMPRS |= 0x07;	//t0: MCLK/32 = 10.25M/256  = 40000Hz
	TMOD |= 0x01;	//t0: 16λ��ʱ��
	TH0 = (65535 - 400) / 256;	
	TL0 = (65535 - 400) % 256;	//10ms => 0.01S * 62500 = 400	
   	TR0 = 1;
	ET0 = 1;
}


void IO_Init()
{
	P0MDL=0xA1;                 //P0.0��������  P0.1��������   P0.2 �������  P0.3 ��������������л��л������룩
	P0MDH=0x6A;                    //P0.4�������  P0.5�������  P0.6�������  P0.7��������
	P04=1;
	
	
	P1MDL=0xAA;                 //P1.0-P1.3 �������
	P1MDH=0x2A;                 //P1.4-P1.6 �������  P1.7����
	P1=0x20;
	P13=0;
	P14=0;
	POWER_ON_EN = 1;
	P2MDL=0xA0;                 //P2.2  �������
	P2MDH=0x82;                  //P2.4 ���  P2.7 ���
	P22=1;
	

	P27=0;	
	
	P3MDL=0x51;
	P3MDH=0xA8;                 //P3.4 ������  P3.5 ���  P3.6 �����������Ҫ�л����룩 P3.7 �������
  xP3AEN |= 0x0d;	//P3.0  P3.2  P3.3
	P37=1;
	P36=1;
	
	xP4MDL=0xAA;
	xP4MDH=0xAA;									//P4.0-P4.7  ���
	
	xP9MDL &= 0xf0;
	xP9MDL |= 0x05;	 //P9.0  P9.1 �������� 
	xP9AEN |= 0x03;	//P9.0  P9.1

	xADCFG=0x10;                  //ADC���뷽ʽΪ  ��4λADRL ��2λADRH 
}
void INT0_Init(void)
{
    EA = 0;
    xIOMUX2 &= 0xef;                  //INT��ѡͨ��P16��P00��
    IT0 = 1; 		                      //�����أ��½��أ������ж�
    EINTCS0 = 0X50;;	                //bit4=1��INT1�ж�ʹ���ź�,�첽�ж�ģʽ
    IE0 = 0;                          //��INT1�½����жϱ�־
    EINTCS0 &= 0xfc;                  //�����־ ��˫������Ƶ��жϣ�2�������־
    EINTCS0 &= 0xfc;                  //�����־
   
}
void initAd(void)
{
	P3MDL &= 0x0c;
	P3MDL |= 0x51;	 //P3.0  P3.2  P3.3 �������� 
	xP3AEN |= 0x0d;	//P3.0  P3.2  P3.3

	xP9MDL &= 0xf0;
	xP9MDL |= 0x05;	 //P9.0  P9.1 �������� 
	xP9AEN |= 0x03;	//P9.0  P9.1

	xADCFG = 0x10;	 //�Ҷ��룬����ʱ��MCLK/2

	//ADCON = ADC_RSSI;//open adc0 channel,adcen open		
}

void initPWM(void)
{
	xSYSCFG |= 0x30;
	xIOMUX0 |= 0x02;//En P23 as Pwm0   MODDA
	xIOMUX2 |= 0x02;//En P27 as Pwm1   FREQ
	xPWMP = 200;	//����
	xPWM0D = 100;	//ռ�ձȣ�modda
	xPWM1D = 200; 	//ռ�ձȣ�ϵͳʱ��У׼
  P2MDL &= 0x3f;
	P2MDL |= 0x80;
	//PWM_MODDA_CLOSE;
	PWM_FRQ_CLOSE;	
	PWM_RFG_OPEN;
}

void delayms(unsigned int i)
{
	unsigned int m,n;
	
	for(m=0;m<i;m++)
	{
		for(n=0;n<785;n++);
	}
}
void delayus(u8 i)
{
    while(--i);
}


void UART0_Init()
{
  xIOMUX0 |= 0x01; //UartEn  P2.0,P2.1��UART
	SCON0=0x50;		 //8λģʽ,�������
 	BRCON0 = 0xe8;   //SMOD=1,��������뷢��
	BRTIM0 = 0xBD; 	 //�����ʷ�������ʼֵ��9600bps	(10.25MHz) err0.4%
	IE |= 0x10;	     //en  ES0
	

	mCbParam.UartTxBuf[0] = MAGIC;
}
void UART1_Init()
{
	xIOMUX2 |= 0x01;        //P24/P23������UART1�˿�
  SCON1 |= 0x50;         //����ģʽ1,8λ�첽�������ʿɵ� ʹ�ܽ���
 
  xBRCON1  = 0XE8;//0x68;         //�����ʼ��㹫ʽΪ1/16��2smod=1���������ʷ������������߾��Ȳ����ʿ���ʹ��   
  xBRTIM1  = 0x98;
  EIE2 |=0X10;                    //ʹ��ES1�ж�
}
void SystemInit()
{
	systemCLK_init(); 
	IO_Init();	
	initPWM();
	initAd();
	initTimer0();
	
	UART0_Init();
	INT0_Init();
	EA=1;
	//initBEEP();
	
	//UART1_Init();
}