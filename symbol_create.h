#ifndef __SYMBOL_CREATE_H
#define __SYMBOL_CREATE_H

#include "main.h"

#define EN_SUPPLY_HIGH()	GPIO_WriteBit(GPIOG,GPIO_Pin_3,SET)//EN_SUPPLY
#define EN_SUPPLY_LOW()	GPIO_WriteBit(GPIOG,GPIO_Pin_G,RESET)//EN_SUPPLY

#define MAIN_SWITCH_HIGH()	GPIO_Init(GPIOF,GPIO_Pin_5,GPIO_Mode_Out_PP_High_Fast)//MAIN_SWITCH
#define MAIN_SWITCH_LOW()		GPIO_Init(GPIOF,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Fast)//MAIN_SWITCH


#define EN_HEAT_HIGH()	GPIO_Init(GPIOE,GPIO_Pin_1,GPIO_Mode_Out_PP_High_Fast)//EN_HEAT
#define EN_HEAT_LOW() 	GPIO_Init(GPIOE,GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Fast)//EN_HEAT  	


#define PRE_CHG_HIGH()	GPIO_Init(GPIOE,GPIO_Pin_0,GPIO_Mode_Out_PP_High_Fast)//PRE_CHG
#define PRE_CHG_LOW()		GPIO_Init(GPIOE,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Fast)//PRE_CHG

#define FAN_SWITCH_HIGH() GPIO_Init(GPIOF,GPIO_Pin_6,GPIO_Mode_Out_PP_High_Fast)//FAN_SWITCH
#define FAN_SWITCH_LOW() 	GPIO_Init(GPIOF,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Fast)//FAN_SWITCH

#define SEC_OV_READ() GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7)//SEC_OV 读其状态


//如果是15串的电芯电池组，则要选择 #define SYSTEM_15S
//如果是16串的电芯电池组，则要选择 #define SYSTEM_16S
//
//#define SYSTEM_15S
#define SYSTEM_16S
//#define SYSTEM_24V  //不可直接使用,需要根据原理图修改adc_v()函数

#define NEW_CURRENT_FORMAORT //保险丝在电流检测后端时需要 开启
	//#define CHG_5BEI //充电电流检测放大倍数是5倍
#define CHG_10BEI   //充电电流检测放大倍数是10倍
	//#define CHG_33BEI//充电电流检测放大倍数是33倍
#define BMS_CHG_DG // you ge dage chg
	//#define SYSTEM_TEMP_TEST


#define ADCLINE 							//电压多点线性计算
#define BMS_2U
#ifdef BMS_2U
 #define jiaozhun_current_kk  	//统一的一块新板中电流计算需要加调整计算 并且电芯热敏电阻只有8颗哦- 4u 的不要调整 可以注释掉本行
#endif

//#define HEAT_USER            //是否启用加热膜
//#define BMS_NO_current



//电芯保护参数
#ifndef SYSTEM_24V
	#define CHG_VOLTAGE_STOP 			3600  //停止充电 电压
	#define CHG_VOLTAGE_RESTOP		3550  // 停止解除	
	#define CHG_SATIATION_MAX 		3550  //充电饱和 最高，在这个区间表示电池组充满电
	#define CHG_SATIATION_MIN			3500  //充电饱和 最低
	#define CHG_RECHG							3400  //可重新充电电压---饱和解除
	#define CHG_MIN_OV_VOLTAGE 		3520  //最小电芯电压大于这个值可关充电
	#define CHG_MIN_OV_REVOLTAGE 	3500  //最小电芯电压小于这个值可开充电
	
	#define DCH_OV_PROTECT 				2500  //过放 保护电压
	#define DCH_OV_RE							3000  //过放 恢复最低电压
	#define DCH_OV_LOW_ANOMALY		1500  //电芯电压异常，将禁止充放电
	#define DCH_OV_HIGH_ANOMALY		4500  //电芯电压异常，将禁止充放电
	#define DCH_0V_VBAT_DEEP      28000 //深度过放 ，将禁止充放电
	
	
	#define CHG_OV_PROTECT 			3850	//过充保护
	#define CHG_OV_RE					  3600	//过充恢复电压  
	#define SEC_OV_MAX 					3900	//二级过充故障检测最大值
	#define SEC_OV_MIN					3800	//二级过充故障检测最小值
	
	#define ALARM_EN						3000	//告警启动
	#define ALARM_RE						3100	//告警恢复（解除）	
	#define VADD_VBAT_SPACE   	3000	//2000  //累加电压与总电压值差

