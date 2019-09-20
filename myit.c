#include "myit.h"


void Myit_Init(void)
{
	GPIO_Init(GPIOD,GPIO_Pin_2 | GPIO_Pin_3,GPIO_Mode_In_FL_IT);       //�����ⲿ�ж�����             
	EXTI_SetPinSensitivity(EXTI_Pin_2,EXTI_Trigger_Falling);       //EXTI_Trigger_Falling_Low�ⲿ�ж��½��ص͵�ƽ����EXTI_Trigger_Falling
	EXTI_SetPinSensitivity(EXTI_Pin_3,EXTI_Trigger_Falling);       //EXTI_Trigger_Falling_Low�ⲿ�ж��½��ص͵�ƽ����EXTI_Trigger_Falling
	ITC_SetSoftwarePriority(EXTI2_IRQn,ITC_PriorityLevel_3);           //�ж����ȼ�
	ITC_SetSoftwarePriority(EXTI3_IRQn,ITC_PriorityLevel_3);           //�ж����ȼ�
	//enableInterrupts();
	
}

#if 1
//11	IT_INT1
@far @interrupt IT_INT1 (void)
{
	
	
	//soc_percent = 11;
	flag.dormancy_on = 0; //�жϻ��� ���Ϲر������ܿ���
	EXTI_ClearITPendingBit(EXTI_IT_Pin3);
}


//10
@far @interrupt void IT_INT2 (void)
{
	
	
	//soc_percent = 21;
	//CO_LOW();
	//DO_LOW();//���� �ж�
	EXTI_ClearITPendingBit(EXTI_IT_Pin2);
}
#endif