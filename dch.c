#include "dch.h"


int temp_dch_count = 0;
int dch_low_count = 0;

//禁止放电检测,通过返回值可以判断当前是神吗原因停止放电
int Forbid_Dch_Check(void)
{
	
	//放电过温
	if( system_state.temp_dch_low_protect_statue == 1 || \
	    system_state.temp_dch_high_protect_statue == 1)
		{
			//temp_dch_count++;
			//if(temp_dch_count == 4)
			//{
				DO_LOW();
				system_state.discharge_state = 0;
				return 1;
				//temp_dch_count = 0;
			//}
		}
		//else temp_dch_count = 0;
		
		//放电过流
	if(system_state.dch_oc_protect_statue == 1)		
		{
			DO_LOW();
			system_state.discharge_state = 0;
			return 2;
		}
		
		
		//放电电芯欠压	
	if(system_state.cell_one_low_state == 1)		
		{
			dch_low_count++;
			if(dch_low_count == 4)
			{
				DO_LOW();
				system_state.discharge_state = 0;
				dch_low_count = 0;
				return 3;
			}
		}
		else dch_low_count = 0;
		
		//主控命令禁止放电
	 if(receive_cmd.forbid_dch == 1)	
		{
			DO_LOW();
			system_state.discharge_state = 0;
			return 4;
		}								
	
}


//放电过流情况控制
void Dch_Oc_Control(void)
{
	
	if(system_state.dch_oc_protect_statue == 1)
	{
		DO_LOW();	
		system_state.discharge_state = 0;
		flag.dch_oc_protect_time	= 1;			
	}	
	
	if(flag.dch_oc_protect_time	== 1)
	{	
		if(dch_oc_time >= TIME_2S)//检测到过流保护有2s的时间了，尝试再次放电
		{
				dch_oc_time = 0;
				system_state.dch_oc_protect_statue = 0;//清除放电过流状态
				flag.dch_oc_protect_time	= 0;	// 清除过流倒计时标志		
				if(system_flag.en_discharge == 1)	//先检查是否可以放电
				{
					disableInterrupts();
					DO_HIGH();
					DAGE_HIGH();
					Delay(8);// 
					DAGE_LOW();
					enableInterrupts();
				}	
		}	
	}
	
}


int do_low_count = 0;
//放电控制，只要平常条件满足，就要将放电管打开？？？？？？？？？？放电管一般情况下常开
void Dch_Control(void)
{
	if(system_flag.en_discharge == 1 /* && \
		system_state.dch_oc_protect_statue == 0 && \
		flag.dch_mos_protect == 0*/
		)
	{
		DO_HIGH();
		dch_low_count = 0;
	}
	else
	{
		dch_low_count++;
		if(dch_low_count >= 4)
		{
			 DO_LOW();
			 //system_state.discharge_state = 0;	
			 dch_low_count = 0;
	 }
	}
	
			
}


void Da_Ge(void)
{
	if(dage_time_2s >= DAGE_TIME_2S)
	{
			dage_time_2s = 0;
		if(
				system_state.short_protect_statue == 0 && \
				system_state.cell_one_low_state == 0	 && \
				system_state.dch_ov_vbat_protect == 0  && \
				system_state.t_vbat_low_deep_state	== 0 && \
				system_state.temp_dch_low_protect_statue == 0 && \
				system_state.temp_dch_high_protect_statue == 0 && \
				/*system_state.temp_chg_low_protect_statue == 0 	&& \
				system_state.temp_chg_high_protect_statue == 0   && \*/
				system_state.dch_cell_anomaly == 0	 && \
				system_state.hardware_tb_statue == 0
			)
			{	
				disableInterrupts();
				DO_HIGH();
				dage_time_2s = 0;
				DAGE_HIGH();
				Delay(8);// 4 MHZ--25-29us
				DAGE_LOW();
				enableInterrupts();	
			}
	}
	
	if((bms_self_flag1.first2s_dage==0) && (system_flag.en_discharge == 1))
	{
			if(time1_count>251)//2s 开机2s后，尝试打嗝 (不开输出的打嗝) 防止一开机就有低温保护等
			{
				disableInterrupts();
				dage_time_2s = 0;
				DAGE_HIGH();
				Delay(8);// 4 MHZ--25-29us
				DAGE_LOW();
				enableInterrupts();	
				bms_self_flag1.first2s_dage = 1;
			}
		
	}

}

u32 dch_current_1 = 0;
u32 dch_current_2 = 0;

#ifndef jiaozhun_current_kk
UINT16 Dch_Current(void)
{
	#ifndef NEW_CURRENT_FORMAORT
		//X = 9.78Y
		dch_current_1 = adc_temp_voltage_add[17]>>3;
		dch_current_2 = (dch_current_1>>2)*3;
		dch_current_1 = dch_current_1 * 9;
		dch_current_1 = dch_current_1 + dch_current_2;
		
		//X = (Y+10)/1.473
		if(dch_current_1 > 20)
		{
			dch_current_1 = dch_current_1 + 100;
			dch_current_2 = (dch_current_1/25);
			dch_current_2 = dch_current_2*17;
			dch_current_1 = dch_current_2;
		}
	#else
		//x=9.390024x
		dch_current_1 = adc_temp_voltage_add[17]>>3;	
		dch_current_2 = dch_current_1*9;
		dch_current_1 = dch_current_1*10;
		dch_current_1 = dch_current_1>>7;
		dch_current_1 = dch_current_1*5;
		dch_current_1 = dch_current_1 + dch_current_2;
		if(dch_current_1>20)
		{
			dch_current_1 = dch_current_1 + 10;
		}
		
	#endif
	
	
	return dch_current_1;
}
#else
UINT16 Dch_Current(void)
{
	#ifndef NEW_CURRENT_FORMAORT
		//X = 9.78Y
		dch_current_1 = adc_temp_voltage_add[17]>>3;
		dch_current_2 = (dch_current_1>>2)*3;
		dch_current_1 = dch_current_1 * 9;
		dch_current_1 = dch_current_1 + dch_current_2;
		
		//X = (Y+10)/1.473
		if(dch_current_1 > 20)
		{
			dch_current_1 = dch_current_1 + 100;
			dch_current_2 = (dch_current_1/25);
			dch_current_2 = dch_current_2*17;
			dch_current_1 = dch_current_2;
		}
	#else
		//x=9.390024x
		dch_current_1 = adc_temp_voltage_add[17]>>3;	
		dch_current_2 = dch_current_1*9;
		dch_current_1 = dch_current_1*10;
		dch_current_1 = dch_current_1>>7;
		dch_current_1 = dch_current_1*5;
		dch_current_1 = dch_current_1 + dch_current_2;
		if(dch_current_1>1000)
		{
			dch_current_2 = dch_current_1*10;
			dch_current_2 = dch_current_2/156;
			if(dch_current_2>24)
			{
				dch_current_2 = dch_current_2 -24;
			}
			if(dch_current_1 > dch_current_2)
			{
				dch_current_1 = dch_current_1 - dch_current_2;
			}
		}
		else
		{
			dch_current_2 = dch_current_1/100;
			dch_current_2 = dch_current_2*7;
			if(dch_current_2>15)
			{
				dch_current_2 = dch_current_2 -15;
			}
			if(dch_current_1 > dch_current_2)
			{
				dch_current_1 = dch_current_1 - dch_current_2;
			}
		}
		/*if(dch_current_1>20)
		{
			dch_current_1 = dch_current_1 + 10;
		}*/
		
	#endif
	
	
	return dch_current_1;
}
#endif