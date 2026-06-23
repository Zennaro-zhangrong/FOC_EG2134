/*
 * UART2.h
 *
 *  Created on: 2026年6月8日
 *      Author: zr186
 */

#ifndef INC_UART2_H_
#define INC_UART2_H_
#include "../../Core/Inc/usart.h"
#include "../Inc/RingBuffer.h"

#define RX_RingBuffer_SIZE 1024
#define TX_RingBuffer_SIZE 1024


typedef struct{
	UART_HandleTypeDef *huart;
	RingBuffer Rx_RingBuffer;
	RingBuffer Tx_RingBuffer;
	unsigned char RX_Data;
	unsigned char TX_Data;
}uartHandle;


void uart2_Init(void);
/**
 * @brief  串口格式化发送函数（支持printf格式）
 * @param  fmt: 格式化字符串
 * @retval 无
 */
void uasrt2_printf(const char *fmt, ...);

/*
 * @功能：进行一次缓冲区数据打印，缓冲区清空则退出函数，一般放在主循环中，执行间隔
 * 过久导致缓冲区频繁溢出可以在 Serial_port_log.h 中适量增加缓冲区大小。
 * @参数：无。
 * @返回：执行时长。
 */
unsigned short usart2_send_loop(void);

/*
 * @功能：接收函数的回调函数
 * @参数：无
 * @返回：无
 */
void usart2_Receive_Callback(void);

#endif /* INC_UART2_H_ */
