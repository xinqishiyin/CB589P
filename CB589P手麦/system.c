
#include "system.h"

u32	        mMessage;                 //
tCbParam  	mCbParam;                 //需要发送到CB机的数据
tSqParam  	mSqParam;                 //SQ模式
tHmSetting  mHmSetting;               //手咪上面需要设置的数据

tSysParam	  mSysParam;                //
tFlag  		  mFlag;     
xdata uchar     mRecive;
xdata tDtmfRecive mDtmfRecive;
xdata tSq			mSq;
xdata uchar     mRecive;
xdata uchar   isSendDtmf;
xdata u16  dtmfNum;
xdata u8  isPowerOn;
xdata tKey mKey;
/*-------------------------------------------------------------------------
*函数：systemCLK_init  系统时钟选择  外部16M时钟
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
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
/*-------------------------------------------------------------------------
*函数：initTimer0  定时器初使化  50ms定时器
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void initTimer0(void) //定时50ms
{
    //初始化定时器
  	xTIMPRS |= 0x07;	//t0: MCLK/32 = 16M/256 = 62.5KHz = 62500Hz
	TMOD |= 0x01;	//t0: 16位定时器
	TH0 = (65535 - 3125) / 256;	
	TL0 = (65535 - 3125) % 256;	//50ms => 0.05S * 62500 = 3125	
   	TR0 = 1;
	ET0 = 1;
}
/*-------------------------------------------------------------------------
*函数：IO_Init  IO口初使化  
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void IO_Init()
{
	P0MDL=0xA6;                 //P0.0推挽输出  P0.1上拉输入   P0.2 推挽输出  P0.3 推挽输出（运行中会切换成输入）
	P0MDH=0xAA;                    //P0.4推挽输出  P0.5推挽输出  P0.6推挽输出  P0.7推挽输出
	P04=0;
	P00=1;
	
	P1MDL=0xAA;                 //P1.0-P1.3 推挽输出
	P1MDH=0xAA;                 //P1.4-P1.7 推挽输出
	P1=0x20;
	
	P15=0;
	//P16=0;
	P2MDL=0xA0;                 //P2.2  推挽输出
	P2MDH=0x80;                  //P2.4 上拉输入  P2.7 推挽输出
	P22=1;
	

	P27=0;	
	
	
	P3MDH=0xA2;                 //P3.4 推挽输出  P3.5 输入  P3.6 输出（运行中要切换输入） P3.7 推挽输出
  P34=1;	
	P37=1;
	P36=1;
	xP4MDL=0x00;
	xP4MDH=0x08;									//P4.0 输入 P4.1 输入 P4.2 输入  P4.4 输入 P4.5 输出
	
	
	xADCFG=0x10;                  //ADC对齐方式为  低4位ADRL 高2位ADRH 
}



/*-------------------------------------------------------------------------
*函数：delayms  延时1ms   
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void delayms(unsigned int i)
{
	unsigned int m,n;
	
	for(m=0;m<i;m++)
	{
		for(n=0;n<1188;n++);
	}
}
/*-------------------------------------------------------------------------
*函数：delayus  延时1.5us 
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void delayus(u8 i)
{
    while(--i);
}
/*-------------------------------------------------------------------------
*函数：initBEEP  PWM  BEEP音初使化 
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void initBEEP(void)
{
	/*
	//xSYSCFG |= 0x20;
	xPWMCN=0x10;            //使能PWM0
	xPWMPSC|=0x13;				  //PWM预分频比20
	xIOMUX0 |= 0x02;         //P23为PWM0输出 
	xPWMP=200;						  //周期						P=(xPWMP)*(定时器分频+1)*Tclk= 200*20*(1/4MHz)=1ms  =1kHz
	xPWM0D=100;						  //PWM0占空比      duty = xPWM0D*(定时器分频+1)*Tclk = 100*20/4 =500us  
	P2MDL &= 0x3f;
  P2MDL |=0x80;            //P23作为输出
	*/
	/*
	 xPWMCN = 0X10;          //使能PWM0
    xPWMPSC = 0X07;         //PWM预分频比 
    xPWMP = 0X80;           //周期寄存器  P=(xPWMP)*(定时器分频+1)*Tclk= 128*8*(1/4MHz)=256us 
    xPWM0D = 0X20;          //PWM0占空比  duty = xPWM0D*(定时器分频+1)*Tclk = 32*8/4 =64us
    xIOMUX0 = 0X02;         //P23为PWM0输出
    P2MDL |=0X80;            //P23作为输出
	P23=1;
	*/
	xSYSCFG |= 0x20;
	xPWMPSC|=0x70;				  //PWM预分频比20
	xIOMUX0 |= 0x02;//En P23 as Pwm0
	xPWMP = 0X86;	//周期
	xPWM0D = 0X43;	//占空比，modda
	P2MDL &= 0x3f;
	P2MDL |= 0x80;
}
/*-------------------------------------------------------------------------
*函数：UART0_Init  串口初使化
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
void UART0_Init()
{
	xIOMUX0 |= 0x01;        //P21/P20复用做UART0端口
	SCON0=0x50;							//工作模式1,8位异步，波特率可调 允许接收与发送
	BRCON0  = 0xE8;         //波特率计算公式为1/16（2smod=1）,波特率发生器开启，高精度波特率控制使能
	
	BRTIM0  = 0x98;          //
  
	 
}
void UART1_Init()
{
	xIOMUX2 |= 0x01;        //P24/P23复用做UART1端口
  SCON1 |= 0x50;         //工作模式1,8位异步，波特率可调 使能接收
 
  xBRCON1  = 0XE8;//0x68;         //波特率计算公式为1/16（2smod=1），波特率发生器开启，高精度波特率控制使能   
  xBRTIM1  = 0x98;
  EIE2 |=0X10;                    //使能ES1中断
}
void INT1_Init(void)
{
    EA = 0;
    xIOMUX2 &= 0xDF;                  //INT脚选P01
    IT1 = 1; 		                      //负边沿（下降沿）触发中断
    EINTCS0 = 0xA0;	                  //bit5=1，INT1中断使能信号 ,异步中断模式
    IE1 = 0;                          //清INT1下降沿中断标志
    EINTCS0 &= 0xfc;                  //清除标志 ，双缓冲机制的中断，2次清除标志
    EINTCS0 &= 0xfc;                  //清除标志
    
}
void SystemInit()
{
	systemCLK_init();
	IO_Init();	
	initTimer0();
	UART0_Init();
	//UART1_Init();
	initBEEP();	
	INT1_Init();
	//UART1_Init();
}

/*-------------------------------------------------------------------------
*函数：Get_AD  AD值采集
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
u8 Get_AD(u8 AD_IO)
{		
	u16 i,j;
	u32 temp;
	u16 val[10];
	
	switch(AD_IO)	
	{		
		case Key1:
			xP3AEN=0x04;  //P3.2模拟通道开启
			ADCON=0x28;
			break;
		case Key2:
			xP3AEN=0x08;  //P3.3模拟通道开启
			ADCON=0x38;
			break;
		default:
			break;
	}
	
	delayus(10);
	for(i=0;i<10;i++)
	{
		ADGO=1;
		while(ADGO==1);
		val[i]=((ADRH<<8)|ADRL);
	}
	for(i=0;i<10;i++)   
  {
    for(j=0;j<10-i;j++)
    {
      if(val[j]>val[j+1])
      { 
        temp = val[j];
        val[j] = val[j+1];
        val[j+1] = temp;                 
      }
    }
  }
  temp=0;
	for(i=4;i<6;i++)	
	{
		temp+=val[i];
	}
	temp=temp/2;
	if((0x0070<temp)&&(temp<0x00ef)) return 0x03;
	else if((0x0100<temp)&&(temp<0x015f)) return 0x06;
	else if((0x01f0<temp)&&(temp<0x021f))return 0x0a;
	else if((0x0230<temp)&&(temp<0x026f)) return 0x0b;
	else if((0x027f<temp)&&(temp<0x02df)) return 0x0d;
	else if((0x02ef<temp)&&(temp<0x033f)) return 0x0f;
	else return 0xff;
}
/*-------------------------------------------------------------------------
*函数：Get_BATT_AD  AD值采集
*参数：无  
*返回值：无
*-------------------------------------------------------------------------*/
u16 Get_BATT_AD()
{
	u16 i,j,reval;
	u32 temp;
	u16 val[5];
	xP3AEN=0x01;  //P3.0模拟通道开启
	ADCON=0x08;	
	delayus(10);
	for(i=0;i<5;i++)
	{
		ADGO=1;
		while(ADGO==1);
		val[i]=((ADRH<<8)|ADRL);
	}
	for(i=0;i<5;i++)
  {
    for(j=0;j<5-i;j++)
    {
      if(val[j]>val[j+1])
      {
        temp = val[j];
        val[j] = val[j+1];
        val[j+1] = temp;
      }
    }
  }
	reval=val[2];
	return reval;
}