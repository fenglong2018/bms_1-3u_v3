#ifndef __COMMON_H
#define __COMMON_H


#include "main.h"

#define USART_DMA_CHANNEL_RX   DMA1_Channel2
#define USART_DMA_CHANNEL_TX   DMA1_Channel1
#define USART_DMA_FLAG_TCRX    (uint16_t)DMA1_FLAG_TC2
#define USART_DMA_FLAG_TCTX    (uint16_t)DMA1_FLAG_TC1
#define USART_DR_ADDRESS       (uint16_t)0x53F1  /* USART3 Data register Address */

#define USART3_BAUDRATE			4800

extern UINT8 TxBuffer[120];
extern UINT8 DATA_TO_TRANSFER ;
extern UINT8 receive_data[10];
extern uint16_t Usart_Data_Length;
extern uint16_t Rx_Bit9;
extern uint16_t Tx_Bit9;
extern UINT16 Crc_Check;
extern UINT16	Crc_Data;
extern UINT8  receive_count;

extern UINT8  MyAddress ;
extern UINT8  MySocMode ;
extern UINT8 system_mode_temp;
extern UINT8 FZ;
extern UINT8 FZ_TEMP;
extern char soc_mode_10;
extern char setaddsoc_0x00_count;
extern char	setaddsoc_0x00_start;

extern char setaddsoc_0x01_count;
extern char	setaddsoc_0x01_start;
extern char setaddsoc_0x02_count;
extern char	setaddsoc_0x02_start;

extern char setaddsoc_0x03_count;
extern char	setaddsoc_0x03_start;









UINT16 Crc(UINT16 count);
void USART3_Config(void);
void DMA_TX_Config(void);
void Format_Buffer(void);
void SetAddSoc(void);
void FormatAddSoc(char soc_mode_10);
void Send_To_M(UINT8 DATA_TO_TRANSFER);


void Bms_self_receive(void);
void Bms_tongxin_dealwith(void);
void Sendtobmsself(UINT8 number);
#endif

