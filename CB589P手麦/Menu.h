#define MENU_H
#ifdef MENU_H
/*--------------�˵������궨��------------------------*/
#define CHANNEL                             0
#define CHANNEL_SQ											   	1   //����SQ�ȼ�����
#define CHANNEL_LONGSQ											2   //����ASQ�ȼ�����
#define CHANNEL_VOL                         3
#define CHANNEL_PPTDN                       4
#define CHANNEL_DW                          5  //˫Ƶ�غ�
#define CHANNEL_SCAN 												6  //����ɨ��ģʽ
#define CHANNEL_AF                          7  //�л�AM/FM
#define CHANNEL_RFG                         8  //����RF GAIN�ȼ�����
#define CHANNEL_DOUBLEF                     9  //��������״̬
#define CHANNEL_LONGF_PB                    10  //����˵�ģʽ PB ������
#define CHANNEL_LONGF_F_F_FC 								11  //�˵��л���FC LCD��ɫ�л�
#define CHANNEL_LONGF_F_FB 								  12  //�˵��л���FL ����������
#define CHANNEL_FAF                         13  //���ù��ҽ�������
#define CHANNEL_FUPAFRFGSCAN                14  //����л�
#define CHANNEL_SQ_SET                      15  //SQ����
#define CHANNEL_FRECAL                      16  //Ƶƫ����


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


void CHANNEL_FUC();
/*-------------------------------------------------------------------------
*������InitMenu  �˵�������ʹ��
*��������  
*����ֵ����
*-------------------------------------------------------------------------*/
void InitMenu(void);
extern void MenuOperate(unsigned char key);

#endif