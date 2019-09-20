/*


*/
#include "common.h"
#include <string.h>  // strlen()


UINT8 TxBuffer[120] = {0};
UINT8 DATA_TO_TRANSFER = 0;
UINT8 receive_data[10] = {0};
uint16_t Usart_Data_Length = 0;
uint16_t Rx_Bit9 = 0;
uint16_t Tx_Bit9 = 0;
uint16_t Tx_Bit9_temp = 0;
UINT16  Crc_Check = 0;
UINT16 Crc_Data	= 0;
UINT8 receive_count = 0;
UINT8 system_mode_temp;
UINT8  MyAddress = 0X01;
UINT8 RxAddress = 0;
UINT8 RxSocMode = 10;
UINT8 MySocMode = 0;
UINT16 TEST_VOLTAGE = 0;
char soc_mode_10 = 1;
char setaddsoc_0x00_count = 0;
char	setaddsoc_0x00_start = 0;

char setaddsoc_0x01_count = 0;
char	setaddsoc_0x01_start = 0;
char setaddsoc_0x02_count = 0;
char	setaddsoc_0x02_start = 0;

char setaddsoc_0x03_count = 0;
char	setaddsoc_0x03_start = 0;

char setaddsoc_0x04_count = 0;
char	setaddsoc_0x04_start = 0;
char setaddsoc_0x05_count = 0;
char	setaddsoc_0x05_start = 0;
char jiaozhun_i = 0;

void USART3_Config(void)
{
	USART_DeInit(USART3);
  USART_Init(USART3, (uint32_t)USART3_BAUDRATE, USART_WordLength_9b, USART_StopBits_1,
                   USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));//21  22 interrupt做完一次中断注意清标志

  
  USART_ITConfig(USART3,USART_IT_RXNE ,ENABLE);//(USART_IT_TypeDef)| USART_IT_IDLE | USART_IT_ERR | USART_IT_OR)
	ITC_SetSoftwarePriority(TIM3_CC_USART3_RX_IRQn, ITC_PriorityLevel_3);  //中断响应等级2
	
	//USART_WakeUpConfig(USART3,USART_WakeUp_AddressMark);
	//USART_ReceiverWakeUpCmd(USART3,ENABLE);
	USART_Cmd(USART3, ENABLE);
	
}




void DMA_TX_Config(void)
{	

	DMA_DeInit(DMA1_Channel1);
 	DMA_GlobalCmd(ENABLE);
	DMA_Init(USART_DMA_CHANNEL_TX, (uint32_t)TxBuffer, (uint16_t)USART_DR_ADDRESS,
	DATA_TO_TRANSFER, DMA_DIR_MemoryToPeripheral, DMA_Mode_Normal,
	DMA_MemoryIncMode_Inc, DMA_Priority_High, DMA_MemoryDataSize_Byte);
	DMA_ITConfig(USART_DMA_CHANNEL_TX,DMA_ITx_TC,ENABLE);	
	DMA_Cmd(USART_DMA_CHANNEL_TX, ENABLE);		
	ITC_SetSoftwarePriority(DMA1_CHANNEL0_1_IRQn, ITC_PriorityLevel_2);  //中断响应等级2
	
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);/* Disable the USART3 Receive interrupt */
	USART_DMACmd(USART3, USART_DMAReq_TX, ENABLE);
	
	
}