#endif	

#ifdef SYSTEM_24V
	#define CHG_VOLTAGE_STOP 			3600  //停止充电 电压
	#define CHG_VOLTAGE_RESTOP		3550  // 停止解除	
	#define CHG_SATIATION_MAX 		3550  //充电饱和 最高，在这个区间表示电池组充满电
	#define CHG_SATIATION_MIN			3500  //充电饱和 最低
	#define CHG_RECHG							3400  //可重新充电电压---饱和解除
	#define CHG_MIN_OV_VOLTAGE 		3520  //最小电芯电压大于这个值可关充电
	#define CHG_MIN_OV_REVOLTAGE 	3500  //最小电芯电压小于这个值可开充电
	
	#define DCH_OV_PROTECT 				2500  //过放 保护电压
	#define DCH_OV_RE							3000  //过放 恢复最低电压
	#define DCH_OV_LOW_ANOMALY		1500  //电芯电压异常，将禁止充放电
	#define DCH_OV_HIGH_ANOMALY		4500  //电芯电压异常，将禁止充放电
	#define DCH_0V_VBAT_DEEP      14000  //深度过放 ，将禁止充放电
	
	#define CHG_OV_PROTECT 			3800	//过充保护
	#define CHG_OV_RE					  3600	//过充恢复电压  
	#define SEC_OV_MAX 					3900	//二级过充故障检测最大值
	#define SEC_OV_MIN					3800	//二级过充故障检测最小值
	
	#define ALARM_EN						3000	//告警启动
	#define ALARM_RE						3100	//告警恢复（解除）	
	#define VADD_VBAT_SPACE   	2000  //累加电压与总电压值差
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef SYSTEM_15S
	#define DCH_0V_VBAT_REPROTECT       45000  //电池组欠压保籸RE
	#define DCH_0V_VBAT_PROTECT        	39000  //电池组欠压保护
	#define DCH_0V_VBAT_ALARM        		45000 //电池组欠压告警
	#define DCH_0V_VBAT_REALARM        	46500 //电池组欠压告警jiechu-----------------
	#define CHG_VBAT_OV_PROTECT 				54000	//充电时过冲保护
	#define CHG_VBAT_OV_REPROTECT 			53000	//充电时过冲解除
#endif	
#ifdef SYSTEM_16S
	#define DCH_0V_VBAT_REPROTECT       48000  //电池组欠压保籸RE
	#define DCH_0V_VBAT_PROTECT        	42000  //电池组欠压保护
	#define DCH_0V_VBAT_ALARM        		48000 //电池组欠压告警
	#define DCH_0V_VBAT_REALARM        	49600 //电池组欠压告警jiechu-----------------
	#define CHG_VBAT_OV_PROTECT 				57500	//充电时过冲保护
	#define CHG_VBAT_OV_REPROTECT				56000	//充电时过冲解除
#endif	
#ifdef	SYSTEM_24V
	#define DCH_0V_VBAT_REPROTECT       24000  //电池组欠压保籸RE
	#define DCH_0V_VBAT_PROTECT        	20000  //电池组欠压保护
	#define DCH_0V_VBAT_ALARM        		24000 //电池组欠压告警
	#define DCH_0V_VBAT_REALARM        	24800 //电池组欠压告警jiechu-----------------
	#define CHG_VBAT_OV_PROTECT 				29300	//充电时过冲保护
	#define CHG_VBAT_OV_REPROTECT				28500	//充电时过冲解除
#endif
////////////////////////////////////////////////////////////////////////////////////////////////



