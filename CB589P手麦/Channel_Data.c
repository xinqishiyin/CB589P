#include "system.h"
#include "Channel_Data.h"
#include "memory.h"

extern Channel channel;

extern Param   param;
/*
u8 ChannelData[16];
u8 ParamData[16];


u16 CDC_Table(u16 encoding)
{
	switch(encoding)
	{
		case 0x0813:return 23;
		case 0x0815:return 25;
		case 0x0816:return 26;
		case 0x0819:return 31;
		case 0x081a:return 32;
		case 0x081e:return 36;
		case 0x0823:return 43;
		case 0x0827:return 47;
		case 0x0829:return 51;
		case 0x082b:return 53;
		case 0x082c:return 54;
		case 0x0835:return 65;
		case 0x0839:return 71;
		case 0x083a:return 72;
		case 0x083b:return 73;
		case 0x083c:return 74;
		case 0x084c:return 114;
		case 0x084d:return 115;
		case 0x084e:return 116;
		case 0x0852:return 122;
		case 0x0855:return 125;
		case 0x0859:return 131;
		case 0x085a:return 132;
		case 0x085c:return 134;
		case 0x0863:return 143;
		case 0x0865:return 145;
		case 0x086a:return 152;
		case 0x086d:return 155;
		case 0x086e:return 156;
		case 0x0872:return 162;
		case 0x0875:return 165;
		case 0x087a:return 172;
		case 0x087c:return 174;
		case 0x0885:return 205;
		case 0x088a:return 212;
		case 0x0893:return 223;
		case 0x0895:return 225;
		case 0x0896:return 226;
		case 0x08a3:return 243;
		case 0x08a4:return 244;
		case 0x08a5:return 245;
		case 0x08a6:return 246;
		case 0x08a9:return 251;
		case 0x08aa:return 252;
		case 0x08ad:return 255;
		case 0x08b1:return 261;
		case 0x08b3:return 263;
		case 0x08b5:return 265;
		case 0x08b6:return 266;
		case 0x08b9:return 271;
		case 0x08bc:return 274;
		case 0x08c6:return 306;
		case 0x08c9:return 311;
		case 0x08cd:return 315;
		case 0x08d5:return 325;
		case 0x08d9:return 331;
		case 0x08da:return 332;
		case 0x08e3:return 343;
		case 0x08e6:return 346;
		case 0x08e9:return 351;
		case 0x08ee:return 356;
		case 0x08f4:return 364;
		case 0x08f5:return 365;
		case 0x08f9:return 371;
		case 0x0909:return 411;
		case 0x090a:return 412;
		case 0x090b:return 413;
		case 0x0913:return 423;
		case 0x0919:return 431;
		case 0x091a:return 432;
		case 0x0925:return 445;
		case 0x0926:return 446;
		case 0x092a:return 452;
		case 0x092c:return 454;
		case 0x092d:return 455;
		case 0x0932:return 462;
		case 0x0934:return 464;
		case 0x0935:return 465;
		case 0x0936:return 466;
		case 0x0943:return 503;
		case 0x0946:return 506;
		case 0x094e:return 516;
		case 0x0953:return 523;
		case 0x0956:return 526;
		case 0x095a:return 532;
		case 0x0966:return 546;
		case 0x0975:return 565;
		case 0x0986:return 606;
		case 0x098a:return 612;
		case 0x0994:return 624;
		case 0x0997:return 627;
		case 0x0999:return 631;
		case 0x099a:return 632;
		case 0x09a5:return 526;
		case 0x09ac:return 654;
		case 0x09b2:return 662;
		case 0x09b4:return 664;
		case 0x09c3:return 703;
		case 0x09ca:return 712;
		case 0x09d3:return 723;
		case 0x09d9:return 731;
		case 0x09da:return 732;
		case 0x09dc:return 734;
		case 0x09e3:return 743;
		case 0x09ec:return 754;		
	}
}


u8 LoadChannel(u8 channel)
{
	u8 i;
	for(i=0;i<16;i++)
	{
		ChannelData=loadData(Channel_Start_Adress+(16*channnel)+i);
	}
	for(i=0;i<16;i++)
	{
		ParamData=loadData(Channel_Start_Adress+0x1000+(16*channnel)+i);
	}
	if(ChannelData[3]!=0xff)
	{
		channel.RX_Freq=ChannelData[3]/16*100+ChannelData[3]%16*10+ChannelData[2]/16+ChannelData[2]%16*0.1+ChannelData[1]/16*0.01+ChannelData[1]%16*0.001+ChannelData[0]/16*0.0001+ChannelData[0]%16*0.00001;
		channel.TX_Freq=ChannelData[7]/16*100+ChannelData[7]%16*10+ChannelData[6]/16+ChannelData[6]%16*0.1+ChannelData[5]/16*0.01+ChannelData[5]%16*0.001+ChannelData[4]/16*0.0001+ChannelData[4]%16*0.00001;
	}
	else return 0;
	if(ChannelData[8]==0xff)
	{
		channel.RX_CTCSS_Freq=0;
		channel.RX_CDCSS_Freq=0;
		channel.RXCTCStype=NONE;
	}
	else if(ChannelData[8]==0x28||ChannelData[8]==0x29)
	{
		channel.RXCTCStype=CDCS;
		channel.RX_CDCS_Mode=Normal;
		RX_CDCSS_Freq=CDC_Table((ChannelData[8]&0x0fff)*256+ChannelData[9]);
	}
	else if(ChannelData[8]==0xa8||ChannelData[8]==0xa9)
	{
		channel.RXCTCStype=CDCS;
		channel.RX_CDCS_Mode=Inverse;
		RX_CDCSS_Freq=CDC_Table((ChannelData[8]&0x0fff)*256+ChannelData[9]);
	}
	else
	{
		channel.RXCTCStype=CTCS;
		channel.RX_CTCSS_Freq=(ChannelData[8]*256+ChannelData[9])/10;
	}
	if(ChannelData[10]==0xff)
	{
		channel.TX_CTCSS_Freq=0;
		channel.TX_CDCSS_Freq=0;
		channel.TXCTCStype=NONE;
	}
	else if(ChannelData[10]==0x28||ChannelData[10]==0x29)
	{
		channel.TXCTCStype=CDCS;
		channel.TX_CDCS_Mode=Normal;
		TX_CDCSS_Freq=CDC_Table((ChannelData[10]&0x0fff)*256+ChannelData[11]);
	}
	else if(ChannelData[10]==0xa8||ChannelData[10]==0xa9)
	{
		channel.TXCTCStype=CDCS;
		channel.TX_CDCS_Mode=Inverse;
		TX_CDCSS_Freq=CDC_Table((ChannelData[10]&0x0fff)*256+ChannelData[11]);
	}
	else
	{
		channel.TXCTCStype=CTCS;
		channel.TX_CTCSS_Freq=(ChannelData[10]*256+ChannelData[11])/10;
	}
	if(ChannelData[12]&)
}
*/