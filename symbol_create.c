/*
	标志位生成
*/
#include "main.h"


UINT16 CHG_CURRENT_PROTECT = 13000;
UINT16 DCH_CURRENT_PROTECT = 18000;

union _UNION16_system_flag system_flag;//系统标志位
union _UNION32_system_state system_state;//系统状态位
union _UNION8_receive_cmd receive_cmd;//接收命令CMD
union _UNION16_fault_query fault_query;//故障查询位
union _UNION32_flag flag;//
union _UNION32_bms_self_flag bms_self_flag;//只用于 BMS自身 通 信的各种标志，通信断开时 几秒后会自动清零
union _UNION32_bms_self_flag_1 bms_self_flag1;
UINT8 pre_en_charge;			//预充电标识，1为 开启 预充电
void Port_Common_Init(void)
{
	GPIO_Init(GPIOE,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Fast);												//PRE_CHG				PE0	
	GPIO_Init(GPIOE,GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Fast);												//EN_HEAT  			PE1
	
	//GPIO_Init(GPIOD,GPIO_Pin_2,GPIO_Mode_In_PU_No_IT);													//INT2					PD2
	//GPIO_Init(GPIOD,GPIO_Pin_3,GPIO_Mode_In_PU_No_IT);													//INT1  		  	PD3
	
	GPIO_Init(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Fast);												//DAGE  				PB0 	
	//GPIO_Init(GPIOG,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Fast);												//EN_SUPPLY  		PG3  do not use now.
	
	GPIO_Init(GPIOF,GPIO_Pin_7,GPIO_Mode_In_PU_No_IT);													  //SEC_OV  				PF7
	GPIO_Init(GPIOF,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Fast);												//FAN_SWITCH			PF6
	//GPIO_Init(GPIOF,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Fast);												//MAIN_SWITCH			PF5 not user
	GPIO_Init(GPIOF,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Fast);												//BAL_CON					PF4
	
	
	GPIO_Init(GPIOG,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Fast);												//CHG_DAGE				PG6
	GPIO_Init(GPIOE,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Fast);												//EN_MEASURE   		PE7
	
	
}

void CoDoInit(void)
{
	GPIO_Init(GPIOG,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Fast);												//C0			  			PG4	
	GPIO_Init(GPIOG,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Fast);												//D0							PG5
}



UINT8 dch_pre_chg_flag = 0;
FlagStatus sec_ov_bit = SET;
UINT8 sec_ov_bit_count = 0;