//温度保护参数
//充电温度保护时	
#define TEMP_NEP									128 	//大于这个值认为是负数(8 位)
#define TEMP_CHG_LOW_PROTECT			0  		//低温保护
#define TEMP_CHG_LOW_RE						5			//低温保护恢复
#define TEMP_CHG_HIGH_PROTECT 		55		//充电高温保护
#define TEMP_CHG_HIGH_RE					50		//充电高温保护恢复
//预充电温度保护时
#define TEMP_PRE_LOW_PROTECT 			-15		//预充电低温保护
#define TEMP_PRE_LOW_RE			 			-5  	//预充电低温恢复
#define TEMP_PRE_HIGH_PROTECT 		55   	//预充电高温保护
#define TEMP_PRE_HIGH_RE					50//35     //预充电高温恢复
//放电温度保护时
#ifdef SYSTEM_TEMP_TEST
	#define TEMP_DCH_LOW_PROTECT 		-30		//放电低温保护 -20 -30
	#define TEMP_DCH_LOW_RE				  -20 	//放电低温恢复 -15 -20
#else
	#define TEMP_DCH_LOW_PROTECT 		-30		//放电低温保护 -20 -30
	#define TEMP_DCH_LOW_RE				  -20 	//放电低温恢复 -15 -20
#endif

#ifdef SYSTEM_TEMP_TEST
	#define	TEMP_DCH_HIGH_PROTECT 		75	//放电高温保护 65 75
#else
	#define TEMP_DCH_HIGH_PROTECT			75//70
#endif

#define TEMP_DCH_HIGH_RE					60    	//放电高温恢复
#define TEMP_THERMISTER_FAULT			-40        // 热敏电阻故障温度thermister_fault
//MOSFET温度保护
#ifdef SYSTEM_TEMP_TEST
	#define TEMP_MOSFET_PROTECT   		106//90  	 //MOSFET高温保护
#else
	#define TEMP_MOSFET_PROTECT   		106//90
#endif
#define TEMP_MOSFET_RE				70   	 //MOSFET高温恢复
//加热膜温度点
#define TEMP_HEAT_TOOLOW			-39//-5   	//最低电芯温度小于-39时将不使用加热膜
#define TEMP_HEAT_EN					0//-5   	//最低电芯温度小于-5时启动 加热膜
#define TEMP_HEAT_STOP				5		//最低电芯温度大于5时停止加热
#define TEMP_HEAT_OFF					50		//最高电芯温度高于50度时关闭加热 	

//电流保护
//预充电电流保护
#define CHG_PRE_CURRENT_PROTECT (5*10)		//暂时设为 5 A,并在数值上放大10倍以便与求电流函数匹配，保护后 2S后再尝试预充电
//充电过流保护
//#define CHG_CURRENT_PROTECT 	    13000			//(13*10)  //充电过流保护电流13A ,保护后 2S后再尝试充电
//放电过流保护
//#define DCH_CURRENT_PROTECT 			18000 			// (18*10)  //放电过流保护电流18A,保护后 2S后再尝试充电
extern UINT16 CHG_CURRENT_PROTECT ;
extern UINT16 DCH_CURRENT_PROTECT ;
extern UINT8 soc_percent_before;

