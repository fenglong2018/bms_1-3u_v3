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

#define SEC_OV_READ() GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7)//SEC_OV ����״̬


//�����15���ĵ�о����飬��Ҫѡ�� #define SYSTEM_15S
//�����16���ĵ�о����飬��Ҫѡ�� #define SYSTEM_16S
//
//#define SYSTEM_15S
#define SYSTEM_16S
//#define SYSTEM_24V  //����ֱ��ʹ��,��Ҫ����ԭ��ͼ�޸�adc_v()����

#define NEW_CURRENT_FORMAORT //����˿�ڵ��������ʱ��Ҫ ����
	//#define CHG_5BEI //���������Ŵ�����5��
#define CHG_10BEI   //���������Ŵ�����10��
	//#define CHG_33BEI//���������Ŵ�����33��
#define BMS_CHG_DG // you ge dage chg
	//#define SYSTEM_TEMP_TEST


#define ADCLINE 							//��ѹ������Լ���
#define BMS_2U
#ifdef BMS_2U
 #define jiaozhun_current_kk  	//ͳһ��һ���°��е���������Ҫ�ӵ������� ���ҵ�о��������ֻ��8��Ŷ- 4u �Ĳ�Ҫ���� ����ע�͵�����
#endif

//#define HEAT_USER            //�Ƿ����ü���Ĥ
//#define BMS_NO_current



//��о��������
#ifndef SYSTEM_24V
	#define CHG_VOLTAGE_STOP 			3600  //ֹͣ��� ��ѹ
	#define CHG_VOLTAGE_RESTOP		3550  // ֹͣ���	
	#define CHG_SATIATION_MAX 		3550  //��籥�� ��ߣ�����������ʾ����������
	#define CHG_SATIATION_MIN			3500  //��籥�� ���
	#define CHG_RECHG							3400  //�����³���ѹ---���ͽ��
	#define CHG_MIN_OV_VOLTAGE 		3520  //��С��о��ѹ�������ֵ�ɹس��
	#define CHG_MIN_OV_REVOLTAGE 	3500  //��С��о��ѹС�����ֵ�ɿ����
	
	#define DCH_OV_PROTECT 				2500  //���� ������ѹ
	#define DCH_OV_RE							3000  //���� �ָ���͵�ѹ
	#define DCH_OV_LOW_ANOMALY		1500  //��о��ѹ�쳣������ֹ��ŵ�
	#define DCH_OV_HIGH_ANOMALY		4500  //��о��ѹ�쳣������ֹ��ŵ�
	#define DCH_0V_VBAT_DEEP      28000 //��ȹ��� ������ֹ��ŵ�
	
	
	#define CHG_OV_PROTECT 			3850	//���䱣��
	#define CHG_OV_RE					  3600	//����ָ���ѹ  
	#define SEC_OV_MAX 					3900	//����������ϼ�����ֵ
	#define SEC_OV_MIN					3800	//����������ϼ����Сֵ
	
	#define ALARM_EN						3000	//�澯����
	#define ALARM_RE						3100	//�澯�ָ��������	
	#define VADD_VBAT_SPACE   	3000	//2000  //�ۼӵ�ѹ���ܵ�ѹֵ��

#endif	

#ifdef SYSTEM_24V
	#define CHG_VOLTAGE_STOP 			3600  //ֹͣ��� ��ѹ
	#define CHG_VOLTAGE_RESTOP		3550  // ֹͣ���	
	#define CHG_SATIATION_MAX 		3550  //��籥�� ��ߣ�����������ʾ����������
	#define CHG_SATIATION_MIN			3500  //��籥�� ���
	#define CHG_RECHG							3400  //�����³���ѹ---���ͽ��
	#define CHG_MIN_OV_VOLTAGE 		3520  //��С��о��ѹ�������ֵ�ɹس��
	#define CHG_MIN_OV_REVOLTAGE 	3500  //��С��о��ѹС�����ֵ�ɿ����
	
	#define DCH_OV_PROTECT 				2500  //���� ������ѹ
	#define DCH_OV_RE							3000  //���� �ָ���͵�ѹ
	#define DCH_OV_LOW_ANOMALY		1500  //��о��ѹ�쳣������ֹ��ŵ�
	#define DCH_OV_HIGH_ANOMALY		4500  //��о��ѹ�쳣������ֹ��ŵ�
	#define DCH_0V_VBAT_DEEP      14000  //��ȹ��� ������ֹ��ŵ�
	
	#define CHG_OV_PROTECT 			3800	//���䱣��
	#define CHG_OV_RE					  3600	//����ָ���ѹ  
	#define SEC_OV_MAX 					3900	//����������ϼ�����ֵ
	#define SEC_OV_MIN					3800	//����������ϼ����Сֵ
	
	#define ALARM_EN						3000	//�澯����
	#define ALARM_RE						3100	//�澯�ָ��������	
	#define VADD_VBAT_SPACE   	2000  //�ۼӵ�ѹ���ܵ�ѹֵ��
