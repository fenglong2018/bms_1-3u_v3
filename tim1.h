#ifndef __TIM1_H
#define __TIM1_H
#include "main.h"
 
#define TIME_2S 250   //250 = 2/0.008 
#define DAGE_TIME_2S 250 
#define USART3_TIME_OUT 25//25=200ms
#define TIME_8S 3000 //1000 才是8s   3000 是24s了
#define SOC_TIME_504MS 63
#define DORMANCY_TIME_400MS 50 //400MS的休眠计时

//#define CHG_VOLTAGE_STOP_8S  1000
//#define CHG_VOLTAGE_STOP_8S  11250  //1000=8s, 11250 = 90S 的 时间
extern UINT16 CHG_VOLTAGE_STOP_8S;


// k = I/(V1 - V2)
#define CURRENT_VOLTAGE_CHGREPAIR_60S  2000//16S定时 标志出V1 和 V2用于动态求电流抬高电压的系数
#define bms_tongxin_dormancy_timeov 1000 

#define SOC_RW_60S 7500 // 60S 的时间


extern int chg_voltage_stop_time;

extern int dch_oc_time; 
extern int chg_oc_time;  
extern int dage_time_2s;  
extern int usart3_time_200ms; 
extern u32 time1_count; 
extern int dormancy_time_count; 
extern int Soc_Rw_time; 
extern int current_voltage_chgrepair_count;
extern UINT16 bms_tongxin_dormancy_time;
extern int chg_dage_count ;
extern int CHG_DAGE_TIME  ;


void Tim1_Init(void);
void TimingDelay_Decrement(void);
void delay_ms( u32 nTime);
void Delay(uint32_t ms);
#endif