UINT16 DCH_CURRENT = 0;
UINT16 DCH_CURRENT_COUNT = 0;// 放电过流计数， 连续 3 次判断才 认为真正过流
UINT16 CHG_CURRENT = 0;
UINT16 CHG_CURRENT_COUNT = 0;// 充电过流计数，连续 3 次判断才认为真正过流
UINT16 deep_state_count = 1; //深度欠压 计数，连续 6 次判断才人为真正深度过放
int vadd_reduce_vbat = 0;
int vbat_reduce_vadd = 0;
int DCH_cell_ANOMALY_count = 0;//电芯电压异常计算，连续 200 次才认为异常
int dch_fault_count = 0;
UINT8 chg_fault_count = 0;
UINT8 samping_fault_count = 0;
UINT8 cell_one_low_count = 0;
UINT8 cell_one_low_re_count = 0;
UINT8 t_vbat_low_deep_state = 0;
UINT8 dch_ov_vbat_alarm_count = 0;
UINT8 dch_ov_vbat_realarm_count = 0;
UINT8 dch_ov_vbat_protect_count = 0;
UINT8 dch_ov_vbat_reprotect_count = 0;
UINT8 cell_one_low_alarm_state_count = 0;
UINT8 cells_over_chg_state_count = 0; 
UINT8 cells_staturate_state_count = 0;
UINT8 temp_chg_low_protect_statue_count = 0;
UINT8 temp_chg_high_protect_statue_count = 0;
UINT8 temp_dch_low_protect_statue_count = 0;
UINT8 temp_dch_high_protect_statue_count =0;
UINT8 cell_one_low_realarm_state_count = 0;
UINT8 cells_over_chg_restate_count = 0;
UINT8 temp_chg_low_protect_restatue_count = 0;
UINT8 temp_dch_high_protect_restatue_count = 0;//解除放电高温保护状态jishu
UINT8 temp_dch_low_protect_restatue_count = 0;
UINT8 temp_chg_high_protect_restatue_count = 0;
UINT8 short_protect_restatue_count = 0;//短路保护解除
UINT8 short_protect_statue_count = 0;
UINT8 thermister_fault_count = 0;
UINT8 b_lowest_gate_count = 0;
UINT8 chg_voltage_stop_count = 0;
UINT8 chg_voltage_restop_count = 0;
UINT8 cells_staturate_restate_count = 0;
UINT8 hardware_tb_statue_count = 0;
UINT8 pre_charge_ok_count = 0;
UINT8 vbat_ov_protect_count = 0;
UINT8 vbat_ov_reprotect_count = 0;
//int vbat_reduce_vadd = 0;
UINT8 chg_min_ov_voltage_count = 0;
UINT8 chg_min_ov_revoltage_count = 0;
UINT8 mos_protect_count = 0;
UINT8 mos_reprotect_count = 0;
UINT8 flag_pre_charge_ok_count = 0;
void Symbol_Create(void)
{		
		//disableInterrupts();	
		
		flag.sec_ov = SEC_OV_READ();		
		vadd_reduce_vbat = (int)((int)total_voltage_add - (int)t_vbat);
		vbat_reduce_vadd = (int)((int)t_vbat) - (int)total_voltage_add;
		//enableInterrupts();
		

		
		if(min_voltage <= DCH_OV_PROTECT)//小于过放保护点，欠压了
		{	
			cell_one_low_count++;
			if(cell_one_low_count >= COUNT_COUNT)
			{
				system_state.cell_one_low_state = 1;//单节过放保护
				
				flag.dch_protect_pornt = 1;
				cell_one_low_count = 0;
			}
		}
		else 
		{
			cell_one_low_count = 0;	
		}
		
		if((min_voltage >= DCH_OV_RE) || (system_state.charge_state == 1))
		{
			cell_one_low_re_count ++;
			if(cell_one_low_re_count >= COUNT_COUNT)
			{
				system_state.cell_one_low_state = 0;
				cell_one_low_re_count = 0;
			}	
		}
		else 
		{
			cell_one_low_re_count = 0;
		}
		

		if((min_voltage <= DCH_OV_LOW_ANOMALY) || (max_voltage >= DCH_OV_HIGH_ANOMALY))  //电芯异常 1500 4500
		{
			DCH_cell_ANOMALY_count++;
			if(DCH_cell_ANOMALY_count >= COUNT_COUNT)
			{
				system_flag.en_charge = 0;
				system_flag.en_discharge = 0;
				system_flag.en_heat =0;
				system_state.dch_cell_anomaly = 1;//将不能充放加热---------------
				DCH_cell_ANOMALY_count = 0;
			//	Write_VOLTAGE_anomaly();
			//	Write_system_flag();
			}
		}
		else
		{
			DCH_cell_ANOMALY_count = 0;
			system_state.dch_cell_anomaly = 0;//暂时将其设置成可解除
		}		

		if(t_vbat < DCH_0V_VBAT_ALARM)
		{
			dch_ov_vbat_alarm_count++;
			if(dch_ov_vbat_alarm_count >= COUNT_COUNT)
			{
				system_state.dch_ov_vbat_alarm = 1;// 电池组欠压告警
				dch_ov_vbat_alarm_count = 0;
			}
		}
		else
		{
			dch_ov_vbat_alarm_count = 0;
		}		
		
		if(t_vbat > DCH_0V_VBAT_REALARM)
		{
			dch_ov_vbat_realarm_count++;
			if(dch_ov_vbat_realarm_count >= COUNT_COUNT)
			{
				system_state.dch_ov_vbat_alarm = 0;
				dch_ov_vbat_realarm_count = 0;
			}
		}
		else
		{
			dch_ov_vbat_realarm_count = 0;
		}		
		
		if(t_vbat < DCH_0V_VBAT_PROTECT)
		{
			dch_ov_vbat_protect_count++;
			if(dch_ov_vbat_protect_count >= COUNT_COUNT)
			{
				system_state.dch_ov_vbat_protect = 1;//电池组欠压保护
				flag.dch_protect_pornt = 1;
				dch_ov_vbat_protect_count = 0;
			}
		}
		else
		{
			dch_ov_vbat_protect_count = 0;
		}	
		
		if((t_vbat > DCH_0V_VBAT_REPROTECT) || (system_state.charge_state == 1))
		{  
			dch_ov_vbat_reprotect_count++;
			if(dch_ov_vbat_reprotect_count >= COUNT_COUNT)
			{
				system_state.dch_ov_vbat_protect = 0;
				dch_ov_vbat_reprotect_count = 0;
			}
		}
		else
		{
			dch_ov_vbat_reprotect_count = 0;
		}		
		
		
		if(t_vbat <= DCH_0V_VBAT_DEEP)//深度欠压，将不能充放加热---------------
		{	
			deep_state_count ++;
			if(deep_state_count >= 200)
			{
				//deep_state_count = 0;
				system_flag.en_charge = 0;
				system_flag.en_discharge = 0;
				system_flag.en_heat =0;
				system_state.t_vbat_low_deep_state = 1;//this flag can not reset``````````````注意要连续判断 3 到 6 次都是这个条件才进入深度欠压状态哦
				deep_state_count = 0;
			}
				
 		}
		else
		{
			deep_state_count = 0;		
		}		
			
		
		if(min_voltage <= ALARM_EN)//低压告警
		{	
			cell_one_low_alarm_state_count++;
			if(cell_one_low_alarm_state_count >= COUNT_COUNT)
			{
				system_state.cell_min_low_alarm_state = 1;	
				cell_one_low_alarm_state_count = 0;
			}
		}
		else
		{
			cell_one_low_alarm_state_count = 0;
		}	
		
		if(min_voltage >= ALARM_RE)//低压告警解除
		{	
			cell_one_low_realarm_state_count++;
			if(cell_one_low_realarm_state_count >= COUNT_COUNT)
			{
				system_state.cell_min_low_alarm_state = 0;
				cell_one_low_realarm_state_count = 0;
			}
		}
		else	
		{
			cell_one_low_realarm_state_count = 0;
		}	
	
		
		
		if(max_voltage >= CHG_OV_PROTECT)
		{	
			cells_over_chg_state_count++;
			if(cells_over_chg_state_count >= COUNT_COUNT )
			{
				system_state.cells_over_chg_state = 1;//电芯过充 软 件 二级保护
				cells_over_chg_state_count = 0;
			}
		}
		else
		{
			cells_over_chg_state_count = 0;
		}	
		
		
		if(max_voltage <= CHG_OV_RE)	
		{
			cells_over_chg_restate_count++;
			if(cells_over_chg_restate_count >= COUNT_COUNT)
			{
			//	system_state.cells_over_chg_state = 0;//过充解除						
			}
		}
		else 
		{
			cells_over_chg_restate_count = 0;
		}
		
		if(t_vbat >= CHG_VBAT_OV_PROTECT )
		{
			vbat_ov_protect_count++;
			if(vbat_ov_protect_count > COUNT_COUNT)
				{
					system_state.vbat_ov_protect = 1;		//总电压过充 保护
					vbat_ov_protect_count = 0;
				  bms_self_flag1.t_vbat_off_prechg = 1;
					flag.chg_voltage_stop_8s_on = 1;
				}
		}
		else
		{
			vbat_ov_protect_count = 0;	
		}
		
		if(system_state.vbat_ov_protect == 1 && \
			t_vbat <= CHG_VBAT_OV_REPROTECT			&& \
			min_voltage < CHG_MIN_OV_REVOLTAGE 
			)
		{
			vbat_ov_reprotect_count++;
			if(vbat_ov_reprotect_count > COUNT_COUNT)
			{
				vbat_ov_reprotect_count = 0;
				bms_self_flag1.t_vbat_off_prechg = 0;
				if(flag.chg_voltage_stop_8s_on == 0)
				{
					system_state.vbat_ov_protect = 0;//总电压过充解除
				}
			}
			
		}
		else
		{
			vbat_ov_reprotect_count = 0;
		}
	
		//max_voltage_symbol = CHG_CURRENT/chgrepair_voltage;		
		//if(max_voltage_symbol>100)
		//{
		//	max_voltage_symbol = 100;
		//}
		//min_voltage_symbol = max_voltage_symbol;
		//max_voltage_symbol = max_voltage - max_voltage_symbol;		
		//min_voltage_symbol = min_voltage - min_voltage_symbol;
		max_voltage_symbol = max_voltage;		
		min_voltage_symbol = min_voltage;
		
		if(	(max_voltage_symbol <= CHG_SATIATION_MAX) && \
				(min_voltage_symbol >= CHG_SATIATION_MIN) && \
				(CHG_CURRENT <50)													/*&& \
				(system_flag.en_charge == 1)*/
			)
		{
			cells_staturate_state_count++;
			if(cells_staturate_state_count >= COUNT_COUNT)
			{
				system_state.cells_staturate_state = 1;//电芯饱和设置		
				//pre_en_charge = 0;// 电池组饱和了，预 充电 功能关闭
				flag.chg_voltage_stop_8s_on = 1;//表示 至少 要过90秒后 才 能开 充电标识哦
				cells_staturate_state_count = 0;
				//flag.current_voltage_chgrepair = 1;
				//CURRENT_VOLTAGE_chgrepair = CHG_CURRENT;
				//current_voltage_1 = max_voltage;
			
			}
		}
		else
		{	
			cells_staturate_state_count = 0;
			if(system_state.cells_staturate_state == 1)
			{
				if(system_state.discharge_state == 1)
				{
					system_state.cells_staturate_state = 0;
				}
				
				if(min_voltage <= CHG_RECHG)
				{
					cells_staturate_restate_count++;
					if(cells_staturate_restate_count >= COUNT_COUNT)
					{
						if(flag.chg_voltage_stop_8s_on == 0)
						{
							system_state.cells_staturate_state = 0;//电芯饱和设置解除
						}
						
						//pre_en_charge = 1;// 电池组饱和了，预 充电 kaiqi
						flag.w_soc_static = 1; //在这之后 当满电时可以调用 soc_re()
						cells_staturate_restate_count = 0;
					}
				}
				else
				{
					cells_staturate_restate_count = 0;
				}			
			}
		}
		
		if(max_voltage_symbol >= CHG_VOLTAGE_STOP)
		{
			chg_voltage_stop_count++;
			if(chg_voltage_stop_count >= COUNT_COUNT)
			{
				system_state.chg_voltage_stop = 1;
				//system_state.cells_over_chg_state = 1;
				flag.chg_voltage_stop_8s_on  = 1;
				chg_voltage_stop_count = 0;	
				//flag.current_voltage_chgrepair = 1;
				//CURRENT_VOLTAGE_chgrepair = CHG_CURRENT;
				//current_voltage_1 = max_voltage;
			}
		}
		else
		{
			chg_voltage_stop_count = 0;
		}		
	
		if(max_voltage <= CHG_VOLTAGE_RESTOP) 
		{
			chg_voltage_restop_count++;
			if(chg_voltage_restop_count >= COUNT_COUNT)
			{
				chg_voltage_restop_count = 0;
				if(flag.chg_voltage_stop_8s_on == 0)
				{	
					system_state.chg_voltage_stop = 0;
					system_state.cells_over_chg_state = 0;	
				}
			}
		}
		else	
		{
			chg_voltage_restop_count = 0;	
		}
			
	
		//if(charge_state == 1) //这个条件好像可不要 
		//{
			if(min_temperature <= TEMP_CHG_LOW_PROTECT)//有一个负温判断
			{	
				temp_chg_low_protect_statue_count++;
				if(temp_chg_low_protect_statue_count >= COUNT_COUNT)
				{					
					system_state.temp_chg_low_protect_statue = 1;//设置充电低温保护状态
					temp_chg_low_protect_statue_count = 0;
					//bms_self_flag1.temp_l_off_prechg = 1;
				}
			}
			else
			{
				temp_chg_low_protect_statue_count = 0;
			}		
														
			if(min_temperature >= TEMP_CHG_LOW_RE)
			{
				temp_chg_low_protect_restatue_count++;
				if(temp_chg_low_protect_restatue_count >= COUNT_COUNT)
				{
					system_state.temp_chg_low_protect_statue = 0;	//解除充电低温保护状态
					temp_chg_low_protect_restatue_count = 0;
					//bms_self_flag1.temp_l_off_prechg = 0;
				}
			}
			else
			{
				temp_chg_low_protect_restatue_count = 0;
			}	

		//}
			
			if(min_temperature <= TEMP_PRE_LOW_PROTECT)
			{
				bms_self_flag1.temp_l_off_prechg = 1;
			}
			if(min_temperature >= TEMP_PRE_LOW_RE)
			{
				bms_self_flag1.temp_l_off_prechg = 0;
			}
			
			if(max_temperature >= TEMP_PRE_HIGH_PROTECT)
			{
				bms_self_flag1.temp_h_off_prechg = 1;
			}
			if(max_temperature <= TEMP_PRE_HIGH_RE)
			{
				bms_self_flag1.temp_h_off_prechg = 0;
			}
			
			if((max_temperature >= TEMP_CHG_HIGH_PROTECT) || \
			   (flag.dch_mos_protect == 1)
				)
			{													//设置充电高温保护状态
				temp_chg_high_protect_statue_count++;
				if(temp_chg_high_protect_statue_count >= COUNT_COUNT)
				{
					system_state.temp_chg_high_protect_statue = 1;
					temp_chg_high_protect_statue_count = 0;
					//bms_self_flag1.temp_h_off_prechg = 1;
				}
			}
			else
			{
				temp_chg_high_protect_statue_count = 0;
			}		
														
			if((max_temperature <= 50) && \
					(min_temperature <= TEMP_CHG_HIGH_RE) && \
					(flag.dch_mos_protect == 0)
				)
			{
				temp_chg_high_protect_restatue_count++;
				if(temp_chg_high_protect_restatue_count >= COUNT_COUNT) 					
				{
					system_state.temp_chg_high_protect_statue = 0;	//解除充电高温保护状态
					temp_chg_high_protect_restatue_count = 0;
					//bms_self_flag1.temp_h_off_prechg = 0;
				}
			}
			else
			{
				temp_chg_high_protect_restatue_count = 0;
			}		
		
	
			if(min_temperature <= TEMP_DCH_LOW_PROTECT)
			{													//设置放电低温保护状态
				temp_dch_low_protect_statue_count++;
				if(temp_dch_low_protect_statue_count >= COUNT_COUNT)
				{
					system_state.temp_dch_low_protect_statue = 1;
					temp_dch_low_protect_statue_count = 0;
				}
			}
			else
			{
				temp_dch_low_protect_statue_count = 0;
			}		
												
			if(min_temperature >= TEMP_DCH_LOW_RE)
			{
				temp_dch_low_protect_restatue_count++;
				if(temp_dch_low_protect_restatue_count >= COUNT_COUNT)
				{
					system_state.temp_dch_low_protect_statue = 0;	//解除放电低温保护状态
					temp_dch_low_protect_restatue_count = 0;
				}
			}
			else
			{
				temp_dch_low_protect_restatue_count = 0;
			}		
		
			if((max_temperature >= TEMP_DCH_HIGH_PROTECT) || \
				 (flag.dch_mos_protect == 1)
				)
			{	
				temp_dch_high_protect_statue_count++;
				if(temp_dch_high_protect_statue_count >= COUNT_COUNT) 	
				{
					system_state.temp_dch_high_protect_statue = 1;//设置放电高温保护状态
					temp_dch_high_protect_statue_count = 0;
				}
			}
			else 
			{
				temp_dch_high_protect_statue_count = 0;
			}

			if((max_temperature <= TEMP_DCH_HIGH_RE) && \
				 (flag.dch_mos_protect == 0)
				)
			{	
				temp_dch_high_protect_restatue_count++;
				if(temp_dch_high_protect_restatue_count >= COUNT_COUNT) 
				{
					system_state.temp_dch_high_protect_statue = 0;//解除放电高温保护状态
					temp_dch_high_protect_restatue_count = 0;
				}
			}
			else
			{
				temp_dch_high_protect_restatue_count = 0;
			}		

		
		
		if(DCH_CURRENT > DCH_CURRENT_SET)//DCH_CURRENT_SET = 200mA
		{
			system_state.discharge_state = 1;//放电 状态
		}
		else
		{
			system_state.discharge_state = 0;
		}
		if(CHG_CURRENT > CHG_CURRENT_SET)
		{	
			system_state.charge_state = 1;//充电 状态
		}
		else
		{
			system_state.charge_state = 0;
		}	
		
		
		if(DCH_CURRENT >= DCH_CURRENT_PROTECT)
		{
			DCH_CURRENT_COUNT ++;
			if(DCH_CURRENT_COUNT >= 3) // 连 续 判断 3 次 都过流了
			{
				system_state.dch_oc_protect_statue = 1;//设置放电过流状态
				flag.dch_oc_protect_time	= 1;// 放电过流 2s 倒计时标志启动	
				DCH_CURRENT_COUNT = 0;
			}
								
		}
		else
		{
			DCH_CURRENT_COUNT = 0;
			// system_state.dch_oc_protect_statue = 0;//清除放电过流状态	 放在 2 S 的倒计时完成函数里清除
		
		}		
			
		

		if(CHG_CURRENT > CHG_CURRENT_PROTECT )// 13A CHG_CURRENT_PROTECT
		{		
				CHG_CURRENT_COUNT ++;
				if(CHG_CURRENT_COUNT >= 3) // 连 续 判断 3 次 都过流了
				{
					CHG_CURRENT_COUNT = 0;
					system_state.chg_oc_protect_statue = 1;//设置充电电过流状态	
					if(system_state.charge_state == 1 )
					{
						charge_state_flag = 1;//正常充电时发生过流  2s 倒计时标志启动	
					}
					minute_chgoc_count++;
					if(minute_chgoc_count>3)
					{
						minute_chgoc_count = 5;
					}
				}				
		}
		else
		{
			CHG_CURRENT_COUNT = 0;
			//	system_state.chg_oc_protect_statue = 0;//清除充电过流状态	放在 2 S 的倒计时完成函数里清除
		}
		
		if(CHG_CURRENT>30000)
		{
		 system_state.chg_lock = 1;
		}
		else
		{
			system_state.chg_lock = 0;
		}
		




		// 均衡判断
		if(max_voltage > B_LOWEST_GATE)		// 电压dayu均衡门限3400mv
		{
			//b_lowest_gate_count++;
			//if(b_lowest_gate_count >= COUNT_COUNT)
			//{
				flag.balance_low =1;
				//b_lowest_gate_count = 0;
			//}
		}
		else 
		{
			//b_lowest_gate_count = 0;
			flag.balance_low =0;
		}
		if(system_state.discharge_state == 1)
		{
			flag.balance_low =0;
		}
	
		sec_ov_bit = GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7);
		if(sec_ov_bit == RESET)
		{
			sec_ov_bit_count++;
			if(sec_ov_bit_count > COUNT_COUNT)
			{
				system_state.sec_ov_protect = 1;
			}
		}
		else
		{
			system_state.sec_ov_protect = 0;
			sec_ov_bit_count = 0;
		}
		
		//硬件二级过压故障查找
		if((max_voltage>=SEC_OV_MAX) && (flag.sec_ov==1)	)
		{
		//	system_flag.serious_alarm = 1;
		//		fault_query.second_protect_fault = 1;
		}
		else if((max_voltage<=SEC_OV_MIN) && (flag.sec_ov==0)	)
		{
		//		fault_query.second_protect_fault = 1;
		//	system_flag.serious_alarm = 1;
		}
		else
		{
			//system_flag.serious_alarm = 0;
		//		fault_query.second_protect_fault = 0;				
		}
		
		//电芯检测电路故障	
		//if( total_voltage_add - t_vbat> VADD_VBAT_SPACE || t_vbat - total_voltage_add > VADD_VBAT_SPACE)
		if((vadd_reduce_vbat > VADD_VBAT_SPACE )|| (vbat_reduce_vadd > VADD_VBAT_SPACE))
		{
			samping_fault_count++;
			if(samping_fault_count > 100)
			{
				fault_query.cell_samping_fault = 1;
				samping_fault_count = 0;
			}
		}
		else 
		{
			samping_fault_count = 0;
			#ifdef SYSTEM_TEMP_TEST
				fault_query.cell_samping_fault = 0;
			#endif
			
		}
		
				
		//热敏电阻故障
		if(min_temperature <= TEMP_THERMISTER_FAULT)
		{	
			thermister_fault_count++;
			if(thermister_fault_count >= COUNT_COUNT)
			{
				//fault_query.thermister_fault = 1;
			}		
		}   
		else
		{
				thermister_fault_count = 0;
		}
		
	
		
		//充电电路故障
		if(system_flag.en_charge == 0 && system_state.charge_pre_state == 0)
		{	
			if(CHG_CURRENT >= 200)//表示禁止充电时 充电电流大于0.2A
			{
				chg_fault_count++;
				if(chg_fault_count >= 100)
				{	
					fault_query.chg_fault = 1;
					chg_fault_count = 0;
				}
			}
			else chg_fault_count = 0;
		}
		else
		{
			chg_fault_count = 0;
				//if()pre_chg开启时，有电流，co开启时却没有电流
			#ifdef SYSTEM_TEMP_TEST
				fault_query.chg_fault = 0;
			#endif
		}
		
		//放电电路故障
		
		if((system_flag.en_discharge == 0) && (DCH_CURRENT >= 200)||((DCH_CURRENT >= 200)&&(CHG_CURRENT >= 200)))//表示禁止放电时 放电电流大于0.2A  同时有充电放电电流时有硬件问题
		{
			dch_fault_count++;
			if(dch_fault_count >= 100)
			{
				fault_query.dch_fault = 1;
				dch_fault_count = 0;
			}
		}
		else
		{
			dch_fault_count = 0;
			#ifdef SYSTEM_TEMP_TEST
				fault_query.dch_fault = 0;
			#endif
		}	
		
		
			if(fault_query.U16 != 0 )
			{
				hardware_tb_statue_count++;
				if(hardware_tb_statue_count>=100)
				{
					#ifdef SYSTEM_TEMP_TEST
						system_state.hardware_tb_statue  = 0;
					#else
						system_state.hardware_tb_statue = 1;//硬件故障标志，不可恢复哦
					#endif
					//system_flag.en_charge = 0;
					//system_flag.en_discharge = 0;
					//system_flag.en_heat =0;
					//pre_en_charge = 0;
					hardware_tb_statue_count = 0;
				}
			}
			else 
			{
				hardware_tb_statue_count = 0;
				
			}
			
			if(system_state.hardware_tb_statue == 1)
			{
				system_flag.serious_alarm = 1;
			}
			

		
		if(system_state.dch_ov_vbat_alarm == 1			||\
			 system_state.cell_min_low_alarm_state == 1
			)
		{
			system_flag.ordinary_alarm = 1;
		}
		else system_flag.ordinary_alarm = 0;
		
		
		
		
		//可放电标志设置
		if(fault_query.dch_fault == 1 											|| \
			 fault_query.second_protect_fault == 1 						|| \
			 receive_cmd.forbid_dch == 1											|| \
			 flag.dch_mos_protect == 1												|| \
			 system_state.dch_oc_protect_statue == 1 					|| \
			 system_state.short_protect_statue == 1 					|| \
			 system_state.temp_dch_high_protect_statue == 1 	|| \
			 system_state.temp_dch_low_protect_statue == 1 		|| \
			 system_state.cell_one_low_state == 1							|| \
			 system_state.t_vbat_low_deep_state	== 1					|| \
			 system_state.dch_cell_anomaly ==1								|| \
			 system_state.dch_ov_vbat_protect == 1						|| \
			 system_state.hardware_tb_statue == 1)
		{
			system_flag.en_discharge = 0;			 	
	 	}
	 	else
	 	{
	 		system_flag.en_discharge = 1;
	 	}
		/*
		if(pre_en_charge == 1  || \
			 system_flag.en_charge== 0)
		 {
				if(CHG_CURRENT > PRE_CURRENT_PROTECT)
				{
					receive_cmd.on_chgmos = 0; 
					flag.forbid_on_chgmos = 1;//强制清除这条命令 可保 正 常 充电MOS 关闭
				}
		 }
		 */
		if( system_state.chg_voltage_stop == 1								|| \
				system_state.temp_chg_low_protect_statue == 1 		|| \
				system_state.temp_chg_high_protect_statue == 1 		|| \
				fault_query.second_protect_fault == 1   					|| \
				system_state.cells_staturate_state == 1 					|| \
				system_state.t_vbat_low_deep_state	== 1					|| \
				system_state.vbat_ov_protect == 1									|| \
				system_state.dch_cell_anomaly ==1								  || \
				system_state.cells_over_chg_state == 1						|| \
					fault_query.U16 != 0  													|| \
					bms_self_flag1.temp_l_off_prechg == 1 					|| \
					bms_self_flag1.temp_h_off_prechg == 1	 					|| \
					bms_self_flag1.t_vbat_off_prechg == 1				
				//system_state.hardware_tb_statue == 1
			)
		{
			pre_en_charge = 0;
		}
		else
		{
			pre_en_charge = 1;
		}
		
	

		if(//receive_cmd.adapter_connect == 0 			||
				flag.pre_charge_ok == 0										||  \
				flag.forbid_on_chgmos == 1								||  \
				flag.chg_voltage_stop_8s_on  == 1					||  \
				flag.dch_mos_protect == 1									||	\
				receive_cmd.on_chgmos == 0								||	\
				system_state.chg_voltage_stop == 1				|| 	\
				system_state.temp_chg_low_protect_statue == 1 	|| \
				system_state.temp_chg_high_protect_statue == 1 	|| \
				fault_query.second_protect_fault == 1   				|| \
				system_state.cells_staturate_state == 1 			  || \
				system_state.t_vbat_low_deep_state	== 1				|| \
				system_state.dch_cell_anomaly == 1					|| \
				system_state.cells_over_chg_state == 1			|| \
				system_state.chg_oc_protect_statue == 1			|| \
				system_state.hardware_tb_statue == 1				|| \
				system_state.vbat_ov_protect == 1				    || \
				system_state.sec_ov_protect ==1							|| \
				system_state.chg_frequently_oc == 1
			) //  CHG_VOLTAGE_STOP 最低最高电芯电压达到3600停止充电```````````````````````````````  			
		{
			//设置不可充电标志	
			if(flag.cmd_on_chgmos_updata == 1)	
			{
				if((receive_cmd.on_chgmos == 1)											&& \
						(system_state.temp_chg_low_protect_statue == 1 	|| \
						system_state.t_vbat_low_deep_state	== 1				  || \
						system_state.hardware_tb_statue == 1)	)
				{
					system_flag.en_charge = 1;
				}
				
				flag.cmd_on_chgmos_updata = 0;
			}
			else 	
			{
				system_flag.en_charge = 0;
			}	
		}
		else 	system_flag.en_charge = 1;//设置可充电标志, 需要很多条件同时满足才可哦··········
		
		if(system_state.chg_oc_protect_statue == 1						|| \
				system_state.dch_oc_protect_statue == 1 				  || \
				/*system_state.cells_over_chg_state == 1						|| \*/
				system_state.sec_ov_protect == 1									|| \
				system_state.temp_chg_low_protect_statue == 1 		|| \
				system_state.temp_chg_high_protect_statue == 1 		|| \
				system_state.temp_dch_low_protect_statue == 1 		|| \
				system_state.temp_dch_high_protect_statue == 1 	  || \
				system_state.hardware_tb_statue == 1							|| \
				system_flag.en_discharge == 0								
			)
		{		
			system_flag.exist_protect_state = 1;
		}
		else
		{
			system_flag.exist_protect_state = 0;
		}


		if((t_n[13] >= TEMP_MOSFET_PROTECT) || \
			 (t_n[14] >= TEMP_MOSFET_PROTECT)	|| \
			 (t_n[15] >= TEMP_MOSFET_PROTECT)
		)
		{
			mos_protect_count++;
			if(mos_protect_count>COUNT_COUNT)
			{
				flag.dch_mos_protect = 1;	
				system_flag.en_charge = 0;
				system_flag.en_discharge = 0;		
				//system_state.heat_state = 1;//暂时用加热状态来代替 mos的过温保护
				mos_protect_count = 0;
			}
			
		}
		else
		{	
			mos_protect_count = 0;
		}
	if((t_n[13] <= TEMP_MOSFET_RE) && \
		 (t_n[14] <= TEMP_MOSFET_RE)	&& \
		 (t_n[15] <= TEMP_MOSFET_RE)
		)
		{
			mos_reprotect_count++;
			if(mos_reprotect_count > COUNT_COUNT)
			{
				flag.dch_mos_protect = 0;	
				//system_state.heat_state = 0;//暂时用加热状态来代替 mos的过温保护
				mos_reprotect_count = 0;
			}
			
		}	
		else
		{
			mos_reprotect_count = 0;
		}
		
		if(system_state.discharge_state == 1 && system_state.hardware_tb_statue == 0)
		{
			pre_en_charge = 1;
			system_flag.en_charge = 1;
		}
		if(system_state.charge_state == 1 && system_state.hardware_tb_statue == 0)
		{
			system_flag.en_discharge = 1;
		}
		
		Fan_Move();
		Soc_formort();
}  