#endif

//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef SYSTEM_15S
	#define DCH_0V_VBAT_REPROTECT       45000  //�����Ƿѹ���rRE
	#define DCH_0V_VBAT_PROTECT        	39000  //�����Ƿѹ����
	#define DCH_0V_VBAT_ALARM        		45000 //�����Ƿѹ�澯
	#define DCH_0V_VBAT_REALARM        	46500 //�����Ƿѹ�澯jiechu-----------------
	#define CHG_VBAT_OV_PROTECT 				54000	//���ʱ���屣��
	#define CHG_VBAT_OV_REPROTECT 			53000	//���ʱ������
#endif	
#ifdef SYSTEM_16S
	#define DCH_0V_VBAT_REPROTECT       48000  //�����Ƿѹ���rRE
	#define DCH_0V_VBAT_PROTECT        	42000  //�����Ƿѹ����
	#define DCH_0V_VBAT_ALARM        		48000 //�����Ƿѹ�澯
	#define DCH_0V_VBAT_REALARM        	49600 //�����Ƿѹ�澯jiechu-----------------
	#define CHG_VBAT_OV_PROTECT 				57500	//���ʱ���屣��
	#define CHG_VBAT_OV_REPROTECT				56000	//���ʱ������
#endif	
#ifdef	SYSTEM_24V
	#define DCH_0V_VBAT_REPROTECT       24000  //�����Ƿѹ���rRE
	#define DCH_0V_VBAT_PROTECT        	20000  //�����Ƿѹ����
	#define DCH_0V_VBAT_ALARM        		24000 //�����Ƿѹ�澯
	#define DCH_0V_VBAT_REALARM        	24800 //�����Ƿѹ�澯jiechu-----------------
	#define CHG_VBAT_OV_PROTECT 				29300	//���ʱ���屣��
	#define CHG_VBAT_OV_REPROTECT				28500	//���ʱ������
#endif
////////////////////////////////////////////////////////////////////////////////////////////////



//�¶ȱ�������
//����¶ȱ���ʱ	
#define TEMP_NEP									128 	//�������ֵ��Ϊ�Ǹ���(8 λ)
#define TEMP_CHG_LOW_PROTECT			0  		//���±���
#define TEMP_CHG_LOW_RE						5			//���±����ָ�
#define TEMP_CHG_HIGH_PROTECT 		55		//�����±���
#define TEMP_CHG_HIGH_RE					50		//�����±����ָ�
//Ԥ����¶ȱ���ʱ
#define TEMP_PRE_LOW_PROTECT 			-15		//Ԥ�����±���
#define TEMP_PRE_LOW_RE			 			-5  	//Ԥ�����»ָ�
#define TEMP_PRE_HIGH_PROTECT 		55   	//Ԥ�����±���
#define TEMP_PRE_HIGH_RE					50//35     //Ԥ�����»ָ�
//�ŵ��¶ȱ���ʱ
#ifdef SYSTEM_TEMP_TEST
	#define TEMP_DCH_LOW_PROTECT 		-30		//�ŵ���±��� -20 -30
	#define TEMP_DCH_LOW_RE				  -20 	//�ŵ���»ָ� -15 -20
#else
	#define TEMP_DCH_LOW_PROTECT 		-30		//�ŵ���±��� -20 -30
	#define TEMP_DCH_LOW_RE				  -20 	//�ŵ���»ָ� -15 -20
#endif

#ifdef SYSTEM_TEMP_TEST
	#define	TEMP_DCH_HIGH_PROTECT 		75	//�ŵ���±��� 65 75
#else
	#define TEMP_DCH_HIGH_PROTECT			75//70
#endif

#define TEMP_DCH_HIGH_RE					60    	//�ŵ���»ָ�
#define TEMP_THERMISTER_FAULT			-40        // ������������¶�thermister_fault
//MOSFET�¶ȱ���
#ifdef SYSTEM_TEMP_TEST
	#define TEMP_MOSFET_PROTECT   		106//90  	 //MOSFET���±���
