#include "myit.h"


void Myit_Init(void)
{
	GPIO_Init(GPIOD,GPIO_Pin_2 | GPIO_Pin_3,GPIO_Mode_In_FL_IT);       //配置外部中断引脚             
	EXTI_SetPinSensitivity(EXTI_Pin_2,EXTI_Trigger_Falling);       //EXTI_Trigger_Falling_Low外部中断下降沿低电平触发EXTI_Trigger_Falling
	EXTI_SetPinSensitivity(EXTI_Pin_3,EXTI_Trigger_Falling);       //EXTI_Trigger_Falling_Low外部中断下降沿低电平触发EXTI_Trigger_Falling
	ITC_SetSoftwarePriority(EXTI2_IRQn,ITC_PriorityLevel_3);           //中断优先级
	ITC_SetSoftwarePriority(EXTI3_IRQn,ITC_PriorityLevel_3);           //中断优先级
	//enableInterrupts();
	
}

#if 1
//11	IT_INT1
@far @interrupt IT_INT1 (void)
{
	
	
	//soc_percent = 11;
	flag.dormancy_on = 0; //中断唤醒 马上关闭休眠总开关
	EXTI_ClearITPendingBit(EXTI_IT_Pin3);
}


//10
@far @interrupt void IT_INT2 (void)
{
	
	
	//soc_percent = 21;
	//CO_LOW();
	//DO_LOW();//过流 中断
	EXTI_ClearITPendingBit(EXTI_IT_Pin2);
}
#endif