char jiaozhun_pl = 0;
int SOC = 0 ;
UINT8 FZ = 0;
UINT8 FZ_TEMP =0;
UINT8 flag_pre_chg_okbh = 0;
void Format_Buffer(void)
{
			
			//Usart_Data_Length = receive[2];
			//Usart_Data_Length = Usart_Data_Length <<8;
			//Usart_Data_Length |= receive[3];
			//Usart_Data_Length += 3;
			for(cycle_number = 2;cycle_number <= Usart_Data_Length;cycle_number++)
			{
				Crc_Check += receive_data[cycle_number];
			}
		
			if(receive_data[Usart_Data_Length+1] == (Crc_Check>>8)  && \
				receive_data[Usart_Data_Length+2] == Crc_Check && \
				receive_data[0] == MyAddress)
			{
				switch(receive_data[1])
				{
					case 0xa1:		// 常规信息
							{
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xa1;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 0x13; // 有效数据位 19 个字节
								
								TxBuffer[4] = t_vbat >> 8;
								TxBuffer[5] = t_vbat;
								
								TxBuffer[6] = max_voltage >> 8;
								TxBuffer[7] = max_voltage ;
								TxBuffer[8] = min_voltage >> 8;
								TxBuffer[9] = min_voltage ;
								
								//TxBuffer[10] = DCH_CURRENT >> 8;
								//TxBuffer[11] = DCH_CURRENT ;	
								if(CHG_CURRENT >= DCH_CURRENT)
								{
									TxBuffer[10] = CHG_CURRENT >> 8 ;
									TxBuffer[10] |= 0x80; 
									TxBuffer[11] = CHG_CURRENT ;
								}
								else
								{
									TxBuffer[10] = DCH_CURRENT >> 8 ;
									TxBuffer[11] = DCH_CURRENT ;
								}
								
								//TxBuffer[12] = max_temperature;
								//TxBuffer[13] = min_temperature;	
								TxBuffer[12] = max_temperature_send;
								TxBuffer[13] = min_temperature_send;
							
							  ////////////////////////////////////////////////////////////////////////
							
								TxBuffer[14] = soc_percent;
								TxBuffer[15] = soc_cycle_number>>8;				
								TxBuffer[16] = soc_cycle_number;
								
								TxBuffer[17] = system_flag.U16;
								TxBuffer[18] = system_flag.U16>>8;
								
								TxBuffer[19] = system_state.U32;
								TxBuffer[20] = system_state.U32>>8;
								TxBuffer[21] = system_state.U32>>16;
								TxBuffer[22] = system_state.U32>>24;
							
								Crc_Data = Crc(22);  // 对 TxBuffer[2] 到 TxBuffer[20] 的数据进行CRC计算
								TxBuffer[23] = Crc_Data >> 8;
								TxBuffer[24] = Crc_Data;
								TxBuffer[25] = 0xbb;
								DATA_TO_TRANSFER = 26;
								////////////////////////////////////////////////////////////////////////
								#if 0
								TxBuffer[14] = soc_percent;
								TxBuffer[15] = system_flag.U16;
								TxBuffer[16] = system_flag.U16>>8;
								
								TxBuffer[17] = system_state.U32;
								TxBuffer[18] = system_state.U32>>8;
								TxBuffer[19] = system_state.U32>>16;
								TxBuffer[20] = system_state.U32>>24;
								Crc_Data = Crc(20);
								TxBuffer[21] = Crc_Data >> 8;
								TxBuffer[22] = Crc_Data;
								
								TxBuffer[23] = 0xbb;
								DATA_TO_TRANSFER = 24;
								#endif
								
								DMA_TX_Config();
								//Send_To_M(DATA_TO_TRANSFER);
							}
						break;
					case 0xa2:	// 发送电芯电压
							{
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xa2;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 0x2a; // 有效数据位 42 个字节

								cycle_number = 1;
								
								#ifdef SYSTEM_15S
								while(cycle_number < 16)
								{
									TxBuffer[2 + cycle_number * 2] = adc_voltage[cycle_number] >> 8;
									TxBuffer[2 + cycle_number * 2 +1] = adc_voltage[cycle_number];
									
									cycle_number++;
								}
								TxBuffer[2 + 16 * 2] = 0xff;
								TxBuffer[2 + 16 * 2 +1] = 0xff;//在15s中将第16节设置为0xffff
								#else
								while(cycle_number < 17)
								{
									TxBuffer[2 + cycle_number * 2] = adc_voltage[cycle_number] >> 8;
									TxBuffer[2 + cycle_number * 2 +1] = adc_voltage[cycle_number];
									
									cycle_number++;
								}
								#endif
								
								
								
								cycle_number = 0;
								while(cycle_number < 8)
								{
									TxBuffer[36 + cycle_number ] = t_n[cycle_number];						
									cycle_number++;
								}
								cycle_number = 0;
								
								//TxBuffer[40] = fault_query.U16;
								//TxBuffer[41] = fault_query.U16 >> 8;//用5 ，  6 节的温度表示硬件出错的标志用于查看

								
								
								//TxBuffer[44] = balance_output.U16>> 8;
								//TxBuffer[45] = balance_output.U16;
								TxBuffer[44] = 0;
								TxBuffer[45] = 0;
								
								Crc_Data = Crc(45);  // 对 TxBuffer[2] 到 TxBuffer[43] 的数据进行CRC计算
								TxBuffer[46] = Crc_Data >> 8;
								TxBuffer[47] = Crc_Data;
								
								TxBuffer[48] = 0xbb;

								DATA_TO_TRANSFER = 49;
								DMA_TX_Config();
							}
							break;
							
					case 0xa3: // 发送故障查询位
							{
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xa3;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 0x02; // 有效数据位 2 个字节

								TxBuffer[4] = fault_query.U8[1];
								TxBuffer[5] = fault_query.U8[0];
								//TxBuffer[5] = 0x01;//调试用语句 加热膜故障 
								
								Crc_Data = Crc(5);  // 对 TxBuffer[2] 到 TxBuffer[5] 的数据进行CRC计算
								TxBuffer[6] = Crc_Data >> 8;
								TxBuffer[7] = Crc_Data;
								
								TxBuffer[8] = 0xbb;

								DATA_TO_TRANSFER = 9;
								DMA_TX_Config();
							}

						break;
						case 0xaa: // 发送版本信息
							{
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xaa;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 0x06; // 有效数据位 3 个字节

								TxBuffer[4] =  SystemVersionInformation_b[0];//硬件版本信息的高位(3字节表示)
								TxBuffer[5] =  SystemVersionInformation_b[1];
								TxBuffer[6] =  SystemVersionInformation_b[2];
								TxBuffer[7] =  SystemVersionInformation[0];//软件版本信息的高位(3字节表示)
								TxBuffer[8] =  SystemVersionInformation[1];
								TxBuffer[9] =  SystemVersionInformation[2];//版本信息的低位
								
								
								Crc_Data = Crc(9);  // 对 TxBuffer[2] 到 xBuffer[5] 的数据进行CRC计算
								TxBuffer[10] = Crc_Data >> 8;
								TxBuffer[11] = Crc_Data;
								
								TxBuffer[12] = 0xbb;

								DATA_TO_TRANSFER = 13;
								DMA_TX_Config();
							}

						break;
					case 0xb1: // 接收到的 主控的 命令
							{	
								receive_cmd.on_heat = 1;// 什么命令是关闭加热呢
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xb1;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 1; // 有效数据位 1 个字节
								TxBuffer[4] = 0;//表示成功接收
								//Crc_Data = 1 ; //Crc(3);  // 对 TxBuffer[2] 到 TxBuffer[3] 的数据进行CRC计算
								TxBuffer[5] = 0;
								TxBuffer[6] = 1;
								
								TxBuffer[7] = 0xbb;

								DATA_TO_TRANSFER = 8;
								DMA_TX_Config();					
							}
						break;
					case 0xb2: // 接收到的 主控的 命令
							{	
								receive_cmd.on_heat = 0;// 什么命令是关闭加热呢
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xb2;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 0; // 有效数据位 0 个字节
								
								//Crc_Data = 0 ; //Crc(3);  // 对 TxBuffer[2] 到 TxBuffer[3] 的数据进行CRC计算
								TxBuffer[4] = 0;
								TxBuffer[5] = 0;
								
								TxBuffer[6] = 0xbb;

								DATA_TO_TRANSFER = 7;
								DMA_TX_Config();					
							}
						break;
					case 0xb3: // 接收到的 主控的 命令
							{	
								receive_cmd.on_chgmos = 1; // 表示有适配器连接到 才会开CHGMOS 的哦
								flag.cmd_on_chgmos_updata = 1;
								//receive_cmd.on_pre_chgmos = 0; //这条命令 不用了
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xb3;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 1; // 有效数据位 1 个字节
								TxBuffer[4] = 0; //有效数据位  0 表示 接收  接 收 成功
								Crc_Data = 1 ;//Crc(3);  // 对 TxBuffer[2] 到 TxBuffer[3] 的数据进行CRC计算
								TxBuffer[5] = 0;
								TxBuffer[6] = 1;
								
								TxBuffer[7] = 0xbb;

								DATA_TO_TRANSFER = 8;
								DMA_TX_Config();
							}
						break;
					case 0xb4: // 接收到的 主控的 命令
							{	
								//receive_cmd.on_pre_chgmos = 1;
								receive_cmd.on_chgmos = 0; // 表示有适配器连接到 才会开CHGMOS 的哦
								flag.cmd_on_chgmos_updata = 1;
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xb4;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 1; // 有效数据位 1 个字节
								TxBuffer[4] = 0;
								Crc_Data = 1 ;//Crc(3);  // 对 TxBuffer[2] 到 TxBuffer[3] 的数据进行CRC计算
								TxBuffer[5] = 0;
								TxBuffer[6] = 1;
								
								TxBuffer[7] = 0xbb;

								DATA_TO_TRANSFER = 8;
								DMA_TX_Config();
							}
						break;
					case 0xb5: // 接收到的 主控的 命令
							{	
								receive_cmd.forbid_dch = 0;// 不禁止放电  ---  开放电
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xb5;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 1; // 有效数据位 1 个字节
								TxBuffer[4] = 0; // 有效数据位 0 表示 接收  成 功
								
								Crc_Data = 1 ;//Crc(3);  // 对 TxBuffer[2] 到 TxBuffer[3] 的数据进行CRC计算
								TxBuffer[5] = 0;
								TxBuffer[6] = 1;
								
								TxBuffer[7] = 0xbb;

								DATA_TO_TRANSFER = 8;
								DMA_TX_Config();					
							}
						break;
					case 0xb6: // 接收到的 主控的 命令
							{	
								receive_cmd.forbid_dch = 1;
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xb6;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 1; // 有效数据位 1 个字节
								TxBuffer[4] = 0; // 有效数据位 0 表示 接收  成 功
								
								Crc_Data = 1 ;//Crc(3);  // 对 TxBuffer[2] 到 TxBuffer[3] 的数据进行CRC计算
								TxBuffer[5] = 0;
								TxBuffer[6] = 1;
								
								TxBuffer[7] = 0xbb;

								DATA_TO_TRANSFER = 8;
								DMA_TX_Config();						
							}
						break;
					case 0xb7: // 接收到的 主控的 命令
							{	
								receive_cmd.off_supply = 1;
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xb7;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 0; // 有效数据位 0 个字节

								Crc_Data = 0 ;//Crc(3);  // 对 TxBuffer[2] 到 TxBuffer[3] 的数据进行CRC计算
								TxBuffer[4] = 0;
								TxBuffer[5] = 0;
								
								TxBuffer[6] = 0xbb;

								DATA_TO_TRANSFER = 7;
								DMA_TX_Config();					
							}
						break;
					case 0xb8: // 接收到的 主控的 命令
							{	
								receive_cmd.off_supply = 0;
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xb8;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 0; // 有效数据位 0 个字节

								Crc_Data = 0 ;//Crc(3);  // 对 TxBuffer[2] 到 TxBuffer[3] 的数据进行CRC计算
								TxBuffer[4] = 0;
								TxBuffer[5] = 0;
								
								TxBuffer[6] = 0xbb;

								DATA_TO_TRANSFER = 7;
								DMA_TX_Config();					
							}
						break;
					
					case 0xb9: // 接收到的 主控的 命令
							{	
								receive_cmd.sleep_cmd = 1;
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xb9;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 0; // 有效数据位 0 个字节

								Crc_Data = 0 ;//Crc(3);  // 对 TxBuffer[2] 到 TxBuffer[3] 的数据进行CRC计算
								TxBuffer[4] = 0;
								TxBuffer[5] = 0;
								
								TxBuffer[6] = 0xbb;

								DATA_TO_TRANSFER = 7;
								DMA_TX_Config();					
							}
						break;	
					case 0xc1: // 校准

						
						break;	
				
					case 0xFE: // 接收到的 主控的 命令
							{	
								TxBuffer[0] = MyAddress;
								TxBuffer[1] = 0xFE;
								
								TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
								TxBuffer[3] = 1; // 有效数据位 1 个字节
								TxBuffer[4] = 0;//表示成功接收
								//Crc_Data = 1 ; //Crc(3);  // 对 TxBuffer[2] 到 TxBuffer[3] 的数据进行CRC计算
								TxBuffer[5] = 0;
								TxBuffer[6] = 1;
								
								TxBuffer[7] = 0xbb;

								DATA_TO_TRANSFER = 8;
								Flash_Eeprom_Init();
								Flash_Eeprom_Write_Data(SYSTEM_BOOTWait_eeprom,20000>>8);
								Flash_Eeprom_Write_Data(SYSTEM_BOOTWait_eeprom+1,20000);
								Flash_Eeprom_Lock();
								Delay(100);
								DMA_TX_Config();
								Delay(10000);
						    _asm("jp $0x8000");
							}
						break;
					default:
						{	TxBuffer[0] = MyAddress;
							TxBuffer[1] = 0xee; // 表示接收到一个 未识别 命令
							
							TxBuffer[2] = 0;// 发送 16 位数据时 高 8 位在前 低 8 位在后，长度只表示 有效数据的字节长度
							TxBuffer[3] = 0; // 有效数据位 0 个字节

							Crc_Data = 0 ;//Crc(3);  // 对 TxBuffer[2] 到 TxBuffer[3] 的数据进行CRC计算
							TxBuffer[4] = 0;
							TxBuffer[5] = 0;
							
							TxBuffer[6] = 0xbb;

							DATA_TO_TRANSFER = 7;
							DMA_TX_Config();
						}

						break;


				}
			}

}





