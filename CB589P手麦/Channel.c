#include "system.h"
#include "Channel.h"
#include "memory.h"


extern u8 *p_EEPROM;
//extern Channel            channel;
//extern Param              param;
u8 chan[16];
u8 para[8];

bit BitIfTrue(char A,int i)
{
  if((A&1<<i)!=0)
  {
    return SET;
  }
  else return RESET;
}
/*---------------------------------------------
*����  SetChannel  �����ŵ�
*����  channelcount �ŵ����  dir ���仹�ǽ���  SET ���� RESET ����
*���� ��
*---------------------------------------------*/
void GetChannel(u8 channelcount)
{
  u8 val, i;
  float fre=0;
  for(i=0;i<16;i++)
  {
    chan[i]=*(p_EEPROM+channelcount*16);
  } 
  //����Ƶ��
  val=chan[3];
  fre=val/16*100;
  fre+=val%16*10;
  val=chan[2];
  fre+=val/16;
  fre+=val%16*0.1;
  val=chan[1];
  fre+=val/16*0.01;
  fre+=val%16*0.001;
  val=chan[0];
  fre+=val/16*0.0001;
  fre+=val%16*0.00001;
  channel.RX_Freq=fre;
    //����Ƶ��
  val=chan[7];
  fre=val/16*100;
  fre+=val%16*10;
  val=chan[6];
  fre+=val/16;
  fre+=val%16*0.1;
  val=chan[5];
  fre+=val/16*0.01;
  fre+=val%16*0.001;
  val=chan[4];
  fre+=val/16*0.0001;
  fre+=val%16*0.00001;
  channel.TX_Freq=fre;
  
  //��������Ƶ
  if(chan[9]==0xff)
  {
    channel.RXCTCStype=NONE;
  }
  else if(chan[9]/16==8)
  {
    channel.RXCTCStype=CDCS;
    channel.RX_CDCS_Mode=RESET;
    channel.RX_CDCSS_Freq=chan[9]%16*100+chan[8]/16*10+chan[8]%16;
  }
  else if(chan[9]/16==12)
  {
    channel.RXCTCStype=CDCS;
    channel.RX_CDCS_Mode=SET;
    channel.RX_CDCSS_Freq=chan[9]%16*100+chan[8]/16*10+chan[8]%16;
  }
  else 
  {
    channel.RXCTCStype=CTCS;  
    channel.RX_CTCSS_Freq=chan[9]/16*100+chan[9]%16*10+chan[8]/16+chan[8]%16*0.1;
  }
   //��������Ƶ
  if(chan[11]==0xff)
  {
    channel.RXCTCStype=NONE;
  }
  else if(chan[11]/16==8)
  {
    channel.RXCTCStype=CDCS;
    channel.RX_CDCS_Mode=RESET;
    channel.RX_CDCSS_Freq=chan[11]%16*100+chan[10]/16*10+chan[10]%16;
  }
  else if(chan[11]/16==12)
  {
    channel.RXCTCStype=CDCS;
    channel.RX_CDCS_Mode=SET;
    channel.RX_CDCSS_Freq=chan[11]%16*100+chan[10]/16*10+chan[10]%16;
  }
  else 
  {
    channel.RXCTCStype=CTCS;  
    channel.RX_CTCSS_Freq=chan[11]/16*100+chan[11]%16*10+chan[10]/16+chan[10]%16*0.1;
  }
  //ɨ�����
  if(BitIfTrue(chan[11],4)==SET)channel.ScanAdd=RESET;
  else channel.ScanAdd=SET;
  
  //��խ��
  if(BitIfTrue(chan[11],2)==SET)channel.band=narrow;
  else channel.band=wide;
  //æ��
  if(BitIfTrue(chan[11],0)==SET)channel.Bussy_Lock=RESET;
  else channel.Bussy_Lock=SET;
  //��Ƶ��  scramble
  
  //ѹ����  comprass
  //PPTID�� 
}

void GetParam()
{
  u8  i;
 
   for(i=0;i<8;i++)
  {
    para[i]=*(p_EEPROM+0x0200);
  }
  //���ʡ��
  if(BitIfTrue(para[0],1)==SET) param.Power_Save=SET;
  else param.Power_Save=RESET;
  
  //����
  //ɨ���ز�
  //PPID
  
  //����ȼ�
  param.Sqleve=para[1];
  
  //���䳬ʱ
  if(para[2]!=0)  
  {
    param.TX_Limit_Time=para[2];
    param.TX_Limit_Time_on=SET;
  }
  else param.TX_Limit_Time_on=RESET;
  
  //���صȼ�
  if(para[3]==0)
  {
    param.VoxOn=RESET;
  }
  else
  {
    param.VoxOn=SET;
    param.Voxleve=para[3];
  }
  
  //�����ӳ�
  
}