////////
s32 soc_static_dchcheck = 0;
int soc_static_counter = 0;
int odd_soc_percent = 0;
int soc_percent = 0;
int soc_cycle_number = 0;
int min_write_test_count = 0;
int Write_Soc_Count_count = 0;
UINT8 soc_percent_before = 0;
UINT8 soc_percent_temp[3] = {0};
UINT8 soc_percent_tempcount = 0;
UINT16 TEMP_soc;
UINT16 soc_percent_;
///////

u32	soc_count   = 36000000;
u32 soc_static_formort = 0;
u32 soc_static  = 36000000;
u32 soc_static_  = 0;

s32 surplus_soc = 0;
s32 surplus_soc_ = 0;
int surplus_percent = 0;
int surplus_percent_ = 0;
UINT8 surplus_percent_before = 0;
UINT8 write_soc_flag = 0;
static UINT8 min_voltage_3430js = 0;
UINT16 TEMP_D;
void Soc_formort(void)
{
	TEMP_D = t_vbat;
	if(bms_self_flag1.readed_soc_static==0)
	{
		if(system_flag.soc_mode_10 == 1)	
		{
			//soc_static = 36000000;
			//soc_static_  = 360000;
			if(write_soc_flag == 0)
			{
				soc_static = 36000000;
			}
			else
			{
				Read_System_soc_static();
				if((soc_static < 3600000) || (soc_static > 460000000))
				{
					soc_static = 36000000;
					//是否要做一个错误处理？？？？？？？？
				}
			}
			soc_static_ = soc_static/100;
		}
		else
		{
			//soc_static = 72000000;
			if(write_soc_flag == 0)
			{
				soc_static = 72000000;
			}
			else
			{
				Read_System_soc_static();
				if((soc_static < 7200000) || (soc_static > 820000000))
				{
					soc_static = 72000000;
				}
			}
			soc_static_ = soc_static/100;
			
		}
		bms_self_flag1.readed_soc_static = 1; 
		bms_self_flag1.soc_static_formort = 0;
	}
	
	if(write_soc_flag == 0)
	 {
			surplus_percent_ = Find_soc_percent(TEMP_D);
			surplus_soc_ = soc_static_ * surplus_percent_;
			write_soc_flag = 1;
	 }
	 else if(write_soc_flag==2)
	 {
			surplus_percent_ = Find_soc_percent(TEMP_D);//这样每次复位都会从新查表，之后可将write_soc_flag的写FLSH去掉
			surplus_soc_ = soc_static_ * surplus_percent_;
			write_soc_flag=3;
	 }
	 
	if((DCH_CURRENT<30) && (CHG_CURRENT<30) )
	{
		if(min_voltage > 3430)
		{
			min_voltage_3430js = min_voltage_3430js + 1;
			if(min_voltage_3430js>50)
			{
				surplus_percent_ = Find_soc_percent(TEMP_D);
				surplus_soc_ = soc_static_ * surplus_percent_;
				min_voltage_3430js = 0;
			}
		}
		else
		{
			min_voltage_3430js = 0;
		}
		if((min_voltage > 3230) && (TEMP_D > 52000) && (soc_percent< 20))
		{
			surplus_percent_ = Find_soc_percent(TEMP_D);
			surplus_soc_ = soc_static_ * surplus_percent_;
		}
		if((max_voltage < 2900) && (soc_percent>10))
		{
			surplus_percent_ = Find_soc_percent(TEMP_D);
			surplus_soc_ = soc_static_ * surplus_percent_;
		}
	}
	if((system_state.cell_one_low_state == 1)||(system_state.dch_ov_vbat_protect == 1))
	{
		surplus_percent_ = 0;
		surplus_soc_ = 0;
		//flag.alow_time_countsoc = 0;
	}
	else
	{
		flag.alow_time_countsoc = 1;
		surplus_percent_ = surplus_soc_ /soc_static_;
		if((surplus_percent_ > 0) && (surplus_percent_ < 100) )
		{
			surplus_percent_ = surplus_percent_ +1;	
		}
		else if (surplus_percent_ == 0)
		{
			if(surplus_soc_ < 10)
			{
				surplus_percent_ = 0;
			}
			else
			{
				surplus_percent_ = 1;
			}
		}
	}
	
	surplus_percent = surplus_percent_;
	if((surplus_percent == 0) && (system_state.discharge_state == 1))
	{
		soc_percent = 1;	//	放电状态下容量显示不能为0%
	}
	else if((surplus_percent == 100) && (system_state.charge_state == 1))
	{
		soc_percent = 99;//还有充电状态,不能显示 100%
	}
	else
	{
		soc_percent = surplus_percent;
	}
	
	if(((surplus_percent%5) == 0) && (surplus_percent_before != surplus_percent))
	{
		surplus_percent_before = surplus_percent;
		if( surplus_percent <= 20 || surplus_percent >= 80)
		{
			Flash_Eeprom_Init();
			//Flash_Eeprom_Write_Data(SOC_eeprom_surplus,surplus_percent);//不要了,每次复位时都是查表
			
			if(write_soc_flag == 1)
			{//这个IF一生只做一次，除非从新擦擦EEPROM
					write_soc_flag = 2;
					Flash_Eeprom_Write_Data(SOC_eeprom_flag,write_soc_flag);
			}
			Flash_Eeprom_Lock();
		}
	}
	////////////////////////////////////////////////////////
	/////
	if(system_state.cells_staturate_state == 1 )
	{
		bms_self_flag1.soc_static_formort = 1;
		soc_static_formort = 0;
	}
	if((bms_self_flag1.soc_static_formort == 1)&& DCH_CURRENT > 100)
	{
		bms_self_flag1.soc_static_go_on = 1;
	}
	if((DCH_CURRENT == 0) || (CHG_CURRENT >100))
	{
		if(bms_self_flag1.soc_static_formort == 1 && bms_self_flag1.soc_static_go_on == 1) //进来只做一 次 操作 就不做了了 
		{
			if(( (system_state.cell_one_low_state == 1)&&(max_voltage<2900) ) || \
				 ( system_state.dch_ov_vbat_protect == 1 )
				)
			{
				if(system_flag.soc_mode_10 == 1)	
				{
					if((soc_static_formort < 3600000) || (soc_static_formort > 46000000))
					{
						soc_static_formort = 36000000;
					//是否要做一个错误处理？？？？？？？？
					}
				}
				else
				{
					if((soc_static_formort < 7200000) || (soc_static_formort > 820000000))
					{
						soc_static_formort = 72000000;
					//是否要做一个错误处理？？？？？？？？
					}
				}
					soc_static = soc_static_formort;
					soc_static_ = soc_static/100;
					Flash_Eeprom_Init();
					Flash_Eeprom_Write_Data(SOC_eeprom_static,soc_static>>24);
					Flash_Eeprom_Write_Data(SOC_eeprom_static+1,soc_static>>16);
					Flash_Eeprom_Write_Data(SOC_eeprom_static+2,soc_static>>8);
					Flash_Eeprom_Write_Data(SOC_eeprom_static+3,soc_static);  
					Flash_Eeprom_Lock();
			}
		}
		bms_self_flag1.soc_static_go_on = 0;
		bms_self_flag1.soc_static_formort = 0;
		soc_static_formort = 0;
	}
	
	///////////////////////////////////////////////////////
	
		//////////////////////////////////////////////////////
			//容量循环加一计算方式
			if(soc_percent <= 30						&& \
				 flag.soc_cycle_number == 1
			  )
			{
				soc_cycle_number++;//容量循环加一
				Flash_Eeprom_Init();
				Flash_Eeprom_Write_Data(SOC_CYCLE_number,soc_cycle_number>>8);
				Flash_Eeprom_Write_Data(SOC_CYCLE_number+1,soc_cycle_number);
				Flash_Eeprom_Lock();
				flag.soc_cycle_number = 0;//跟踪标志清0，要饱和后 才能从新置1
			}
			if(soc_percent >= 90)
			{
				flag.soc_cycle_number = 1;
			}
			if(system_state.charge_state == 1)
			{
				flag.soc_cycle_number = 0;//有个bug 放电过程中 如果 出现一次程序复位，则本次容量计算忽略
			}
			//////////////////////////////////////////////////////

}