// interrupt 22
u32 ko_32_reg = 0;//用与使本中断生成 32位 中断保存
@far @interrupt void TIM3_CC_USART3_RX_IRQHandler (void)
{
	//USART_ITConfig(USART3,(USART_IT_TypeDef)(USART_IT_RXNE | USART_IT_IDLE | USART_IT_ERR | USART_IT_OR),DISABLE);
	ko_32_reg++;
	if(ko_32_reg>1024)
	{
		bms_self_flag.u32_reg_count = 1;//在别处 并不使用这条语 句
	}
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  	{
		Rx_Bit9 = USART_ReceiveData9(USART3);
		Bms_self_receive();
		if((Rx_Bit9 & 0x100) == 0x100)
		{
			RxAddress = Rx_Bit9;
			if(flag.usart3_receive_address == 0)
			{
				if(RxAddress == MyAddress)
				{
					Tx_Bit9 =0;
					Tx_Bit9 |= 0x100;
					Tx_Bit9 |= MyAddress;
					USART_SendData9(USART3, Tx_Bit9);
					flag.usart3_receive_address = 1;
					usart3_time_200ms =0;
					//g_sys_flags.usart3_address_timeout = 0;
				}
				//**********************************
					
				//SetAddSoc();
						
				//**********************************
    	
			}
		}
		else
		{
			if(flag.usart3_receive_address ==1)
			{
				if(usart3_time_200ms > USART3_TIME_OUT)
				{
					flag.usart3_receive_address =0;
					usart3_time_200ms = 0;
					receive_count = 0;
				}
				else
				{
					receive_data[receive_count++] = Rx_Bit9;
					if(receive_count == 4)
					{
						Usart_Data_Length = receive_data[2];
						Usart_Data_Length = Usart_Data_Length <<8;
						Usart_Data_Length |= receive_data[3];
						Usart_Data_Length += 3;
					}
					if(receive_count == Usart_Data_Length + 4)
					{
						flag.usart3_receive_address =0;
						usart3_time_200ms = 0;
						if(receive_data[receive_count-1] == 0xbb)
						{
							Format_Buffer();
						}
						receive_count = 0;
					}
				}
				
			}
			
		}
	
	}
	//if (USART_GetFlagStatus(USART3, USART_FLAG_OR) != RESET)//注意！不能使用if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)来判断
	//{
		USART_ReceiveData9(USART3);
	//}
	//if (USART_GetITStatus(USART3, USART_IT_OR) != RESET)//注意！不能使用	if (USART_GetFlagStatus(USART3, USART_FLAG_OR) != RESET)来判断
	//{
		USART_ReceiveData9(USART3);
	//}
	USART_ClearITPendingBit(USART3,(USART_IT_TypeDef)(USART_IT_RXNE | USART_IT_IDLE | USART_IT_ERR | USART_IT_OR));
}




