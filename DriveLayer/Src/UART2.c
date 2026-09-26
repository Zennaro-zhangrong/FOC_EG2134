/*
 * UART2.c
 *
 *  Created on: 2026年6月8日
 *      Author: zr186
 */
#include "../Inc/UART2.h"
#include "../../Core/Inc/usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

uint8_t Rx_RingBuffer[RX_RingBuffer_SIZE] = {0};
uint8_t Tx_RingBuffer[TX_RingBuffer_SIZE] = {0};

uartHandle uart2_Handle;

void uart2_Init(void){
	uart2_Handle.huart = &huart4;
	RingBuffer_Init(&uart2_Handle.Rx_RingBuffer, Rx_RingBuffer, RX_RingBuffer_SIZE);
	RingBuffer_Init(&uart2_Handle.Tx_RingBuffer, Tx_RingBuffer, TX_RingBuffer_SIZE);
	HAL_UART_Receive_IT(uart2_Handle.huart, &uart2_Handle.RX_Data, 1);
}

/**
 * @brief  串口格式化发送函数（支持printf格式）
 * @param  fmt: 格式化字符串
 * @retval 无
 */
void uasrt2_printf(const char *fmt, ...)
{
    char buf[256];  // 发送缓冲区，可根据需求调整大小
    va_list args;

    // 初始化可变参数列表
    va_start(args, fmt);
    // 格式化字符串到缓冲区
    vsnprintf(buf, sizeof(buf), fmt, args);
    // 结束可变参数处理
    va_end(args);

    //写入环形缓冲区
    unsigned char loop_num = strlen(buf);
    for(int i = 0; i < loop_num; i++){
       RingBuffer_WriteByte(&uart2_Handle.Tx_RingBuffer, (unsigned char)buf[i]);
    }
}


/*
 * @功能：进行一次缓冲区数据打印，缓冲区清空则退出函数，一般放在主循环中，执行间隔
 * 过久导致缓冲区频繁溢出可以在 Serial_port_log.h 中适量增加缓冲区大小。
 * @参数：无。
 * @返回：执行时长。
 */
unsigned short usart2_send_loop(void){
	unsigned int time_start = HAL_GetTick();
    while(RingBuffer_ReadByte(&uart2_Handle.Tx_RingBuffer, &uart2_Handle.TX_Data) == 1){
        HAL_UART_Transmit(uart2_Handle.huart, &uart2_Handle.TX_Data, 1, 10);//发送函数
    }
	unsigned int time_now = HAL_GetTick();
	return (unsigned short)(time_now - time_start);
}


/*
 * @功能：接收函数的回调函数
 * @参数：无
 * @返回：无
 */
void usart2_Receive_Callback(void){
	RingBuffer_WriteByte(&uart2_Handle.Rx_RingBuffer, uart2_Handle.RX_Data);
	HAL_UART_Receive_IT(uart2_Handle.huart, &uart2_Handle.RX_Data, 1);
}