#define COUNT_COUNT 6
#define PRE_CURRENT_PROTECT   100   //预充电电流大于这个值表示 电池组 压差过大，不能直接开充电
#define DCH_CURRENT_SET 			200   //放电时 电流大于这个才 置 放电标志
#define CHG_CURRENT_SET				200		//充电电流大于这个值才置 充电状态
union _UNION16_system_flag
{
    UINT16 U16;
    UINT8  U8[2];
    struct
    {
    	struct{
			        unsigned en_charge  		: 1;			//可充电标志，1为可充电，0为不可充电
			        unsigned en_discharge   	: 1;			//可放电标志，1为可放电，0为不可放电
			        unsigned en_heat			: 1;			//可加热标志，1为可加热，0为不可加热---这三个参数用于控制是否能充放电，做充电和放电操作都要检查这三个参数
			        unsigned charge_state   	: 1;			//充电状态，  1为正在充电，0为不在充电
			        unsigned discharge_state  : 1;			//放电状态，	1为正在放电，0为不在放电
			        unsigned heat_state	 	: 1;			//加热状态，	1为正在加热，0为不在加热
			        unsigned balance_state	  	: 1;			//均衡状态，	1为至少有一节在开均衡，0为没一节在开均衡
			        unsigned exist_protect_state	: 1;		//模组cunzai保护状态	
						};
			struct{
							unsigned ordinary_alarm: 1;			//一般告警，	1为存在一般告警，0为没有一般告警
							unsigned serious_alarm	  	: 1;	//严重告警，	1为存在严重告警，0为没有严重告警
							unsigned dch_protect_pornt : 1;//放电保护点
							unsigned bit11 : 1;
							unsigned bit12 : 1;
							unsigned bit13 : 1;    
							unsigned soc_mode_10 : 1;//表示 1 = 10AH 模块 
							unsigned soc_mode_20 : 1;
						};
    };
};				//系统标志位

union _UNION32_system_state
{
    UINT32 U32;
    UINT16  U16[2];
	UINT8  U8[4];
    struct
    {
		struct
			{
				unsigned charge_state 				: 1;	//充电状态,		1为正在正常充电，0为没在充电
        unsigned charge_pre_state   			: 1;	//预充电状态，1为预充电开启，0为预充电关闭
        unsigned discharge_state  		 	: 1;	//放电状态，	1为正在放电，0为没有在放电
        unsigned heat_state 					: 1;	//加热状态，	1为加热开启，0为加热关闭
        unsigned hardware_tb_protect_state  	: 1;	//硬件故障保护状态
        unsigned cell_one_low_state   		: 1;	//cell/vbat欠压保护状态
        unsigned t_vbat_low_deep_state	 		: 1;	//cell深度过放状态
        unsigned cell_min_low_alarm_state  	: 1;	//cell、vbat低压告警状态
			}; 
		struct
			{			
        unsigned chg_voltage_stop  : 1;	//CHG_VOLTAGE_STOP 最低最高电压大于停止电压3600
        unsigned cells_staturate_state  		: 1;		//cells充饱和状态
        unsigned temp_chg_low_protect_statue 	: 1;	//充电低温保护状态
        unsigned temp_chg_high_protect_statue 	: 1;	//充电高温保护状态
        unsigned temp_dch_low_protect_statue 	: 1; 	//放电低温保护状态
        unsigned temp_dch_high_protect_statue 	: 1;	//放电高温保护状态    
        unsigned short_protect_statue 		: 1;		//短路保护状态
        unsigned chg_oc_protect_statue 		: 1;  	//充电过流保护状态
			};
		struct
			{			
        unsigned dch_oc_protect_statue  			: 1;		//放电过流保护状态
        unsigned hardware_tb_statue  			: 1;		//硬件故障状态
        unsigned vout_statue  					: 1;		//控制输出状态
        unsigned balance_statue  					: 1;		//均衡状态/ 电压低于3400 不可开均衡
        unsigned dch_cell_anomaly				: 1;		//电芯电压异常		DCH_OV_HIGH_ANOMALY		
        unsigned dch_ov_vbat_protect			: 1;		//电池组欠压保护DCH_0V_VBAT_PROTECT  
        unsigned dch_ov_vbat_alarm				: 1;		//DCH_0V_VBAT_ALARM
				unsigned cells_over_chg_state	 	: 1;		//cell过充状态,软件二级保护,大于 3600也过充        	        	
			};
			
        
			struct
			{	
				unsigned sec_ov_protect  : 1;		//二级过充保护
        unsigned vbat_ov_protect : 1;		//总电压过充 保护
        unsigned chg_frequently_oc : 1;
        unsigned fan_switch_on : 1;
        unsigned chg_lock : 1;   //充电自锁级 
        unsigned bit14 : 1;
        unsigned bit15 : 1; 
				unsigned bit16 : 1; 
			};
		};
};				//系统状态位