#else
	#define TEMP_MOSFET_PROTECT   		106//90
#endif
#define TEMP_MOSFET_RE				70   	 //MOSFET���»ָ�
//����Ĥ�¶ȵ�
#define TEMP_HEAT_TOOLOW			-39//-5   	//��͵�о�¶�С��-39ʱ����ʹ�ü���Ĥ
#define TEMP_HEAT_EN					0//-5   	//��͵�о�¶�С��-5ʱ���� ����Ĥ
#define TEMP_HEAT_STOP				5		//��͵�о�¶ȴ���5ʱֹͣ����
#define TEMP_HEAT_OFF					50		//��ߵ�о�¶ȸ���50��ʱ�رռ��� 	

//��������
//Ԥ����������
#define CHG_PRE_CURRENT_PROTECT (5*10)		//��ʱ��Ϊ 5 A,������ֵ�ϷŴ�10���Ա������������ƥ�䣬������ 2S���ٳ���Ԥ���
//����������
//#define CHG_CURRENT_PROTECT 	    13000			//(13*10)  //��������������13A ,������ 2S���ٳ��Գ��
//�ŵ��������
//#define DCH_CURRENT_PROTECT 			18000 			// (18*10)  //�ŵ������������18A,������ 2S���ٳ��Գ��
extern UINT16 CHG_CURRENT_PROTECT ;
extern UINT16 DCH_CURRENT_PROTECT ;
extern UINT8 soc_percent_before;

#define COUNT_COUNT 6
#define PRE_CURRENT_PROTECT   100   //Ԥ�������������ֵ��ʾ ����� ѹ����󣬲���ֱ�ӿ����
#define DCH_CURRENT_SET 			200   //�ŵ�ʱ ������������� �� �ŵ��־
#define CHG_CURRENT_SET				200		//�������������ֵ���� ���״̬
union _UNION16_system_flag
{
    UINT16 U16;
    UINT8  U8[2];
    struct
    {
    	struct{
			        unsigned en_charge  		: 1;			//�ɳ���־��1Ϊ�ɳ�磬0Ϊ���ɳ��
			        unsigned en_discharge   	: 1;			//�ɷŵ��־��1Ϊ�ɷŵ磬0Ϊ���ɷŵ�
			        unsigned en_heat			: 1;			//�ɼ��ȱ�־��1Ϊ�ɼ��ȣ�0Ϊ���ɼ���---�������������ڿ����Ƿ��ܳ�ŵ磬�����ͷŵ������Ҫ�������������
			        unsigned charge_state   	: 1;			//���״̬��  1Ϊ���ڳ�磬0Ϊ���ڳ��
			        unsigned discharge_state  : 1;			//�ŵ�״̬��	1Ϊ���ڷŵ磬0Ϊ���ڷŵ�
			        unsigned heat_state	 	: 1;			//����״̬��	1Ϊ���ڼ��ȣ�0Ϊ���ڼ���
			        unsigned balance_state	  	: 1;			//����״̬��	1Ϊ������һ���ڿ����⣬0Ϊûһ���ڿ�����
			        unsigned exist_protect_state	: 1;		//ģ��cunzai����״̬	
						};
			struct{
							unsigned ordinary_alarm: 1;			//һ��澯��	1Ϊ����һ��澯��0Ϊû��һ��澯
							unsigned serious_alarm	  	: 1;	//���ظ澯��	1Ϊ�������ظ澯��0Ϊû�����ظ澯
							unsigned dch_protect_pornt : 1;//�ŵ籣����
							unsigned bit11 : 1;
							unsigned bit12 : 1;
							unsigned bit13 : 1;    
							unsigned soc_mode_10 : 1;//��ʾ 1 = 10AH ģ�� 
							unsigned soc_mode_20 : 1;
						};
    };
};				//ϵͳ��־λ