void SetAddSoc(void)
{
		//updata_count = 0;
		if((Rx_Bit9 & 0xc0) == 0xc0 )
		{
			if((Rx_Bit9 & 0x30) == 0x10)
			{
				soc_mode_10 = 1;
			}
			else soc_mode_10 = 0;
			
			switch(Rx_Bit9 & 0x0f)
			{	
				case 0x00:
					setaddsoc_0x00_start = 1;
					setaddsoc_0x00_count = 1;
					setaddsoc_0x01_start = 0;
					setaddsoc_0x01_count = 0;
					setaddsoc_0x02_start = 0;
					setaddsoc_0x02_count = 0;
					setaddsoc_0x03_start = 0;
					setaddsoc_0x03_count = 0;
					setaddsoc_0x04_start = 0;
					setaddsoc_0x04_count = 0;
					setaddsoc_0x05_start = 0;
					setaddsoc_0x05_count = 0;
					//receive_data_temp[0] = Rx_Bit9;
					USART_SendData9(USART3, 0x111);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
					goto rec_out;
					break;
				
				case 0x01:
					setaddsoc_0x00_start = 0;
					setaddsoc_0x00_count = 0;
					setaddsoc_0x01_start = 1;
					setaddsoc_0x01_count = 1;
					setaddsoc_0x02_start = 0;
					setaddsoc_0x02_count = 0;
					setaddsoc_0x03_start = 0;
					setaddsoc_0x03_count = 0;
					setaddsoc_0x04_start = 0;
					setaddsoc_0x04_count = 0;
					setaddsoc_0x05_start = 0;
					setaddsoc_0x05_count = 0;
					//receive_data_temp[0] = Rx_Bit9;
					USART_SendData9(USART3, 0x111);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
					goto rec_out; 
					break;
				
				case 0x02:
					setaddsoc_0x00_start = 0;
					setaddsoc_0x00_count = 0;
					setaddsoc_0x01_start = 0;
					setaddsoc_0x01_count = 0;
					setaddsoc_0x02_start = 1;
					setaddsoc_0x02_count = 1;
					setaddsoc_0x03_start = 0;
					setaddsoc_0x03_count = 0;
					setaddsoc_0x04_start = 0;
					setaddsoc_0x04_count = 0;
					setaddsoc_0x05_start = 0;
					setaddsoc_0x05_count = 0;
					//receive_data_temp[0] = Rx_Bit9;
					USART_SendData9(USART3, 0x111);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
					goto rec_out; 
					break;
				case 0x03:
					setaddsoc_0x00_start = 0;
					setaddsoc_0x00_count = 0;
					setaddsoc_0x01_start = 0;
					setaddsoc_0x01_count = 0;
					setaddsoc_0x02_start = 0;
					setaddsoc_0x02_count = 0;
					setaddsoc_0x03_start = 1;
					setaddsoc_0x03_count = 1;
					setaddsoc_0x04_start = 0;
					setaddsoc_0x04_count = 0;
					setaddsoc_0x05_start = 0;
					setaddsoc_0x05_count = 0;
					//receive_data_temp[0] = Rx_Bit9;
					USART_SendData9(USART3, 0x111);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
					goto rec_out; 
				case 0x04:
					setaddsoc_0x00_start = 0;
					setaddsoc_0x00_count = 0;
					setaddsoc_0x01_start = 0;
					setaddsoc_0x01_count = 0;
					setaddsoc_0x02_start = 0;
					setaddsoc_0x02_count = 0;
					setaddsoc_0x03_start = 0;
					setaddsoc_0x03_count = 0;
					setaddsoc_0x04_start = 1;
					setaddsoc_0x04_count = 1;
					setaddsoc_0x05_start = 0;
					setaddsoc_0x05_count = 0;
					//receive_data_temp[0] = Rx_Bit9;
					USART_SendData9(USART3, 0x111);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
					goto rec_out;
					break;
				case 0x05:
					setaddsoc_0x00_start = 0;
					setaddsoc_0x00_count = 0;
					setaddsoc_0x01_start = 0;
					setaddsoc_0x01_count = 0;
					setaddsoc_0x02_start = 0;
					setaddsoc_0x02_count = 0;
					setaddsoc_0x03_start = 0;
					setaddsoc_0x03_count = 0;
					setaddsoc_0x04_start = 0;
					setaddsoc_0x04_count = 0;
					setaddsoc_0x05_start = 1;
					setaddsoc_0x05_count = 1;
					//receive_data_temp[0] = Rx_Bit9;
					USART_SendData9(USART3, 0x111);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
					goto rec_out;
					break;
					//break;
	
			}	
		}
	FormatAddSoc(soc_mode_10);
		
	
rec_out: return;	 
}