void Send_Precent(void)
{
			USART_SendData9(USART3, 0x1ee);
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			USART_SendData9(USART3, 0x1ee);
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			
			USART_SendData9(USART3, soc_percent);
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			
			USART_SendData9(USART3, soc_count>>24);
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			USART_SendData9(USART3, soc_count>>16);
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			USART_SendData9(USART3, soc_count>>8);
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			USART_SendData9(USART3, soc_count);
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);

}
int tempxiao = 0;
void Fan_Move(void)
{
	if(((t_n[13]>50) || (t_n[14]>50) || (t_n[15]>50)) && \
		 ((system_state.charge_state == 1)||(system_state.discharge_state == 1)))
		{
			flag.fan_high = 1;
		}
		else if((t_n[13]<45) && (t_n[14]<45) && (t_n[15]<45))
		{
			flag.fan_high = 0;
		}
		if((system_state.t_vbat_low_deep_state	== 1)||(system_state.cell_one_low_state == 1))
		{
			flag.fan_high = 0;
		}
		if(	(system_state.charge_state == 0)			&& \
				(system_state.discharge_state == 0)		&& \
				(t_n[13]<55) && (t_n[14]<55) && (t_n[15]<55)
			)
		{
			flag.fan_high = 0;
		}
		if(flag.fan_high==1)
		{
			FAN_SWITCH_HIGH();
			system_state.fan_switch_on = 1;
		}
		else
		{
			FAN_SWITCH_LOW();	
			system_state.fan_switch_on = 0;
		}
		
		
		
		
/*
		tempxiao = Find_soc_percent(19970);
		tempxiao = Find_soc_percent(19972);
		tempxiao = Find_soc_percent(19973);
		tempxiao = Find_soc_percent(20160);
		tempxiao = Find_soc_percent(20164);
		
		tempxiao = Find_soc_percent(21013);
		tempxiao = Find_soc_percent(21547);
		tempxiao = Find_soc_percent(21549);
		tempxiao = Find_soc_percent(21013);
		
		tempxiao = Find_soc_percent(28480);
		tempxiao = Find_soc_percent(28483);
		tempxiao = Find_soc_percent(28533);
		tempxiao = Find_soc_percent(28933);
*/
}