union _UNION32_system_state
{
    UINT32 U32;
    UINT16  U16[2];
	UINT8  U8[4];
    struct
    {
		struct
			{
				unsigned charge_state 				: 1;	//���״̬,		1Ϊ����������磬0Ϊû�ڳ��
        unsigned charge_pre_state   			: 1;	//Ԥ���״̬��1ΪԤ��翪����0ΪԤ���ر�
        unsigned discharge_state  		 	: 1;	//�ŵ�״̬��	1Ϊ���ڷŵ磬0Ϊû���ڷŵ�
        unsigned heat_state 					: 1;	//����״̬��	1Ϊ���ȿ�����0Ϊ���ȹر�
        unsigned hardware_tb_protect_state  	: 1;	//Ӳ�����ϱ���״̬
        unsigned cell_one_low_state   		: 1;	//cell/vbatǷѹ����״̬
        unsigned t_vbat_low_deep_state	 		: 1;	//cell��ȹ���״̬
        unsigned cell_min_low_alarm_state  	: 1;	//cell��vbat��ѹ�澯״̬
			}; 
		struct
			{			
        unsigned chg_voltage_stop  : 1;	//CHG_VOLTAGE_STOP �����ߵ�ѹ����ֹͣ��ѹ3600
        unsigned cells_staturate_state  		: 1;		//cells�䱥��״̬
        unsigned temp_chg_low_protect_statue 	: 1;	//�����±���״̬
        unsigned temp_chg_high_protect_statue 	: 1;	//�����±���״̬
        unsigned temp_dch_low_protect_statue 	: 1; 	//�ŵ���±���״̬
        unsigned temp_dch_high_protect_statue 	: 1;	//�ŵ���±���״̬    
        unsigned short_protect_statue 		: 1;		//��·����״̬
        unsigned chg_oc_protect_statue 		: 1;  	//����������״̬
			};
		struct
			{			
        unsigned dch_oc_protect_statue  			: 1;		//�ŵ��������״̬
        unsigned hardware_tb_statue  			: 1;		//Ӳ������״̬
        unsigned vout_statue  					: 1;		//�������״̬
        unsigned balance_statue  					: 1;		//����״̬/ ��ѹ����3400 ���ɿ�����
        unsigned dch_cell_anomaly				: 1;		//��о��ѹ�쳣		DCH_OV_HIGH_ANOMALY		
        unsigned dch_ov_vbat_protect			: 1;		//�����Ƿѹ����DCH_0V_VBAT_PROTECT  
        unsigned dch_ov_vbat_alarm				: 1;		//DCH_0V_VBAT_ALARM
				unsigned cells_over_chg_state	 	: 1;		//cell����״̬,�����������,���� 3600Ҳ����        	        	
			};
			
        
			struct
			{	
				unsigned sec_ov_protect  : 1;		//�������䱣��
        unsigned vbat_ov_protect : 1;		//�ܵ�ѹ���� ����
        unsigned chg_frequently_oc : 1;
        unsigned fan_switch_on : 1;
        unsigned chg_lock : 1;   //��������� 
        unsigned bit14 : 1;
        unsigned bit15 : 1; 
				unsigned bit16 : 1; 
			};
		};
};				//ϵͳ״̬λ


union _UNION8_receive_cmd
{
    UINT8 U8;
    
    struct
    {
        unsigned on_heat 			: 1;		//�򿪼���Ĥ 1Ϊ�����ȣ�0Ϊ��������
        unsigned on_chgmos 		: 1; 		//�򿪳��MOS��bms���� 1Ϊ�����MOS��0Ϊû���������������������
        unsigned off_supply 		: 1;		//�ر����ع��磬
        unsigned forbid_dch 		: 1;		//��ֹ�ŵ磬1Ϊ��ֹ�ŵ磬0Ϊ��������ʱ�����
        unsigned sleep_cmd 		: 1; 		//�������1Ϊ�������ߣ�0Ϊ������
        unsigned adapter_connect 	: 1;		//���������� 1Ϊ���룬0Ϊ�޽���
        unsigned on_pre_chgmos : 1;//��Ԥ���
        unsigned bit7 : 1;
    };
};//��������CMD