union _UNION8_receive_cmd
{
    UINT8 U8;
    
    struct
    {
        unsigned on_heat 			: 1;		//打开加热膜 1为开加热，0为不开加热
        unsigned on_chgmos 		: 1; 		//打开充电MOS（bms允许） 1为开充电MOS，0为没有命令根据自身条件控制
        unsigned off_supply 		: 1;		//关闭主控供电，
        unsigned forbid_dch 		: 1;		//禁止放电，1为禁止放电，0为条件允许时开输出
        unsigned sleep_cmd 		: 1; 		//休眠命令，1为进入休眠，0为无命令
        unsigned adapter_connect 	: 1;		//适配器接入 1为接入，0为无接入
        unsigned on_pre_chgmos : 1;//打开预充电
        unsigned bit7 : 1;
    };
};//接收命令CMD


union _UNION16_fault_query
{
    UINT16 U16;
    UINT8  U8[2];
    struct
    {
		struct{
				unsigned heat_fault  			 : 1;			//加热膜故障
		        unsigned second_protect_fault : 1;			//二级保护故障
		        unsigned cell_samping_fault	 : 1;			//电芯电压采用故障
		        unsigned thermister_fault   	 : 1;			//热敏电阻故障
		        unsigned low15_fault 			 : 1;			//15v电路故障
		        unsigned chg_fault			  	 : 1;			//充电电路故障
		        unsigned dch_fault	  			 : 1;			//放电电路故障
		        unsigned bit7	: 1;			
			};	
		struct{
		        unsigned bit8	: 1;			
		        unsigned bit9  : 1;
		        unsigned bit10 : 1;
		        unsigned bit11 : 1;
		        unsigned bit12 : 1;
		        unsigned bit13 : 1;    
		        unsigned bit14 : 1;
		        unsigned bit15 : 1;
			};
    };
};				//故障查询位

extern UINT8 pre_en_charge;			//预充电标识，1为 开启 预充电
union _UNION32_flag
{
    u32 U32;
		u32 U32_;
		UINT8 U8[8];
    struct
		{
			struct
			{
					unsigned  dch_oc_protect_time : 1;//过流保护启动后开始2s倒计时标志		
					unsigned  chg_oc_protect_time : 1;//
					unsigned  chg_pre_oc_protect_time : 1;//
					unsigned  sec_ov : 1;//保存SEC_OV端口的状态
					unsigned  usart3_receive_address : 1;//
					unsigned  balance_low : 1;//均衡状态/ 电压低于3400 不可开均衡
					unsigned  forbid_on_chgmos : 1;//本地禁止充电标识，1为本地禁止充电
					unsigned  pre_charge_ok : 1;//1为预充电 通过 ,0 为预充电 不通过
					
			};
			struct{
							unsigned  dch_pre_chg_flag : 1;
							unsigned cmd_on_chgmos_updata	: 1;
							unsigned dormancy_on  				: 1;//休眠总开关
							unsigned dormancy_poweron_off : 1;//程序启动时5秒内禁止休眠标志
							unsigned soc_percent_chenge_begin : 1;
							unsigned poweron_soc_first_g 			: 1;    
							unsigned poweron_soc_first  			: 1;//上电获取容量标识 （上电500毫秒时采集到的容量做为初始容量）
							unsigned dch_mos_protect : 1;
				};
				struct{
							unsigned chg_voltage_stop_8s_on	: 1;			
							unsigned sec_ov_bit             : 1;
							unsigned soc_come 					: 1;
							unsigned soc_come_g  				: 1;
							unsigned dch_protect_pornt : 1;
							unsigned soc_cycle_number  : 1; //标记容量循环是否可加一   
							unsigned soc_percent_add : 1;
							unsigned w_soc_static : 1;
				};
				struct{
							unsigned jiaozhun_h :1;
							unsigned jiaozhun_l : 1;
							unsigned current_voltage_chgrepair : 1;//充电 时 的电流补充标识 
							unsigned current_voltage_60s_ok : 1;
							unsigned adc_timeout : 1;
							unsigned bit29 : 1;    
							unsigned fan_high : 1;
							unsigned tim1_dormancy_over : 1;
				};
					struct{
		        unsigned alow_time_countsoc	: 1;			
		        unsigned chgoff_prechg_on  : 1;
		        unsigned bit110 : 1;
		        unsigned bit111 : 1;
		        unsigned bit112 : 1;
		        unsigned bit113 : 1;    
		        unsigned bit114 : 1;
		        unsigned bit115 : 1;
					};
		};
};//接收命令CMD