UINT8 check_adapter_count = 0;
UINT16 heat_current=0;
void HeatWork(void)
{
  if(system_flag.en_charge == 0)
	{
		if(
					flag.forbid_on_chgmos == 1								||  \
					flag.chg_voltage_stop_8s_on  == 1					||  \
					flag.dch_mos_protect == 1									||	\
					receive_cmd.on_chgmos == 0								||	\
					system_state.chg_voltage_stop == 1				|| 	\
					/*system_state.temp_chg_low_protect_statue == 1 	|| \*/
					system_state.temp_chg_high_protect_statue == 1 	|| \
					fault_query.second_protect_fault == 1   				|| \
					system_state.cells_staturate_state == 1 			  || \
					system_state.t_vbat_low_deep_state	== 1				|| \
					system_state.dch_cell_anomaly == 1					|| \
					system_state.cells_over_chg_state == 1			|| \
					system_state.chg_oc_protect_statue == 1			|| \
					system_state.hardware_tb_statue == 1				|| \
					system_state.vbat_ov_protect == 1				    || \
					system_state.sec_ov_protect ==1							|| \
					system_state.chg_frequently_oc == 1
			)
		{
			system_flag.en_heat = 0;	
		}
		else
		{
			if(system_state.temp_chg_low_protect_statue == 1)
			{
					bms_self_flag1.heat_read_go = 1;
					if(system_flag.en_heat == 0)
					{
						CO_HIGH();
						Delay(1000);//10ms
						Dis_adc_Channel();
						ADC_Cmd(ADC1, ENABLE);
						ADC_ChannelCmd(ADC1, adc_channel[0], ENABLE);	
						ADC_SoftwareStartConv(ADC1);
						ADC_SoftwareStartConv(ADC1);
						adc_timeout = 0xffff;
						while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
						{
							adc_timeout--;
							if(adc_timeout < 1)
							{
								flag.adc_timeout = 1;
								break;
							}
						}
						ADC_GetConversionValue(ADC1);
						ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
						ADC_ChannelCmd(ADC1, adc_channel[0], DISABLE);
						ADC_ChannelCmd(ADC1, ADC_Channel_21, ENABLE);
						ADC_SoftwareStartConv(ADC1);
						adc_timeout = 0xffff;
						while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0)
						{
							adc_timeout--;
							if(adc_timeout < 1)
							{
								flag.adc_timeout = 1;
								break;
							}
						}
						heat_current = ADC_GetConversionValue(ADC1);
						ADC_ChannelCmd(ADC1, ADC_Channel_21, DISABLE);
						CO_LOW();
						if(heat_current<32) // AD=32  dengtong 0.4A
						{
							system_flag.en_heat = 0;	
						}
						else
						{
							system_flag.en_heat = 1;		
							bms_self_flag1.heat_10fenzhong = 1;
						}
					}
					else
					{
						//temp();
						if((max_temperature < TEMP_HEAT_OFF) && (min_temperature > TEMP_HEAT_TOOLOW))
						{
							if((min_temperature >= TEMP_HEAT_STOP))//&&(system_state.temp_chg_low_protect_statue == 0))
							{
								system_flag.en_heat = 0;		
							}
							else
							{
								if(bms_self_flag1.heat_10fenzhong == 1)
								{
									system_flag.en_heat = 1;	
								}
							  else
								{
									system_flag.en_heat = 0;		
								}
							}
						}
						else
						{
							system_flag.en_heat = 0;		
						}
						if(bms_self_flag1.heat_10fenzhong == 0)
						{
							system_flag.en_heat = 0;		
						}
					}
			}
			else
			{
				system_flag.en_heat = 0;	
			}			
		}
	}
	else
	{
		system_flag.en_heat = 0;	
	}
	
	if(system_flag.en_heat == 1)
	{
		EN_HEAT_HIGH();
		system_state.heat_state = 1;
	}
	else
	{
		EN_HEAT_LOW();
		system_state.heat_state = 0;
	}
}		


