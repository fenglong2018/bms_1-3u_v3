#include "tim1.h"
u32 time1_count = 0;
static  u32 TimingDelay; 
int chg_voltage_stop_time = 0;
int Soc_Rw_time = 0;

int chg_stop_90s = 0;


UINT16 CHG_VOLTAGE_STOP_8S = 250;//停止充电 倒计时 2s

void Tim1_Init(void)
{
	// 4Mhz的主时钟，16分频后tim1时钟为0.25Mhz，从0到999后刚好是4ms，之后自动重装0，开始新的计数计时。这里设置计时为8ms
	  TIM1_TimeBaseInit(16,TIM1_CounterMode_Up,1880,0);
		TIM1_ARRPreloadConfig(ENABLE);
		TIM1_ITConfig(TIM1_IT_Update , ENABLE);
		ITC_SetSoftwarePriority(TIM1_UPD_OVF_TRG_IRQn,ITC_PriorityLevel_2);   
		TIM1_Cmd(ENABLE);
}

// 4MHZ下Delay(100) -- 1000us=1ms
//Delay(10) = 100us
void Delay(uint32_t ms)
{
	while(ms>0)
	{		
		ms--;
	}
}



void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
     { 
	TimingDelay--;
      }
}

void delay_ms( u32 nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

int dch_oc_time = 0;
int chg_oc_time = 0;
int dage_time_2s = 0;
int usart3_time_200ms = 0;
int soc_time = 0;
int dormancy_time_count = 0;

int current_voltage_chgrepair_count = 0;
UINT16 bms_tongxin_dormancy_time = 0;
int chg_dage_count = 0;
int CHG_DAGE_TIME = 250;

int chg_current_ = 0;
int dch_current_ = 0;
UINT16 minute_chgoc_count_time = 0;
u32 pre_30fenzhong_count = 0;
u32 heat_10fenzhong_count = 0;
//#pragma vector= 23
@far @interrupt void tim1_ms_irp (void)
{
  time1_count++;
	if((flag.dormancy_poweron_off==0)&&(time1_count>625))
	{
		flag.dormancy_poweron_off = 1;//一次性标识，开机5秒后才 允许 休眠
	}
	if((flag.poweron_soc_first_g==0)&& (flag.poweron_soc_first==0)&&(time1_count>62))
	{
		flag.poweron_soc_first = 1;//一次性标识		
	}
	//TimingDelay_Decrement();
  //放电过流延时计数
  if(flag.dch_oc_protect_time == 1)	
  {
		dch_oc_time += 1;
	}
  
  //充电过流延时计数
  if( charge_state_flag ==1 || charge_pre_state_flag == 1)	
  { 
		chg_oc_time += 1; 
	}
  
  //打嗝事件计数
  dage_time_2s++;
  chg_dage_count++;
	
		
	if(flag.chg_voltage_stop_8s_on  == 1)
	{
		chg_voltage_stop_time++;
		if(chg_voltage_stop_time >= CHG_VOLTAGE_STOP_8S)
		{
				flag.chg_voltage_stop_8s_on = 0;
				chg_voltage_stop_time = 0;
		}
	}
	
	soc_time++;
	if(soc_time >= SOC_TIME_504MS)
	{	
		soc_time = 0;
		flag.soc_come_g = 1;
	}
	
	
	if(flag.current_voltage_chgrepair == 1)
	{
		current_voltage_chgrepair_count++;
		if(current_voltage_chgrepair_count >= CURRENT_VOLTAGE_CHGREPAIR_60S)
		{
			current_voltage_chgrepair_count = 0;
			flag.current_voltage_chgrepair = 0;
			flag.current_voltage_60s_ok = 1;
		}
	}
	
	
	
	
 if(flag.usart3_receive_address == 1)
 {
	usart3_time_200ms ++;
	if(usart3_time_200ms >= USART3_TIME_OUT)
	{	
		flag.usart3_receive_address = 0;
		usart3_time_200ms = 0;
		receive_count = 0;
	}
 }
 else
 {
	usart3_time_200ms = 0;
 }
	
	
	if(bms_self_flag.bms_tongxin_dormancy_n == 1)
	{
		bms_tongxin_dormancy_time++;
		if(bms_tongxin_dormancy_time >= bms_tongxin_dormancy_timeov)
		{
			//bms_self_flag.bms_tongxin_dormancy_n = 0;
			bms_self_flag.U32 = 0;
			bms_tongxin_dormancy_time = 0;
			CHG_VOLTAGE_STOP_8S = 250;  //2s
		}
	}
			
	
	
	
	/////////////////////////////////////	////
	Soc_Rw_time++;//Soc_Rw()函数调用计时
	if(Soc_Rw_time > 125)
	{
		Soc_Rw_time = 0;
		if( flag.alow_time_countsoc == 1)
		{
			if(flag.dormancy_on == 1)
			{
				surplus_soc_ = surplus_soc_ - 2;
				if(surplus_soc_ < 0)
				{
					surplus_soc_ = 0;
				}
				else if((surplus_soc_ > soc_static) || (system_state.cells_staturate_state == 1))
				{
					surplus_soc_ = soc_static;
				}
			}
			else
			{
				chg_current_ = CHG_CURRENT;
				dch_current_ = DCH_CURRENT;
				if(dch_current_ <= 0)
				{
					dch_current_ = 10;
				}
				surplus_soc_ += (chg_current_ - dch_current_);
				if(surplus_soc_ < 0)
				{
					surplus_soc_ = 0;
				}
				else if((surplus_soc_ > soc_static) || (system_state.cells_staturate_state == 1))
				{
					surplus_soc_ = soc_static;
				}
				
				////////////////////////////////////////
				if( (bms_self_flag1.soc_static_go_on == 1)&&(bms_self_flag1.soc_static_formort == 1) )
				{
					soc_static_formort = soc_static_formort + dch_current_;
				}
				////////////////////////////////////////
			}
		}
		
	}
	
	/////////////////////////////////////
	/////////////////////////////////////////
	if(bms_self_flag1.minute_chgoc_count_begin==1)
	{
		minute_chgoc_count_time++;
		if(minute_chgoc_count_time>2500)// 7500 = one minute
		{
			bms_self_flag1.minute_chgoc_count_begin = 0;
			minute_chgoc_count_time = 0;
			pre_30fenzhong_count = 0;
		}
	}
	else
	{
		minute_chgoc_count_time = 0;
	}
	if(bms_self_flag1.pre_30fenzhong==1)
	{
		pre_30fenzhong_count++;
		if(pre_30fenzhong_count>225000)// 225000 = 30 钟计时到达
		{
			bms_self_flag1.pre_30fenzhong = 0;
			pre_30fenzhong_count = 0;		
		}
		
	}
	else
	{
		pre_30fenzhong_count = 0;
	}
	
	if(bms_self_flag1.heat_10fenzhong == 1)
	{
		heat_10fenzhong_count++;
		if(heat_10fenzhong_count>7500) //75000 = 10fenzhong
		{
			heat_10fenzhong_count = 0;
			bms_self_flag1.heat_10fenzhong = 0;
		}
		
	}
	
	/////////////////////////////////////	////

  TIM1_ClearITPendingBit(TIM1_IT_Update);
	
	dormancy_time_count++;//休眠计时
	if(dormancy_time_count < DORMANCY_TIME_400MS)
	{
		if(flag.dormancy_on == 1)
		{
			//	wfi();//休眠时间没有到继续休眠
			flag.tim1_dormancy_over = 0;
		}
	}
	else
	{
		dormancy_time_count = 0;
		flag.tim1_dormancy_over = 1;
	}
	
}