void FormatAddSoc(char soc_mode_10)
{
	//----------------------------------------------------------------------------------------
	if(setaddsoc_0x00_start == 1)
	{
		if(Rx_Bit9 == 0x111)
		{
			if(setaddsoc_0x00_count == 1)
			{
				setaddsoc_0x00_count = 2;
				USART_SendData9(USART3, 0x112);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x00_count = 0;
				setaddsoc_0x00_start = 0;
			}
		}
		if(Rx_Bit9 == 0x112)
		{
			if(setaddsoc_0x00_count == 2)
			{
				setaddsoc_0x00_count = 3;
				USART_SendData9(USART3, 0x113);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x00_count = 0;
				setaddsoc_0x00_start = 0;
			}
		}
		if(Rx_Bit9 == 0x113)
		{
			if(setaddsoc_0x00_count == 3)
			{
				setaddsoc_0x00_count = 4;
				USART_SendData9(USART3, 0x114);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x00_count = 0;
				setaddsoc_0x00_start = 0;
			}
		}
		if(Rx_Bit9 == 0x114)
		{
			if(setaddsoc_0x00_count == 4)
			{
				setaddsoc_0x00_count = 5;
				USART_SendData9(USART3, 0x115);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x00_count = 0;
				setaddsoc_0x00_start = 0;
			}
		}
		if(Rx_Bit9 == 0x115)
		{
			if(setaddsoc_0x00_count == 5)
			{
				//********确认操控
				//Read_MyAddress();
				//Read_MySocMode();
				//USART_SendData9(USART3, MyAddress);
				//USART_SendData9(USART3, MySocMode);
				//while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				
				
				system_mode_temp = 0;
				Read_System_Mode();
				USART_SendData9(USART3, 0x100 | system_mode_temp);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				#ifdef SYSTEM_15S
				for(jiaozhun_pl=0;jiaozhun_pl<=15;jiaozhun_pl++)
				#else
				for(jiaozhun_pl=0;jiaozhun_pl<=16;jiaozhun_pl++)
				#endif			
				{
					jiaozhun_l[jiaozhun_pl] = 0;
					jiaozhun_h[jiaozhun_pl] = 0;
				}
				jiaozhun_v_bat_h = 0;
				jiaozhun_v_bat_l = 0;
				
				dch_current_ref = 0;
				chg_current_ref = 0;
				Flash_Eeprom_Init();
				
				Flash_Eeprom_Write_Data(SOC_CYCLE_number,0x00);
				Flash_Eeprom_Write_Data(JIAOZHUN_L_eeprom+1,0x00);
				Delay(5);
				
				Flash_Eeprom_Lock();
				//********************************
			}
		 
			setaddsoc_0x00_count = 0;
			setaddsoc_0x00_start = 0;
			
		}
	}
	else 
	{	
		setaddsoc_0x00_count = 0;
		setaddsoc_0x00_start = 0;
	}
			
	//----------------------------------------------------------------------------------------		
	if(setaddsoc_0x01_start == 1)
	{
		if(Rx_Bit9 == 0x111)
		{
			if(setaddsoc_0x01_count == 1)
			{
				setaddsoc_0x01_count = 2;
				
				USART_SendData9(USART3, 0x112);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				
			}
			else 
			{	setaddsoc_0x01_count = 0;
				setaddsoc_0x01_start = 0;
			}
		}
		if(Rx_Bit9 == 0x112)
		{
			if(setaddsoc_0x01_count == 2)
			{
				setaddsoc_0x01_count = 3;
				
				USART_SendData9(USART3, 0x113);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			
			}
			else 
			{	setaddsoc_0x01_count = 0;
				setaddsoc_0x01_start = 0;
			}
		}
		if(Rx_Bit9 == 0x113)
		{
			if(setaddsoc_0x01_count == 3)
			{
				setaddsoc_0x01_count = 4;
				
				USART_SendData9(USART3, 0x114);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				//Delay(100);
			}
			else 
			{	setaddsoc_0x01_count = 0;
				setaddsoc_0x01_start = 0;
			}
		}
		if(Rx_Bit9 == 0x114)
		{
			if(setaddsoc_0x01_count == 4)
			{
				setaddsoc_0x01_count = 5;
				
				USART_SendData9(USART3, 0x115);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				//Delay(100);
			}
			else 
			{	setaddsoc_0x01_count = 0;
				setaddsoc_0x01_start = 0;
			}
		}
		if(Rx_Bit9 == 0x115)
		{
			if(setaddsoc_0x01_count == 5)
			{
				setaddsoc_0x01_count = 0;
				//********确认操控
				
				if(soc_mode_10 == 1)
				{
					system_mode_temp = 0xd1;
					Write_System_Mode();
					system_mode_temp = 0;
					Read_System_Mode();
					USART_SendData9(USART3, 0x100 | system_mode_temp);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				}
				else 
				{
					system_mode_temp = 0xe1;
					Write_System_Mode();
					system_mode_temp = 0;
					Read_System_Mode();
					USART_SendData9(USART3, 0x100 | system_mode_temp);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				}
				//********************************
			}
			else 
			{	setaddsoc_0x01_count = 0;
				setaddsoc_0x01_start = 0;
			}
	 	}
	
	}
	else 
	{	
		setaddsoc_0x01_count = 0;
		setaddsoc_0x01_start = 0;
	}
	//----------------------------------------------------------------------------------------
	if(setaddsoc_0x02_start == 1)
	{
		if(Rx_Bit9 == 0x111)
		{
			if(setaddsoc_0x02_count == 1)
			{
				setaddsoc_0x02_count = 2;
				USART_SendData9(USART3, 0x112);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x02_count = 0;
				setaddsoc_0x02_start = 0;
			}
		}
		if(Rx_Bit9 == 0x112)
		{
			if(setaddsoc_0x02_count == 2)
			{
				setaddsoc_0x02_count = 3;
				USART_SendData9(USART3, 0x113);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x02_count = 0;
				setaddsoc_0x02_start = 0;
			}
		}
		if(Rx_Bit9 == 0x113)
		{
			if(setaddsoc_0x02_count == 3)
			{
				setaddsoc_0x02_count = 4;
				USART_SendData9(USART3, 0x114);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x02_count = 0;
				setaddsoc_0x02_start = 0;
			}
		}
		if(Rx_Bit9 == 0x114)
		{
			if(setaddsoc_0x02_count == 4)
			{
				setaddsoc_0x02_count = 5;
				USART_SendData9(USART3, 0x115);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x02_count = 0;
				setaddsoc_0x02_start = 0;
			}
		}
		if(Rx_Bit9 == 0x115)
		{
			if(setaddsoc_0x02_count == 5)
			{
				setaddsoc_0x02_count = 0;
				//********确认操控
				
				if(soc_mode_10 == 1)
				{
					system_mode_temp = 0xd2;
					Write_System_Mode();
					system_mode_temp = 0;
					Read_System_Mode();
					USART_SendData9(USART3, 0x100 | system_mode_temp);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				}
				else 
				{
					system_mode_temp = 0xe2;
					Write_System_Mode();
					system_mode_temp = 0;
					Read_System_Mode();
					USART_SendData9(USART3, 0x100 | system_mode_temp);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				}
				//********************************
			}
			else 
			{	setaddsoc_0x02_count = 0;
				setaddsoc_0x02_start = 0;
			}
	 	}
	
	}
	else 
	{	
		setaddsoc_0x02_count = 0;
		setaddsoc_0x02_start = 0;
	}
	//----------------------------------------------------------------------------------------
	if(setaddsoc_0x03_start == 1)
	{
		if(Rx_Bit9 == 0x111)
		{
			if(setaddsoc_0x03_count == 1)
			{
				setaddsoc_0x03_count = 2;
				USART_SendData9(USART3, 0x112);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x03_count = 0;
				setaddsoc_0x03_start = 0;
			}
		}
		if(Rx_Bit9 == 0x112)
		{
			if(setaddsoc_0x03_count == 2)
			{
				setaddsoc_0x03_count = 3;
				USART_SendData9(USART3, 0x113);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x03_count = 0;
				setaddsoc_0x03_start = 0;
			}
		}
		if(Rx_Bit9 == 0x113)
		{
			if(setaddsoc_0x03_count == 3)
			{
				setaddsoc_0x03_count = 4;
				USART_SendData9(USART3, 0x114);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x03_count = 0;
				setaddsoc_0x03_start = 0;
			}
		}
		if(Rx_Bit9 == 0x114)
		{
			if(setaddsoc_0x03_count == 4)
			{
				setaddsoc_0x03_count = 5;
				USART_SendData9(USART3, 0x115);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x03_count = 0;
				setaddsoc_0x03_start = 0;
			}
		}
		if(Rx_Bit9 == 0x115)
		{
			if(setaddsoc_0x03_count == 5)
			{
				setaddsoc_0x03_count = 0;
				//********确认操控
				if(soc_mode_10 == 1)
				{
					system_mode_temp = 0xd3;
					Write_System_Mode();
					system_mode_temp = 0;
					Read_System_Mode();
					USART_SendData9(USART3, 0x100 | system_mode_temp);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				}
				else 
				{
					system_mode_temp = 0xe3;
					Write_System_Mode();
					system_mode_temp = 0;
					Read_System_Mode();
					USART_SendData9(USART3, 0x100 | system_mode_temp);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				}
				//********************************
			}
			else 
			{	setaddsoc_0x03_count = 0;
				setaddsoc_0x03_start = 0;
			}
	 	}
	
	}
	else 
	{	
		setaddsoc_0x03_count = 0;
		setaddsoc_0x03_start = 0;
	}
	//----------------------------------------------------------------------------------------
if(setaddsoc_0x04_start == 1)
	{
		if(Rx_Bit9 == 0x111)
		{
			if(setaddsoc_0x04_count == 1)
			{
				setaddsoc_0x04_count = 2;
				USART_SendData9(USART3, 0x112);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x04_count = 0;
				setaddsoc_0x04_start = 0;
			}
		}
		if(Rx_Bit9 == 0x112)
		{
			if(setaddsoc_0x04_count == 2)
			{
				setaddsoc_0x04_count = 3;
				USART_SendData9(USART3, 0x113);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x04_count = 0;
				setaddsoc_0x04_start = 0;
			}
		}
		if(Rx_Bit9 == 0x113)
		{
			if(setaddsoc_0x04_count == 3)
			{
				setaddsoc_0x04_count = 4;
				USART_SendData9(USART3, 0x114);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x04_count = 0;
				setaddsoc_0x04_start = 0;
			}
		}
		if(Rx_Bit9 == 0x114)
		{
			if(setaddsoc_0x04_count == 4)
			{
				setaddsoc_0x04_count = 5;
				USART_SendData9(USART3, 0x115);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x04_count = 0;
				setaddsoc_0x04_start = 0;
			}
		}
		if(Rx_Bit9 == 0x115)
		{
			if(setaddsoc_0x04_count == 5)
			{
				//********确认操控
				//Read_MyAddress();
				//Read_MySocMode();
				//USART_SendData9(USART3, MyAddress);
				//USART_SendData9(USART3, MySocMode);
				//while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				
				flag.jiaozhun_l = 1;
				//********************************
			}
		
			setaddsoc_0x04_count = 0;
			setaddsoc_0x04_start = 0;
			
		}
	}
	else 
	{	
		setaddsoc_0x04_count = 0;
		setaddsoc_0x04_start = 0;
	}
			
	//----------------------------------------------------------------------------------------
if(setaddsoc_0x05_start == 1)
	{
		if(Rx_Bit9 == 0x111)
		{
			if(setaddsoc_0x05_count == 1)
			{
				setaddsoc_0x05_count = 2;
				USART_SendData9(USART3, 0x112);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x05_count = 0;
				setaddsoc_0x05_start = 0;
			}
		}
		if(Rx_Bit9 == 0x112)
		{
			if(setaddsoc_0x05_count == 2)
			{
				setaddsoc_0x05_count = 3;
				USART_SendData9(USART3, 0x113);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x05_count = 0;
				setaddsoc_0x05_start = 0;
			}
		}
		if(Rx_Bit9 == 0x113)
		{
			if(setaddsoc_0x05_count == 3)
			{
				setaddsoc_0x05_count = 4;
				USART_SendData9(USART3, 0x114);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x05_count = 0;
				setaddsoc_0x05_start = 0;
			}
		}
		if(Rx_Bit9 == 0x114)
		{
			if(setaddsoc_0x05_count == 4)
			{
				setaddsoc_0x05_count = 5;
				USART_SendData9(USART3, 0x115);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}
			else 
			{	setaddsoc_0x05_count = 0;
				setaddsoc_0x05_start = 0;
			}
		}
		if(Rx_Bit9 == 0x115)
		{
			if(setaddsoc_0x05_count == 5)
			{
				//********确认操控
				//Read_MyAddress();
				//Read_MySocMode();
				//USART_SendData9(USART3, MyAddress);
				//USART_SendData9(USART3, MySocMode);
				//while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
				
				flag.jiaozhun_h = 1;
				
			
			
				//********************************
			}
		
			setaddsoc_0x05_count = 0;
			setaddsoc_0x05_start = 0;
			
		}
	}
	else 
	{	
		setaddsoc_0x05_count = 0;
		setaddsoc_0x05_start = 0;
	}
			
	//----------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------

}