#ifdef SYSTEM_24V
UINT16 voltage_persent1[170] = 
{
19972 	,	20053 	,	20107 	,	20160 	,	20213 	,	20267 	,	20320 	,	20373 	,	20427 	,	20480 	, 	//0%   
20533 	,	20587 	,	20640 	,	20693 	,	20747 	,	20800 	,	20853 	,	20907 	,	20960 	,	21013 	,  //0~2%  
21067 	,	21120 	,	21173 	,	21227 	,	21280 	,	21333 	,	21387 	,	21440 	,	21493 	,	21547 	,  //3~5%  
21600 	,	21653 	,	21707 	,	21760 	,	21813 	,	21867 	,	21920 	,	21973 	,	22027 	,	22080 	,  //6~8%  
22133 	,	22187 	,	22240 	,	22293 	,	22347 	,	22400 	,	22453 	,	22507 	,	22560 	,	22613 	,  //9~11% 
22667 	,	22720 	,	22773 	,	22827 	,	22880 	,	22933 	,	22987 	,	23040 	,	23093 	,	23147 	,  //12~15%
23200 	,	23253 	,	23307 	,	23360 	,	23413 	,	23467 	,	23520 	,	23573 	,	23627 	,	23680 	,  //16~20%
23733 	,	23787 	,	23840 	,	23893 	,	23947 	,	24000 	,	24053 	,	24107 	,	24160 	,	24213 	,  //21~25%
24267 	,	24320 	,	24373 	,	24427 	,	24480 	,	24533 	,	24587 	,	24640 	,	24693 	,	24747 	,  //26~36%
24800 	,	24853 	,	24907 	,	24960 	,	25013 	,	25067 	,	25120 	,	25173 	,	25227 	,	25280 	,  //37~46%
25333 	,	25360 	,	25387 	,	25413 	,	25440 	,	25467 	,	25493 	,	25520 	,	25547 	,	25573 	,  //47~54%
25600 	,	25627 	,	25653 	,	25680 	,	25707 	,	25733 	,	25760 	,	25787 	,	25813 	,	25840 	,  //55~65%
25867 	,	25920 	,	25973 	,	26027 	,	26080 	,	26133 	,	26187 	,	26240 	,	26293 	,	26347 	,  //66~76%
26400 	,	26453 	,	26507 	,	26560 	,	26613 	,	26667 	,	26720 	,	26773 	,	26827 	,	26880 	,  //77~87%
26933 	,	26987 	,	27040 	,	27093 	,	27147 	,	27200 	,	27253 	,	27307 	,	27360 	,	27413 	,  //88~93%
27467 	,	27520 	,	27573 	,	27627 	,	27680 	,	27733 	,	27787 	,	27840 	,	27893 	,	27947 	,  //94~99%
28000 	,	28053 	,	28107 	,	28160 	,	28213 	,	28267 	,	28373 	,	28427 	,	28480 	,	28533 		 // 100% 
};
#elif defined SYSTEM_15S
UINT16 voltage_persent1[170] = 
{
						37447,37600,37700,37800,37900,38000,38100,38200,38300,38400,   //0%    
            38500,38600,38700,38800,38900,39000,39100,39200,39300,39400,   //0~2%  
            39500,39600,39700,39800,39900,40000,40100,40200,40300,40400,   //3~5%  
            40500,40600,40700,40800,40900,41000,41100,41200,41300,41400,   //6~8%  
            41500,41600,41700,41800,41900,42000,42100,42200,42300,42400,   //9%~11%
            42500,42600,42700,42800,42900,43000,43100,43200,43300,43400,   //12~15%
            43500,43600,43700,43800,43900,44000,44100,44200,44300,44400,   //16~20%
            44500,44600,44700,44800,44900,45000,45100,45200,45300,45400,   //21~25%
            45500,45600,45700,45800,45900,46000,46100,46200,46300,46400,   //26~36%
            46500,46600,46700,46800,46900,47000,47100,47200,47300,47400,   //37~46%
            47500,47550,47600,47650,47700,47750,47800,47850,47900,47950,   //47~54%
            48000,48050,48100,48150,48200,48250,48300,48350,48400,48450,   //55~65%
            48500,48600,48700,48800,48900,49000,49100,49200,49300,49400,   //66~76%
            49500,49600,49700,49800,49900,50000,50100,50200,50300,50400,   //77~87%
            50500,50600,50700,50800,50900,51000,51100,51200,51300,51400,   //88~93%
            51500,51600,51700,51800,51900,52000,52100,52200,52300,52400,   //94~99%
            52500,52600,52700,52800,52900,53000,53200,53300,53400,53500 	 //100%  
};
#elif defined SYSTEM_16S
UINT16 voltage_persent1[170] = 
{
39944	,	40106	,	40214	,	40320	,	40426	,	40534	,	40640	,	40746	,	40854	,	40960	, 	//0%    
41066	,	41174	,	41280	,	41386	,	41494	,	41600	,	41706	,	41814	,	41920	,	42026	,  //0%~2%	
42134	,	42240	,	42346	,	42454	,	42560	,	42666	,	42774	,	42880	,	42986	,	43094	,  //3%~5%	
43200	,	43306	,	43414	,	43520	,	43626	,	43734	,	43840	,	43946	,	44054	,	44160	,  //6%~8%	
44266	,	44374	,	44480	,	44586	,	44694	,	44800	,	44906	,	45014	,	45120	,	45226	,  //9%~11%	
45334	,	45440	,	45546	,	45654	,	45760	,	45866	,	45974	,	46080	,	46186	,	46294	,  //12~15%	
46400	,	46506	,	46614	,	46720	,	46826	,	46934	,	47040	,	47146	,	47254	,	47360	,  //16~20%	
47466	,	47574	,	47680	,	47786	,	47894	,	48000	,	48106	,	48214	,	48320	,	48426	,  //21~25%	
48534	,	48640	,	48746	,	48854	,	48960	,	49066	,	49174	,	49280	,	49386	,	49494	,  //26~36%	
49600	,	49706	,	49814	,	49920	,	50026	,	50134	,	50240	,	50346	,	50454	,	50560	,  //37~46%	
50666	,	50720	,	50774	,	50826	,	50880	,	50934	,	50986	,	51040	,	51094	,	51146	,  //47~54%	
51200	,	51254	,	51306	,	51360	,	51414	,	51466	,	51520	,	51574	,	51626	,	51680	,  //55~65%	
51734	,	51840	,	51946	,	52054	,	52160	,	52266	,	52374	,	52480	,	52586	,	52694	,  //66~76%	
52800	,	52906	,	53014	,	53120	,	53226	,	53334	,	53440	,	53546	,	53654	,	53760	,  //77~87%	
53866	,	53900 , 53974	,	54010,  54080	,	54130,  54186	,	54230,  54294	,	54340 , 
54400	,	54450,  54506	,	54550,  54614	,	54650,  54720	,	54826	, 54934	, 55000 ,//88~93%	
55040	,	55146	,	55254	,	55360	,	55466	,	55574	,	55680	,	55786	,	55894	, 56000  //94~99%	

#if 0	
39944	,	40106	,	40214	,	40320	,	40426	,	40534	,	40640	,	40746	,	40854	,	40960	, 	//0%    
41066	,	41174	,	41280	,	41386	,	41494	,	41600	,	41706	,	41814	,	41920	,	42026	,  //0%~2%	
42134	,	42240	,	42346	,	42454	,	42560	,	42666	,	42774	,	42880	,	42986	,	43094	,  //3%~5%	
43200	,	43306	,	43414	,	43520	,	43626	,	43734	,	43840	,	43946	,	44054	,	44160	,  //6%~8%	
44266	,	44374	,	44480	,	44586	,	44694	,	44800	,	44906	,	45014	,	45120	,	45226	,  //9%~11%	
45334	,	45440	,	45546	,	45654	,	45760	,	45866	,	45974	,	46080	,	46186	,	46294	,  //12~15%	
46400	,	46506	,	46614	,	46720	,	46826	,	46934	,	47040	,	47146	,	47254	,	47360	,  //16~20%	
47466	,	47574	,	47680	,	47786	,	47894	,	48000	,	48106	,	48214	,	48320	,	48426	,  //21~25%	
48534	,	48640	,	48746	,	48854	,	48960	,	49066	,	49174	,	49280	,	49386	,	49494	,  //26~36%	
49600	,	49706	,	49814	,	49920	,	50026	,	50134	,	50240	,	50346	,	50454	,	50560	,  //37~46%	
50666	,	50720	,	50774	,	50826	,	50880	,	50934	,	50986	,	51040	,	51094	,	51146	,  //47~54%	
51200	,	51254	,	51306	,	51360	,	51414	,	51466	,	51520	,	51574	,	51626	,	51680	,  //55~65%	
51734	,	51840	,	51946	,	52054	,	52160	,	52266	,	52374	,	52480	,	52586	,	52694	,  //66~76%	
52800	,	52906	,	53014	,	53120	,	53226	,	53334	,	53440	,	53546	,	53654	,	53760	,  //77~87%	
53866	,	53974	,	54080	,	54186	,	54294	,	54400	,	54506	,	54614	,	54720	,	54826	,  //88~93%	
54934	,	55040	,	55146	,	55254	,	55360	,	55466	,	55574	,	55680	,	55786	,	55894	,  //94~99%	
56000	,	56106	,	56214	,	56320	,	56426	,	56534	,	56746	,	56854	,	56960	,	57066		// 100%   
#endif
 
};
#endif