union _UNION32_bms_self_flag   //只有BMS--TTL通信时才用到这个标志参数
{
    UINT32 U32;
    UINT16  U16[2];
    struct
    {
		struct{
						unsigned bms_buffer_come	: 1;//bms自己通信 数据正式开始			
		        unsigned bms_buffer_receive_ok : 1;
		        unsigned bms_buffer_crc_ok : 1;
		        unsigned bit3 : 1;
		        unsigned bit4 : 1;
		        unsigned bit5 : 1;    
		        unsigned bit6 : 1;
		        unsigned alow_time_countsoc : 1;	
			};	
		struct{
		        unsigned bms_tongxin_dormancy_n	 : 1;	//自己通讯开始标识，在此后的8s 不会进行休眠		
		        unsigned bms_tongxin_dormancy_  : 1;//bms_self_flag.bms_tongxin_dormancy = 1;
		        unsigned bms_tongxin_balance_on : 1;
		        unsigned bms_tongxin_balance_off : 1;
		        unsigned bit12 : 1;
		        unsigned bit13 : 1;    
		        unsigned bit14 : 1;
		        unsigned bit15 : 1;
			};
			struct{
		        unsigned bit18	: 1;			
		        unsigned bit19  : 1;
		        unsigned bit110 : 1;
		        unsigned bit111 : 1;
		        unsigned bit112 : 1;
		        unsigned bit113 : 1;    
		        unsigned bit114 : 1;
		        unsigned bit115 : 1;
			};
			struct{
		        unsigned bms_send_alldata	: 1;			
		        unsigned bit29  : 1;
		        unsigned bit120 : 1;
		        unsigned bit121 : 1;
		        unsigned bit122 : 1;
		        unsigned bit123 : 1;    
		        unsigned bit124 : 1;
		        unsigned u32_reg_count : 1;
			};
    };
};				//故障查询位


union _UNION32_bms_self_flag_1
{
    UINT32 U32;
    UINT16  U16[2];
    struct
    {
		struct{
						unsigned minute_chgoc_count_begin	: 1;
		        unsigned pre_30fenzhong : 1;
		        unsigned first2s_dage : 1;
		        unsigned bit3 : 1;
		        unsigned bit4 : 1;
		        unsigned bit5 : 1;    
		        unsigned bit6 : 1;
		        unsigned bit7 : 1;	
			};	
		struct{
		        unsigned readed_soc_static : 1;
		        unsigned soc_static_go_on : 1;    
		        unsigned soc_static_formort : 1;
		        unsigned disdch_protect_on : 1;
						unsigned check_adapter	 : 1;	
		        unsigned adapter_ok  : 1;
		        unsigned heat_read_go  : 1;//加热膜启动 
		        unsigned heat_10fenzhong : 1;
			};
			struct{
		        unsigned bit18	: 1;			
		        unsigned pre_10fenzhong  : 1;//10分钟的倒计时定 器启动
		        unsigned cells_over_chg_prejishi : 1;//统计10分 钟内 过压次数标志 
		        unsigned chg_oc_protect_perjishi : 1;//统计10分 钟内 过流次数标志
		        unsigned chg_oc_protect_offpre : 1;
		        unsigned co_disprechg : 1;    
		        unsigned off_chgmos : 1;
		        unsigned on_pre_chgmos : 1;
			};
			struct{
		        unsigned bit28	: 1;			
		        unsigned bit29  : 1;
		        unsigned chg_tongxin_link : 1;//not usering
		        unsigned temp_h_off_prechg : 1;
		        unsigned temp_l_off_prechg : 1;
		        unsigned t_vbat_off_prechg : 1;    
		        unsigned staturate_off_prechg : 1;
		        unsigned bit31 : 1;
			};
    };
};				