union _UNION16_fault_query
{
    UINT16 U16;
    UINT8  U8[2];
    struct
    {
		struct{
				unsigned heat_fault  			 : 1;			//����Ĥ����
		        unsigned second_protect_fault : 1;			//������������
		        unsigned cell_samping_fault	 : 1;			//��о��ѹ���ù���
		        unsigned thermister_fault   	 : 1;			//�����������
		        unsigned low15_fault 			 : 1;			//15v��·����
		        unsigned chg_fault			  	 : 1;			//����·����
		        unsigned dch_fault	  			 : 1;			//�ŵ��·����
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
};				//���ϲ�ѯλ

extern UINT8 pre_en_charge;			//Ԥ����ʶ��1Ϊ ���� Ԥ���
union _UNION32_flag
{
    u32 U32;
		u32 U32_;
		UINT8 U8[8];
    struct
		{
			struct
			{
					unsigned  dch_oc_protect_time : 1;//��������������ʼ2s����ʱ��־		
					unsigned  chg_oc_protect_time : 1;//
					unsigned  chg_pre_oc_protect_time : 1;//
					unsigned  sec_ov : 1;//����SEC_OV�˿ڵ�״̬
					unsigned  usart3_receive_address : 1;//
					unsigned  balance_low : 1;//����״̬/ ��ѹ����3400 ���ɿ�����
					unsigned  forbid_on_chgmos : 1;//���ؽ�ֹ����ʶ��1Ϊ���ؽ�ֹ���
					unsigned  pre_charge_ok : 1;//1ΪԤ��� ͨ�� ,0 ΪԤ��� ��ͨ��
					
			};
			struct{
							unsigned  dch_pre_chg_flag : 1;
							unsigned cmd_on_chgmos_updata	: 1;
							unsigned dormancy_on  				: 1;//�����ܿ���
							unsigned dormancy_poweron_off : 1;//��������ʱ5���ڽ�ֹ���߱�־
							unsigned soc_percent_chenge_begin : 1;
							unsigned poweron_soc_first_g 			: 1;    
							unsigned poweron_soc_first  			: 1;//�ϵ��ȡ������ʶ ���ϵ�500����ʱ�ɼ�����������Ϊ��ʼ������
							unsigned dch_mos_protect : 1;
				};
				struct{
							unsigned chg_voltage_stop_8s_on	: 1;			
							unsigned sec_ov_bit             : 1;
							unsigned soc_come 					: 1;
							unsigned soc_come_g  				: 1;
							unsigned dch_protect_pornt : 1;
							unsigned soc_cycle_number  : 1; //�������ѭ���Ƿ�ɼ�һ   
							unsigned soc_percent_add : 1;
							unsigned w_soc_static : 1;
				};
				struct{
							unsigned jiaozhun_h :1;
							unsigned jiaozhun_l : 1;
							unsigned current_voltage_chgrepair : 1;//��� ʱ �ĵ��������ʶ 
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
};//��������CMD




union _UNION32_bms_self_flag   //ֻ��BMS--TTLͨ��ʱ���õ������־����
{
    UINT32 U32;
    UINT16  U16[2];
    struct
    {
		struct{
						unsigned bms_buffer_come	: 1;//bms�Լ�ͨ�� ������ʽ��ʼ			
		        unsigned bms_buffer_receive_ok : 1;
		        unsigned bms_buffer_crc_ok : 1;
		        unsigned bit3 : 1;
		        unsigned bit4 : 1;
		        unsigned bit5 : 1;    
		        unsigned bit6 : 1;
		        unsigned alow_time_countsoc : 1;	
			};	
		struct{
		        unsigned bms_tongxin_dormancy_n	 : 1;	//�Լ�ͨѶ��ʼ��ʶ���ڴ˺��8s �����������		
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
};				//���ϲ�ѯλ


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
		        unsigned heat_read_go  : 1;//����Ĥ���� 
		        unsigned heat_10fenzhong : 1;
			};
			struct{
		        unsigned bit18	: 1;			
		        unsigned pre_10fenzhong  : 1;//10���ӵĵ���ʱ�� ������
		        unsigned cells_over_chg_prejishi : 1;//ͳ��10�� ���� ��ѹ������־ 
		        unsigned chg_oc_protect_perjishi : 1;//ͳ��10�� ���� ����������־
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



extern union _UNION16_system_flag system_flag;//ϵͳ��־λ
extern union _UNION32_system_state system_state;//ϵͳ״̬λ
extern union _UNION8_receive_cmd receive_cmd;//��������CMD
extern union _UNION16_fault_query fault_query;//���ϲ�ѯλ
extern union _UNION32_flag flag;
extern union _UNION32_bms_self_flag bms_self_flag;
extern union _UNION32_bms_self_flag_1 bms_self_flag1;

extern UINT16 DCH_CURRENT ;
extern UINT16 CHG_CURRENT ; 
extern UINT16 CHG_CURRENT_COUNT ;	// ���������������� 3 ���жϲ���Ϊ��������
extern UINT16 DCH_CURRENT_COUNT ;	// �ŵ�������������� 3 ���жϲ���Ϊ��������
extern UINT16 deep_state_count  ;	//��ȹ��� ���������� 6 ���жϲ���Ϊ������ȹ���
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
extern UINT8 temp_dch_high_protect_restatue_count;//����ŵ���±���״̬jishu
extern UINT8 temp_dch_low_protect_restatue_count;
extern UINT8 temp_chg_high_protect_restatue_count;
extern UINT8 short_protect_restatue_count ;//��·�������
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