UINT8 return_persent[170] = 
{
		0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
		0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
		0	,	0	,	1	,	1	,	1	,	1	,	2	,	2	,	2	,	2	,
		2	,	2	,	2	,	2	,	2	,	3	,	3	,	3	,	3	,	3	,
		3	,	3	,	4	,	4	,	4	,	4	,	5	,	5	,	5	,	5	,
		6	,	6	,	7	,	7	,	8	,	8	,	9	,	9	,	10	,	10	,
		11	,	11	,	12	,	12	,	13	,	13	,	14	,	14	,	15	,	15	,
		16	,	16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,	24	,
		25	,	26	,	27	,	28	,	29	,	30	,	31	,	32	,	33	,	34	,
		35	,	36	,	37	,	38	,	39	,	40	,	41	,	42	,	43	,	44	,
		45	,	46	,	47	,	48	,	49	,	50	,	51	,	52	,	53	,	54	,
		55	,	54	,	53	,	52	,	51	,	50	,	49	,	48	,	47	,	46	,
		65	,	66	,	67	,	68	,	69	,	70	,	71	,	72	,	73	,	74	,
		75	,	76	,	77	,	78	,	79	,	80	,	81	,	82	,	83	,	84	,
		85  , 86  , 87  , 88  , 89  , 90  , 91  , 92  , 93  , 94  ,
		95  , 96  , 97  , 98  , 98  , 99	,	99	,	99	,	99	,	99	,
		100	,	100 ,	100	,	100	,	100	,	100	,	100	,	100	,	100	,	100	
		
		
	
	#if 0
	
	
						0,0,0,0,0,0,0,0,0,0,                     
            0,0,1,1,1,1,2,2,2,2,
            3,3,3,3,4,4,4,5,5,5,
            6,6,6,6,7,7,7,8,8,8,
            9,9,9,9,10,10,10,11,11,11,
            12,12,13,13,13,14,14,15,15,15,
            16,16,17,17,18,18,19,19,20,20,
            21,21,22,22,23,23,24,24,25,25,
            26,27,28,29,30,31,32,33,34,35,
            36,37,38,39,40,41,42,43,44,45,
            46,47,47,48,49,50,51,52,53,54,
            55,56,57,58,59,60,61,62,63,64,
            65,66,67,68,69,70,71,72,73,74,
            75,76,77,78,79,80,81,82,83,84,
            85,86,87,88,89,90,91,92,93,94,
            95,95,96,96,97,97,98,98,99,99,
            100,100,100,100,100,100,100,100,100,100    
   #endif            
};