extern union _UNION16_system_flag system_flag;//系统标志位
extern union _UNION32_system_state system_state;//系统状态位
extern union _UNION8_receive_cmd receive_cmd;//接收命令CMD
extern union _UNION16_fault_query fault_query;//故障查询位
extern union _UNION32_flag flag;
extern union _UNION32_bms_self_flag bms_self_flag;
extern union _UNION32_bms_self_flag_1 bms_self_flag1;

extern UINT16 DCH_CURRENT ;
extern UINT16 CHG_CURRENT ; 
extern UINT16 CHG_CURRENT_COUNT ;	// 充电过流计数，连续 3 次判断才认为真正过流
extern UINT16 DCH_CURRENT_COUNT ;	// 放电过流计数，连续 3 次判断才认为真正过流
extern UINT16 deep_state_count  ;	//深度过放 计数，连续 6 次判断才人为真正深度过放
extern UINT8 chg_fault_count;
extern UINT8 samping_fault_count;
extern UINT8 cell_one_low_count;
extern UINT8 cell_one_low_re_count;
extern UINT8 cell_vbat_low_alarm_count;
extern UINT8 dch_ov_vbat_realarm_count;
extern UINT8 dch_ov_vbat_protect_count;
extern UINT8 dch_ov_vbat_reprotect_count;
extern UINT8 cell_one_low_alarm_state_count;
extern UINT8 cells_over_chg_state_count; 
extern UINT8 cells_staturate_state_count;
extern UINT8 temp_chg_low_protect_statue_count;
extern UINT8 temp_chg_high_protect_statue_count;
extern UINT8 temp_dch_low_protect_statue_count;
extern UINT8 temp_dch_high_protect_statue_count;
extern UINT8 cell_vbat_low_realarm_state_count;
extern UINT8 cells_over_chg_restate_count;
extern UINT8 temp_chg_low_protect_restatue_count;
extern UINT8 temp_dch_high_protect_restatue_count;//解除放电高温保护状态jishu
extern UINT8 temp_dch_low_protect_restatue_count;
extern UINT8 temp_chg_high_protect_restatue_count;
extern UINT8 short_protect_restatue_count ;//短路保护解除
extern UINT8 short_protect_statue_count ;
extern UINT8 thermister_fault_count ;
extern UINT8 b_lowest_gate_count ;
extern UINT8 chg_voltage_stop_count ;
extern UINT8 chg_voltage_restop_count ;
extern UINT8 hardware_tb_statue_count;
extern UINT8 vbat_ov_protect_count;
extern UINT8 chg_min_ov_voltage_count;
extern UINT8 chg_min_ov_revoltage_count;
extern UINT8 mos_protect_count;
extern UINT8 mos_reprotect_count;


extern  int soc_percent ;
extern int odd_soc_percent;
extern int soc_cycle_number;
extern INT16 find_tabe[];
extern INT8 return_tabe[];
extern INT16 fist_tabe[] ;
extern INT8 return_fist[];
extern soc_cycle_number;

extern u32 soc_static_formort;
extern u32 soc_static;
extern u32 soc_static_ ;

extern s32 surplus_soc ;
extern s32 surplus_soc_ ;
extern int surplus_percent;
extern int surplus_percent_ ;
extern UINT8 write_soc_flag ;


void Port_Common_Init(void);
void CoDoInit(void);
void Symbol_Create(void);
void Soc_Rw(void);

void Soc_formort(void);

void Write_Soc_Static(void);
void Write_Soc_Count(void);
void Read_Soc_eeprom_flag(void);
void Read_Soc_Static(void);
void Read_Soc_Count(void);
int Find_Data(int temp,INT16 find_tabe[],INT8 return_tabe[],UINT8 high);
void Fan_Move(void);
int Find_soc_percent(UINT16 voltage);
void Send_Precent(void);
void HeatWork(void);
#endif                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       