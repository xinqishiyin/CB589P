#ifndef _MENUEVENT_H_
#define _MENUEVENT_H_


/*
-------------------------------------------------------------��������------------------------------
*/
void CHANNEL_TABLE_FUC();       //������ұ�����
void CHANNEL_FUPAFRFGSCAN_DN_FUC();     //���ұ��
void CHANNEL_FUPAFRFGSCAN_UP_FUC();     //���ұ��
void CHANNEL_FAF_FUC();         //�������ѡ��
void CHANNEL_FAF_DN_FUC();      //���Ҽ�
void CHANNEL_FAF_UP_FUC();      //���Ҽ�
void CHANNEL_FAF_PPT_FUC();     //�����������
/*
------------------------------------------------------------���ҿ������------------------------------
*/
void CHANNEL_LONG_AF_FUC();     //�����ص�CE��

/*
-------------------------------------------------------------�ŵ�����------------------------------
*/
void CHANNEL_SCAN_UP_FUC();
void CHANNEL_SCAN_DN_FUC();
void CHANNEL_FUC();					    //ͨ��ģʽ
void CHANNEL_UP_FUC();				  //�ŵ���
void CHANNEL_LONGUP_FUC();      //ͨ��������
void CHANNEL_DN_FUC();  			  //�ŵ���
void CHANNEL_LONGDN_FUC();      //�ŵ�������

void CHANNEL_EMG_FUC();         //�����ŵ�
/*
-------------------------------------------------------------��������------------------------------
*/
void CHANNEL_VOLUP_FUC();       //������
void CHANNEL_VOLDN_FUC();				//������
/*
---------------------------------------------------------------SQ\ASQ����------------------------------
*/
void CHANNEL_SQ_FUC();					//����SQ�ȼ�����
void CHANNEL_LONGSQ_FUC();			//����ASQ�ȼ�����
void CHANNEL_SQ_VOLUP_FUC();		//SQ ASQ�ȼ���
void CHANNEL_SQ_VOLDN_FUC();  	//SQ ASQ�ȼ���

/*
----------------------------------------------------------------ɨ������------------------------------
*/
void CHANNEL_DW_FUC();          //˫Ƶ�غ�
void CHANNEL_SCAN_FUC();				//����ɨ��ģʽ
void CHANNEL_SCAN_SCAN_FUC();   //�˳�ɨ��ģʽ
/*
---------------------------------------------------------------�������------------------------------
*/
void CHANNEL_PPTDN_FUC();       //����
void CHANNEL_PPTUP_FUC();       //ֹͣ����
/*
---------------------------------------------------------------��������------------------------------
*/
void CHANNEL_AF_FUC();          //�л�AM/FM
void CHANNEL_LONG_AF_FUC();     //�����ص�CE��
/*
---------------------------------------------------------------RFG����------------------------------
*/

void CHANNEL_RFG_FUC();         //����RF GAIN�ȼ�����
void CHANNEL_RFG_UP_FUC();      //RF GAIN ��
void CHANNEL_RFG_DN_FUC();      //RF GAIN ��

/*-
-------------------------------------------------------------------����------------------------------
*/
void CHANNEL_DOUBLEF_FUC();     //��������״̬
void CHANNEL_LOCK_PRESSOTHER_FUC();     //������ʾ
/*
--------------------------------------------------------------����������------------------------------
*/
void CHANNEL_LONGF_PB_FUC();    //����˵�ģʽ PB ������
void CHANNEL_LONGF_PB_UP_FUC();	//PB ON��OF�л�
void CHANNEL_LONGF_PB_DN_FUC();	//PB ON��OF�л�
void CHANNEL_LONGF_F_F_FC_FUC();//�˵��л���FC LCD��ɫ�л�
void CHANNEL_LONGF_F_F_FC_UP_FUC();			//LCD��ɫ�л�
void CHANNEL_LONGF_F_F_FC_DN_FUC();			//LCD��ɫ�л�
void CHANNEL_LONGF_F_FB_FUC();	//�˵��л���FL ����������
void CHANNEL_LONGF_F_FB_UP_FUC();//�����ƿ�������
void CHANNEL_LONGF_F_FB_DN_FUC();//�����ƿ�������
/*
-----------------------------------------------------------------�˵�����------------------------------
*/
void CHANNEL_MENU_BACK_FUC();   //����ʱ��PPT����

void CHANNEL_DOUBLEMUTE_FUC();  //����



/*
----------------------------------------------------------------SQ��������------------------------------
*/
void CHANNEL_SQ_SET_FUC();      //����SQ����
void CHANNEL_SQ_SET_UP_FUC();   //SQ���ڼӼ�
void CHANNEL_SQ_SET_DN_FUC();   //SQ���ڼ���
void CHANNEL_SQ_SET_F_FUC();    //SQ�����л����ö���

/*
----------------------------------------------------------------Ƶƫ��������------------------------------
*/
//void CHANNEL_FRECAL_FUC();   //����Ƶƫ����
//void CHANNEL_FRECAL_UP_FUC();   //Ƶƫ���ڼӼ�
//void CHANNEL_FRECAL_DN_FUC();   //Ƶƫ���ڼ���

/*
------------------------------------------------------------------��������------------------------------
*/
void FACTORY_SETTING_FUC();     //��������

/*
------------------------------------------------------------------�ػ�------------------------------
*/
void CHANNEL_LONGPOWER_FUC();   //�ػ�

#endif