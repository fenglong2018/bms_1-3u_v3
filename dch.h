#ifndef __DCH_H
#define __DCH_H

#include "main.h"

#define DO_HIGH() GPIO_WriteBit(GPIOG,GPIO_Pin_5,SET)//DO
#define DO_LOW()  GPIO_WriteBit(GPIOG,GPIO_Pin_5,RESET)//DO

#define DAGE_HIGH()	GPIO_WriteBit(GPIOB,GPIO_Pin_0,SET)//DAGE
#define DAGE_LOW()	GPIO_WriteBit(GPIOB,GPIO_Pin_0,RESET)//DAGE


//tc_dch2 = (UINT16)((UINT32)((UINT32)temp_4067_h[3]*(UINT32)ADC_RATIO)/(UINT32)1000) ;//11
//#define DCH_CURRENT() ((UINT32)(adc_temp_voltage_add[17]>>3)*1220 / 175)

//#define DCH_CURRENT() ((u32)(adc_temp_voltage_add[17]>>3)*10 + (u32)(adc_temp_voltage_add[17]>>3)/6)
UINT16 Dch_Current(void);

//#define DCH_CURRENT() ((UINT32)temp_4067_h[3]*7348 >> 10) //
//#define DCH_CURRENT() (((u32)tc_dch2)*100000)/(17*583) //
   
extern int do_low_count ;


//禁止放电检测
int Forbid_Dch_Check(void);
//放电过流情况控制
void Dch_Oc_Control(void);
//放电控制，只要平常条件满足，就要将放电管打开？？？？？？？？？？
void Dch_Control(void);

void Da_Ge(void);
#endif