#include "system.h"

xdata uchar     mRecive;
xdata uint			mMessage;
xdata tCbParam  	mCbParam;
xdata tSqParam  	mSqParam;
xdata tFlag  		mFlag;
xdata tTimer0		mTimer0;
xdata tSq			mSq;
xdata tDtmfRecive mDtmfRecive;
/*串口接收*/
uchar mUartCmd = 0;
tReceivePackage mReceivePackage;

/*SQ相关*/
xdata uchar mDebugSqLevel;
xdata uchar mOpenSqDbLevel;
xdata uchar mLastOpenSqDbLevel;

/*以下VCO相关*/		
xdata uchar mRssi;   
xdata uchar mLastRssi;	//上次传输的RSSI
xdata ulong mXn31202Ch1_Tx;
xdata ulong mXn31202Ch1_Rx;
xdata ulong mReferenceFreq = 2500;	//用于计算R寄存器和控制寄存器
xdata ulong mChannelBaseFreq = 26965000;	//第1信道的频率
xdata ulong mCurrentFreq = 0;		//当前信道的频率，通过信道1和步进频率计算
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
	CLKEN &= 0xFB;                 	// P90，P91复用成晶振输入脚
	CLKEN |= 0x08;	              	// 打开CRYH
	while((MCKSEL&0x40) != 0x40);  	// 查看CRYH稳定标志位是否置起
	WPKEY  = 0x37;
   	MCKSEL = 0x01;                	// 切换到CRYH
	MCKDIV = 0x03;	//不分频
}

void initTimer0(void) //定时10ms
{
    //初始化定时器
  	xTIMPRS |= 0x07;	//t0: MCLK/32 = 10.25M/256  = 40000Hz
	TMOD |= 0x01;	//t0: 16位定时器
	TH0 = (65535 - 400) / 256;	
	TL0 = (65535 - 400) % 256;	//10ms => 0.01S * 62500 = 400	
   	TR0 = 1;
	ET0 = 1;
}


void IO_Init()
{
	P0MDL=0xA1;                 //P0.0上拉输入  P0.1上拉输入   P0.2 推挽输出  P0.3 推挽输出（运行中会切换成输入）
	P0MDH=0x6A;                    //P0.4推挽输出  P0.5推挽输出  P0.6推挽输出  P0.7上拉输入
	P04=1;
	
	
	P1MDL=0xAA;                 //P1.0-P1.3 推挽输出
	P1MDH=0x2A;                 //P1.4-P1.6 推挽输出  P1.7输入
	P1=0x20;
	P13=0;
	P14=0;
	POWER_ON_EN = 1;
	P2MDL=0xA0;                 //P2.2  推挽输出
	P2MDH=0x82;                  //P2.4 输出  P2.7 输出
	P22=1;
	

	P27=0;	
	
	P3MDL=0x51;
	P3MDH=0xA8;                 //P3.4 推输入  P3.5 输出  P3.6 输出（运行中要切换输入） P3.7 推挽输出
  xP3AEN |= 0x0d;	//P3.0  P3.2  P3.3
	P37=1;
	P36=1;
	
	xP4MDL=0xAA;
	xP4MDH=0xAA;									//P4.0-P4.7  输出
	
	xP9MDL &= 0xf0;
	xP9MDL |= 0x05;	 //P9.0  P9.1 上拉输入 
	xP9AEN |= 0x03;	//P9.0  P9.1

	xADCFG=0x10;                  //ADC对齐方式为  低4位ADRL 高2位ADRH 
}
void INT0_Init(void)
{
    EA = 0;
    xIOMUX2 &= 0xef;                  //INT脚选通（P16或P00）
    IT0 = 1; 		                      //负边沿（下降沿）触发中断
    EINTCS0 = 0X50;;	                //bit4=1，INT1中断使能信号,异步中断模式
    IE0 = 0;                          //清INT1下降沿中断标志
    EINTCS0 &= 0xfc;                  //清除标志 ，双缓冲机制的中断，2次清除标志
    EINTCS0 &= 0xfc;                  //清除标志
   
}
void initAd(void)
{
	P3MDL &= 0x0c;
	P3MDL |= 0x51;	 //P3.0  P3.2  P3.3 上拉输入 
	xP3AEN |= 0x0d;	//P3.0  P3.2  P3.3

	xP9MDL &= 0xf0;
	xP9MDL |= 0x05;	 //P9.0  P9.1 上拉输入 
	xP9AEN |= 0x03;	//P9.0  P9.1

	xADCFG = 0x10;	 //右对齐，采样时钟MCLK/2

	//ADCON = ADC_RSSI;//open adc0 channel,adcen open		
}

void initPWM(void)
{
	xSYSCFG |= 0x30;
	xIOMUX0 |= 0x02;//En P23 as Pwm0   MODDA
	xIOMUX2 |= 0x02;//En P27 as Pwm1   FREQ
	xPWMP = 200;	//周期
	xPWM0D = 100;	//占空比，modda
	xPWM1D = 200; 	//占空比，系统时钟校准
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
  xIOMUX0 |= 0x01; //UartEn  P2.0,P2.1作UART
	SCON0=0x50;		 //8位模式,允许接收
 	BRCON0 = 0xe8;   //SMOD=1,允许接收与发送
	BRTIM0 = 0xBD; 	 //波特率发生器初始值，9600bps	(10.25MHz) err0.4%
	IE |= 0x10;	     //en  ES0
	

	mCbParam.UartTxBuf[0] = MAGIC;
}
void UART1_Init()
{
	xIOMUX2 |= 0x01;        //P24/P23复用做UART1端口
  SCON1 |= 0x50;         //工作模式1,8位异步，波特率可调 使能接收
 
  xBRCON1  = 0XE8;//0x68;         //波特率计算公式为1/16（2smod=1），波特率发生器开启，高精度波特率控制使能   
  xBRTIM1  = 0x98;
  EIE2 |=0X10;                    //使能ES1中断
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