void Send_To_M(UINT8 DATA_TO_TRANSFER)
{
	UINT8 i = 0;
	for(i;i < DATA_TO_TRANSFER;i++)
	{
		USART_SendData9(USART3, 0x100 | TxBuffer[i]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
	}


}




UINT16 Crc(UINT16 count)
{
	Crc_Data = 0;
	cycle_number = 2;
	while(cycle_number <= count)
	{
		Crc_Data += TxBuffer[cycle_number];
		cycle_number++;

	}
	cycle_number = 0;
	return Crc_Data;

}
UINT16 Crc_bms(UINT16 count)
{
	Crc_Data = 0;
	cycle_number = 0;
	while(cycle_number <=  count)
	{
		Crc_Data += TxBuffer[cycle_number];
		cycle_number++;

	}
	cycle_number = 0;
	return Crc_Data;

}

//通信转发应答
void USART_ACK(void)
{
	Tx_Bit9 = 0x7f;
	USART_SendData9(USART3, Tx_Bit9);
	Crc_Check = 0;

}
//通信失败返回错误
void USART3_Error(void)
{
	Tx_Bit9 = 0x70;
	USART_SendData9(USART3, Tx_Bit9);
	Crc_Check = 0;
}

//
UINT16 bms_crc_data = 0;
UINT8 bms_tongxin_cycle = 0;
UINT8 bms_buffer[126] = {0};
UINT8 bms_buffer_data = 0;
UINT8 bms_buffer_stair_count = 0;
UINT8 bms_buffer_count = 0;
UINT8 bms_buffer_data_count = 0;
UINT8 data_number = 0;
UINT8 Bms_crc(UINT8 data_number)
{
	bms_tongxin_cycle = 0;
	bms_crc_data = 0;
	for(bms_tongxin_cycle;bms_tongxin_cycle<data_number;bms_tongxin_cycle++)
	{
		bms_crc_data += bms_buffer[bms_tongxin_cycle];	
	}
	return bms_crc_data;
}
void Bms_self_receive(void)
{
	bms_buffer_data = (Rx_Bit9 & 0x0ff);
	
	if(bms_self_flag.bms_buffer_come == 1)
	{
		bms_buffer[bms_buffer_count] = bms_buffer_data;
		if(bms_buffer_count==0)
		{
			bms_buffer_data_count = bms_buffer_data;
		}
		bms_buffer_count++;
		if((bms_buffer_count >= bms_buffer_data_count) || (bms_buffer_count >= 126))
		{
			bms_buffer_count = 0;
			bms_self_flag.bms_buffer_come = 0;
			bms_self_flag.bms_buffer_receive_ok = 1;
			bms_self_flag.bms_tongxin_dormancy_n = 1;
			bms_tongxin_dormancy_time = 0;
			CHG_VOLTAGE_STOP_8S = 150; //设置为2秒的停充倒计时，自通信解除后会从新设置为 90秒
		}
	}
	else
	{
		if((bms_buffer_data == 0xff)&&(bms_self_flag.bms_buffer_come == 0))
		{
			bms_buffer_stair_count++;
			if(bms_buffer_stair_count == 6)
			{
				bms_buffer_stair_count = 0;
				bms_buffer_count = 0;
				bms_self_flag.bms_buffer_come = 1;
			}
		}
		else
		{
			bms_buffer_stair_count = 0;
		}
	
	
	}
	
}
UINT8 bms_buffer_dealwith[126] = {0};
void Bms_tongxin_dealwith(void)
{
	UINT8 cycle = 0;
	if(bms_self_flag.bms_buffer_receive_ok == 1)	
	{
		data_number = bms_buffer[0];
		Bms_crc(data_number-3);
		if((bms_crc_data>>8)==bms_buffer[data_number-3] &&  \
				bms_crc_data == bms_buffer[data_number-2]   &&  \
				0x01 == bms_buffer[data_number-1]						&&  \
				0x01 ==  bms_buffer[1]	
				
			)
		{
			bms_self_flag.bms_buffer_crc_ok = 1;
			cycle = 0;
			for(cycle;cycle<data_number;cycle++)
			{
				bms_buffer_dealwith[cycle] = bms_buffer[cycle];
			}
			
			if((bms_buffer_dealwith[2] & 0x80) == 0x80)  //写 系列指令   
			{
				if((bms_buffer_dealwith[2] & 0x01) == 0x01)//按 地址  写 
				{
					
				}
				else																		 //按 指令写			
				{
					switch(bms_buffer_dealwith[3]&0xff)
					{
						case 0x01://写 模块 (1 到 5 )  的 地址  和 AH量 设置 （ 10 或 20 AH )
									//Flash_Eeprom_Init();
									system_mode_temp = bms_buffer[4];
									system_mode_temp |= (bms_buffer[5]<<4);	
									//Flash_Eeprom_Write_Data(SYSTEM_MODE_eeprom,system_mode_temp);
									Write_System_Mode();
									
									Read_System_Mode();
									//Flash_Eeprom_Lock();
								break;
						case 0x02:
									if(bms_buffer_dealwith[4] == 35)
									{
										flag.jiaozhun_h = 1;
									}
									if(bms_buffer_dealwith[4] == 27)
									{
										flag.jiaozhun_l = 1;
									}									
									
										
								break;
						case 0x03:
									
									
									
								break;
						case 0x04://均衡 的 通讯 控制
										if(bms_buffer_dealwith[4] == 0)
										{
											bms_self_flag.bms_tongxin_balance_off = 1;
											bms_self_flag.bms_tongxin_balance_on = 0;
											/*Flash_Eeprom_Init();//擦除容量的循环次数
											Flash_Eeprom_Erase(SOC_CYCLE_number);
											Flash_Eeprom_Erase(SOC_CYCLE_number+1);
											Read_System_Mode();
											Flash_Eeprom_Lock();*/
										}
										else if(bms_buffer_dealwith[4] == 1)
										{
											bms_self_flag.bms_tongxin_balance_off = 0;
											bms_self_flag.bms_tongxin_balance_on = 1;
										}
										else
										{
											bms_self_flag.bms_tongxin_balance_on = 0;
											bms_self_flag.bms_tongxin_balance_off = 0;
										}
									
								break;
						case 0x05://设置循环次数
										soc_cycle_number = bms_buffer_dealwith[4];//容量循环加一
										Flash_Eeprom_Init();
										Flash_Eeprom_Write_Data(SOC_CYCLE_number,soc_cycle_number>>8);
										Flash_Eeprom_Write_Data(SOC_CYCLE_number+1,soc_cycle_number);
										
										soc_cycle_number	=Flash_Eeprom_Read_Data(SOC_CYCLE_number);
										soc_cycle_number =soc_cycle_number<<8;
										soc_cycle_number |=Flash_Eeprom_Read_Data(SOC_CYCLE_number+1);
										
										if(soc_cycle_number == 0)
										{
											write_soc_flag = 0;
											Flash_Eeprom_Write_Data(SOC_eeprom_flag,write_soc_flag);
											bms_self_flag1.readed_soc_static=0;// 重新 读取 固定容量
										}
										Flash_Eeprom_Lock();
										flag.soc_cycle_number = 0;
								break;
						default:
						
								break;		
					
					
					}
					
				}
			
			}
			else																			//读 系列 指令
			{
				if((bms_buffer_dealwith[2] & 0x01) == 0x01)//按地址读
				{
					
				}
				else																		//按指令读			
				{
					switch(bms_buffer_dealwith[3]&0xff)
					{
						case 0x01:
									 bms_self_flag.bms_send_alldata = 1;
								break;
						case 0x02:
						
								break;
						case 0x03:
						
								break;
						case 0x04:
						
								break;
						case 0x05:
						
								break;
						default:
						
								break;		
					
					
					}
				}

			}

				bms_self_flag.bms_buffer_receive_ok =0;//通讯的指令 部分只分析一次	
		}
		else
		{
			bms_self_flag.bms_buffer_crc_ok = 0;
			bms_self_flag.bms_buffer_receive_ok =0;
		}
	}

	if( bms_self_flag.bms_send_alldata == 1)
	{
								if(system_flag.soc_mode_20 == 1)
								{
								TxBuffer[0] = 82;//数据长度
								DATA_TO_TRANSFER = 82;
								}
								else
								{
								TxBuffer[0] = 79;//数据长度
								DATA_TO_TRANSFER = 79;
								}
								
								TxBuffer[1] = 0x01;// 
								TxBuffer[2] = 0x80; //  按 指令显示 数据
								TxBuffer[3] = 0x01;	//按 01 指令 显示数据
								cycle_number = 1;
								#ifdef SYSTEM_15S
								while(cycle_number < 16)
								{
									TxBuffer[2 + cycle_number * 2] = adc_voltage[cycle_number] >> 8;
									TxBuffer[2 + cycle_number * 2 +1] = adc_voltage[cycle_number];
									
									cycle_number++;
								}
								TxBuffer[2 + 16 * 2] = 0xff;
								TxBuffer[2 + 16 * 2 +1] = 0xff;//在15s中将第16节设置为0xffff
								#else
								while(cycle_number < 17)
								{
									TxBuffer[2 + cycle_number * 2] = adc_voltage[cycle_number] >> 8;
									TxBuffer[2 + cycle_number * 2 +1] = adc_voltage[cycle_number];
									
									cycle_number++;
								}
								#endif
								cycle_number = 0;
								while(cycle_number < 8)
								{
									TxBuffer[36 + cycle_number ] = t_n[cycle_number];						
									cycle_number++;
								}
								cycle_number = 0;
								
								//TxBuffer[40] = fault_query.U16;
								//TxBuffer[41] = fault_query.U16 >> 8;//用5 ，  6 节的温度表示硬件出错的标志用于查看

								//TxBuffer[44] = balance_output.U16>> 8;
								//TxBuffer[45] = balance_output.U16;
								TxBuffer[44] = 0;
								TxBuffer[45] = 0;
								
								//////
								TxBuffer[46] = t_vbat >> 8;
								TxBuffer[47] = t_vbat;
								
								TxBuffer[48] = max_voltage >> 8;
								TxBuffer[49] = max_voltage ;
								TxBuffer[50] = min_voltage >> 8;
								TxBuffer[51] = min_voltage ;
								TxBuffer[52] = max_temperature_send;
								TxBuffer[53] = min_temperature_send;
								TxBuffer[54] = soc_percent;
								TxBuffer[55] = soc_cycle_number>>8;				
								TxBuffer[56] = soc_cycle_number;
								
								TxBuffer[57] = system_flag.U16>>8;
								TxBuffer[58] = system_flag.U16;
								
								TxBuffer[59] = system_state.U32>>24;
								TxBuffer[60] = system_state.U32>>16;
								TxBuffer[61] = system_state.U32>>8;
								TxBuffer[62] = system_state.U32;
								
								TxBuffer[63] = fault_query.U16>>8;
								TxBuffer[64] = fault_query.U16;
								
								TxBuffer[65] = flag.U32>>24;				
								TxBuffer[66] = flag.U32>>16;
								TxBuffer[67] = flag.U32>>8;
								TxBuffer[68] = flag.U32;
								
								TxBuffer[68] = MyAddress;
								
								TxBuffer[69] = t_n[13];	
								TxBuffer[70] = t_n[14];	
								TxBuffer[71] = t_n[15];	
								if(system_flag.soc_mode_20==1)
								{
									TxBuffer[72] = t_n[8];	
									TxBuffer[73] = t_n[9];	
									TxBuffer[74] = t_n[10];	
									/////
									TxBuffer[75] = CHG_CURRENT>>8;
									TxBuffer[76] = CHG_CURRENT;	
									TxBuffer[77] = DCH_CURRENT>>8;	
									TxBuffer[78] = DCH_CURRENT;	
									
									Crc_Data = Crc_bms(78);  // 对 TxBuffer[2] 到 TxBuffer[43] 的数据进行CRC计算
									TxBuffer[79] = Crc_Data >> 8;
									TxBuffer[80] = Crc_Data;
									
									TxBuffer[81] = 0x01;
									
									
								}
								else
								{
									TxBuffer[72] = CHG_CURRENT>>8;
									TxBuffer[73] = CHG_CURRENT;	
									TxBuffer[74] = DCH_CURRENT>>8;	
									TxBuffer[75] = DCH_CURRENT;	
									
									Crc_Data = Crc_bms(75);  // 对 TxBuffer[2] 到 TxBuffer[43] 的数据进行CRC计算
									TxBuffer[76] = Crc_Data >> 8;
									TxBuffer[77] = Crc_Data;
									
									TxBuffer[78] = 0x01;
								}
								
								USART_SendData9(USART3, 0x100);
								while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
								Delay(5);
								USART_SendData9(USART3, 0x1ff);
								while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
								Delay(5);
								USART_SendData9(USART3, 0x1ff);
								while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
								Delay(5);
								USART_SendData9(USART3, 0x1ff);
								while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
								Delay(5);
								USART_SendData9(USART3, 0x1ff);
								while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
								Delay(5);
								USART_SendData9(USART3, 0x1ff);
								while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
								Delay(5);
								USART_SendData9(USART3, 0x1ff);
								while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
								Delay(5);
								
								//DMA_TX_Config();
								Sendtobmsself(DATA_TO_TRANSFER);
								bms_self_flag.bms_send_alldata = 0;
								//Delay(20000);
		//SendMyData();
	}

}

void Sendtobmsself(UINT8 number)
{
	UINT8 cycle = 0;
	while(cycle < number)
	{
		USART_SendData9(USART3, 0x100|TxBuffer[cycle]);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
		Delay(5);
		cycle++;
	}	




}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       