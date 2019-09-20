#include "tim4.h"


UINT32 soc = 0;
UINT32 soc_count1 = 0;
int soc_temp = 0;
//timer4³õÊ¼»¯º¯Êý
void TIM4_Init(void)
{

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_Prescaler_128, 124);//4// 4 MS     =      1/(4MHz/128 ) *  (124+1)
  //TIM4_PrescalerConfig(TIM4_Prescaler_128,TIM4_PSCReloadMode_Immediate);
//	TIM4_UpdateDisableConfig( DISABLE);
	
  
 
  TIM4_ITConfig(TIM4_IT_Update, ENABLE);
  
	TIM4_ClearFlag(TIM4_FLAG_Update);
  TIM4_Cmd(ENABLE);
}

// 25 
@far @interrupt void TIM4_UPD_OVF_TRG_IRQHandler (void)
{
	soc_count1++;
	if(soc_count1 >= SOC_TIME)
	{
		soc_temp = CHG_CURRENT - DCH_CURRENT;
		soc += soc*( CHG_CURRENT - DCH_CURRENT)>>1;
		soc_count1 = 0;
	}
	
  /* Cleat Interrupt Pending bit */
  TIM4_ClearITPendingBit(TIM4_IT_Update);

}


