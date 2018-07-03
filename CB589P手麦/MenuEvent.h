#ifndef _MENUEVENT_H_
#define _MENUEVENT_H_


/*
-------------------------------------------------------------国家设置------------------------------
*/
void CHANNEL_TABLE_FUC();       //进入国家表设置
void CHANNEL_FUPAFRFGSCAN_DN_FUC();     //国家表加
void CHANNEL_FUPAFRFGSCAN_UP_FUC();     //国家表减
void CHANNEL_FAF_FUC();         //进入国家选择
void CHANNEL_FAF_DN_FUC();      //国家减
void CHANNEL_FAF_UP_FUC();      //国家加
void CHANNEL_FAF_PPT_FUC();     //保存国家设置
/*
------------------------------------------------------------国家快捷设置------------------------------
*/
void CHANNEL_LONG_AF_FUC();     //长按回到CE段

/*
-------------------------------------------------------------信道设置------------------------------
*/
void CHANNEL_SCAN_UP_FUC();
void CHANNEL_SCAN_DN_FUC();
void CHANNEL_FUC();					    //通道模式
void CHANNEL_UP_FUC();				  //信道加
void CHANNEL_LONGUP_FUC();      //通道连续加
void CHANNEL_DN_FUC();  			  //信道减
void CHANNEL_LONGDN_FUC();      //信道连续减

void CHANNEL_EMG_FUC();         //紧急信道
/*
-------------------------------------------------------------音量设置------------------------------
*/
void CHANNEL_VOLUP_FUC();       //音量加
void CHANNEL_VOLDN_FUC();				//音量减
/*
---------------------------------------------------------------SQ\ASQ设置------------------------------
*/
void CHANNEL_SQ_FUC();					//进入SQ等级设置
void CHANNEL_LONGSQ_FUC();			//进入ASQ等级设置
void CHANNEL_SQ_VOLUP_FUC();		//SQ ASQ等级加
void CHANNEL_SQ_VOLDN_FUC();  	//SQ ASQ等级减

/*
----------------------------------------------------------------扫描设置------------------------------
*/
void CHANNEL_DW_FUC();          //双频守候
void CHANNEL_SCAN_FUC();				//进入扫描模式
void CHANNEL_SCAN_SCAN_FUC();   //退出扫描模式
/*
---------------------------------------------------------------发射接收------------------------------
*/
void CHANNEL_PPTDN_FUC();       //发射
void CHANNEL_PPTUP_FUC();       //停止发射
/*
---------------------------------------------------------------调制设置------------------------------
*/
void CHANNEL_AF_FUC();          //切换AM/FM
void CHANNEL_LONG_AF_FUC();     //长按回到CE段
/*
---------------------------------------------------------------RFG设置------------------------------
*/

void CHANNEL_RFG_FUC();         //进入RF GAIN等级设置
void CHANNEL_RFG_UP_FUC();      //RF GAIN 加
void CHANNEL_RFG_DN_FUC();      //RF GAIN 减

/*-
-------------------------------------------------------------------锁键------------------------------
*/
void CHANNEL_DOUBLEF_FUC();     //进入锁键状态
void CHANNEL_LOCK_PRESSOTHER_FUC();     //锁屏提示
/*
--------------------------------------------------------------手麦功能设置------------------------------
*/
void CHANNEL_LONGF_PB_FUC();    //进入菜单模式 PB 按键音
void CHANNEL_LONGF_PB_UP_FUC();	//PB ON和OF切换
void CHANNEL_LONGF_PB_DN_FUC();	//PB ON和OF切换
void CHANNEL_LONGF_F_F_FC_FUC();//菜单切换到FC LCD颜色切换
void CHANNEL_LONGF_F_F_FC_UP_FUC();			//LCD颜色切换
void CHANNEL_LONGF_F_F_FC_DN_FUC();			//LCD颜色切换
void CHANNEL_LONGF_F_FB_FUC();	//菜单切换到FL 按键灯设置
void CHANNEL_LONGF_F_FB_UP_FUC();//按键灯开关设置
void CHANNEL_LONGF_F_FB_DN_FUC();//按键灯开关设置
/*
-----------------------------------------------------------------菜单返回------------------------------
*/
void CHANNEL_MENU_BACK_FUC();   //设置时按PPT返回

void CHANNEL_DOUBLEMUTE_FUC();  //静音



/*
----------------------------------------------------------------SQ调节设置------------------------------
*/
void CHANNEL_SQ_SET_FUC();      //进入SQ调节
void CHANNEL_SQ_SET_UP_FUC();   //SQ调节加键
void CHANNEL_SQ_SET_DN_FUC();   //SQ调节减键
void CHANNEL_SQ_SET_F_FUC();    //SQ调节切换设置对象

/*
----------------------------------------------------------------频偏调节设置------------------------------
*/
//void CHANNEL_FRECAL_FUC();   //进入频偏调节
//void CHANNEL_FRECAL_UP_FUC();   //频偏调节加键
//void CHANNEL_FRECAL_DN_FUC();   //频偏调节减键

/*
------------------------------------------------------------------出厂设置------------------------------
*/
void FACTORY_SETTING_FUC();     //出厂设置

/*
------------------------------------------------------------------关机------------------------------
*/
void CHANNEL_LONGPOWER_FUC();   //关机

#endif