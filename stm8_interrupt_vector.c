/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */

#include "main.h"
typedef void @far (*interrupt_handler_t)(void);

struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	/*
	DO_LOW();
	system_state.discharge_state = 0;
	CO_LOW();
	system_state.charge_state = 0;
	PRE_CHG_LOW();
	system_state.charge_pre_state = 0;
	
	USART_SendData8(USART3, 0Xaa);		
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	*/USART_SendData9(USART3, 0X1aa);		
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
		
	Delay(20000);
	return;
}

extern void _stext();     /* startup routine */
extern @far @interrupt void DMA1_CHANNEL0_1_IRQHandler (void);// 2
extern @far @interrupt void IT_INT1 (void);// 11
extern @far @interrupt void IT_INT2 (void);// 10
extern @far @interrupt void TIM3_CC_USART3_RX_IRQHandler (void);// 22
extern @far @interrupt void tim1_ms_irp (void);// 23
extern @far @interrupt void TIM4_UPD_OVF_TRG_IRQHandler (void);// 23

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NonHandledInterrupt}, /* trap  */
	{0x82, NonHandledInterrupt}, /* irq0  */
	{0x82, NonHandledInterrupt}, /* irq1  */
	{0x82, DMA1_CHANNEL0_1_IRQHandler}, /* irq2  */
	{0x82, NonHandledInterrupt}, /* irq3  */
	{0x82, NonHandledInterrupt}, /* irq4  */
	{0x82, NonHandledInterrupt}, /* irq5  */
	{0x82, NonHandledInterrupt}, /* irq6  */
	{0x82, NonHandledInterrupt}, /* irq7  */
	{0x82, NonHandledInterrupt}, /* irq8  */
	{0x82, NonHandledInterrupt}, /* irq9  */
	{0x82, NonHandledInterrupt}, /* irq10 */
	{0x82, NonHandledInterrupt}, /* irq11 */
	{0x82, NonHandledInterrupt}, /* irq12 */
	{0x82, NonHandledInterrupt}, /* irq13 */
	{0x82, NonHandledInterrupt}, /* irq14 */
	{0x82, NonHandledInterrupt}, /* irq15 */
	{0x82, NonHandledInterrupt}, /* irq16 */
	{0x82, NonHandledInterrupt}, /* irq17 */
	{0x82, NonHandledInterrupt}, /* irq18 */
	{0x82, NonHandledInterrupt}, /* irq19 */
	{0x82, NonHandledInterrupt}, /* irq20 */
	{0x82, NonHandledInterrupt}, /* irq21 */
	{0x82, TIM3_CC_USART3_RX_IRQHandler}, /* irq22 */
	{0x82, tim1_ms_irp}, /* irq23 */
	{0x82, NonHandledInterrupt}, /* irq24 */
	{0x82, TIM4_UPD_OVF_TRG_IRQHandler}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};