int hi_ = 169;
int lo_ = 0;
int mi_ = 0; 
int k_ = 0;
union mybool ad_find1;	
int Find_soc_percent(UINT16 voltage)
{
	hi_ = 169;
	lo_ = 0;
	mi_ = 0; 
	//k_ = 0;
	while (hi_ >= lo_)
                {
                   mi_ = (hi_ + lo_) / 2;
                    if (voltage < voltage_persent1[mi_]) hi_ = mi_- 1;//姣旇緝鐨勬槸鏁板�艰�屼笉鏄笅鏍?
                    else if (voltage > voltage_persent1[mi_]) lo_ = mi_+ 1;
                    else
                    {
												k_ = return_persent[mi_] ;// temp_tabe[m];
                        ad_find1.temperatureFind = 1;
                        break;
                    }
                }
              
                if (ad_find1.temperatureFind == 0)
                {
                    if (hi_ < 0)
                    {
                         k_ = 0;
                    }
                    if (hi_ >= 0 && hi_ <= 169)
                    {
                        k_ = return_persent[hi_]  ;//temp_tabe[high + 1];
                    }
                    if (hi_ > 169)
                    {
                     	 k_ = 100;
                    }
                }
                ad_find1.temperatureFind = 0;
               
		return  k_;
}


/*

void Format_SocPercent(int soc_percent)
{
	if(CHG_CURRENT > 50)
	{
		flag.soc_percent_add = 1;
	}
	if(DCH_CURRENT > 50)
	{
		flag.soc_percent_add = 0;
	}
	
	if(odd_soc_percent < soc_percent)
	{
		if(flag.soc_percent_add == 0)
		{
			soc_percent = odd_soc_percent;
		}
		else
		{
			odd_soc_percent = soc_percent;
		}
	}
	
	if(odd_soc_percent >= soc_percent)
	{
		if(flag.soc_percent_add == 0)
		{
			odd_soc_percent = soc_percent;
		}
		else
		{
			odd_soc_percent = soc_percent;
		}
	}
		
	
	
	
	
}
*/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          