#ifndef __CHG_H
#define __CHG_H

#include "main.h"

#define CO_HIGH() GPIO_WriteBit(GPIOG,GPIO_Pin_4,SET)//CO
#define CO_LOW()  GPIO_WriteBit(GPIOG,GPIO_Pin_4,RESET)//CO

#define CHG_DAGE_HIGH()	GPIO_Init(GPIOG,GPIO_Pin_6,GPIO_Mode_Out_PP_High_Fast)//CHG_DAGE
#define CHG_DAGE_LOW() 	GPIO_Init(GPIOG,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Fast)//CHG_DAGE

//#define CHG_CURRENT() (((u32)tc_chg2)*100000)/(33*583) //
//#define CHG_CURRENT() ((UINT32)(adc_temp_voltage_add[16]>>3)*610 / 175)

UINT16 Chg_Current(void);

//#define CHG_CURRENT() ((u32)(adc_temp_voltage_add[16]>>3)*3 + (u32)(adc_temp_voltage_add[16]>>3)*6/10)
extern int charge_state_flag;
extern int charge_pre_state_flag;

extern UINT16 CURRENT_VOLTAGE_chgrepair;
extern UINT16 current_voltage_1;
extern UINT16 current_voltage_2;
extern UINT16 chgrepair_voltage;
extern UINT8 chg_dage_count1 ;
extern UINT8 minute_chgoc_count;
void Chg_Control(void);
void Chg_dage(void);
void PreChgControl(void);
#endif 