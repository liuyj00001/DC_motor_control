#include "timer.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// ALIENTEK STM32F407开发板
// 定时器 驱动代码
// 正点原子@ALIENTEK
// 技术论坛:www.openedv.com
// 创建日期:2014/5/4
// 版本：V1.0
// 版权所有，盗版必究。
// Copyright(C) 广州市星翼电子科技有限公司 2014-2024
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////

void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); /// 使能TIM3时钟

	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;				// 自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84 - 1;				// 定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); // 初始化TIM3

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 允许定时器3更新中断


	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				 // 定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // 抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	TIM_Cmd(TIM3, ENABLE);					   // 使能定时器3
}
float Encoder_Speed;
int16_t Encoder_CNT_B = 0;
int16_t Encoder_CNT_N = 0;
void TIM3_IRQHandler(void) // 周期1ms
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // 检查TIM6更新中断发生与否
	{
		Encoder_CNT_N = TIM_GetCounter(TIM9); // 读取TIM9计数值
		if (Encoder_CNT_B > Encoder_CNT_N)
			Encoder_Speed = (Encoder_CNT_N + 0xffff - Encoder_CNT_B) * 1000.0f; // 计算速度
		else
			Encoder_Speed = (Encoder_CNT_N - Encoder_CNT_B) * 1000.0f; // 计算速度
		Encoder_CNT_B = Encoder_CNT_N;
		printf("TIM6_IRQHandler Encoder_CNT_B \r\n");
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除TIM6更新中断标志位
	}
}

void TIM9_Encoder_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);  // 使能TIM9时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // 使能GPIOE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9);
	// 定时器9初始化
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;					// 定时器分频 计数器的最大值
	TIM_TimeBaseStructure.TIM_Prescaler = 0;					// 定时器预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);				// 初始化TIM9

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				// 选择输入端
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// 上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 选择输入端
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			// 输入捕获分频
	TIM_ICInitStructure.TIM_ICFilter = 0x0;							// 输入捕获滤波器
	TIM_ICInit(TIM9, &TIM_ICInitStructure);							// 初始化TIM9输入捕获参数
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				// 选择输入端
	TIM_ICInit(TIM9, &TIM_ICInitStructure);							// 初始化TIM9输入捕获参数

	TIM_EncoderInterfaceConfig(TIM9, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // 使用编码器模式

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;  // 定时器9中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // 子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // 使能中断
	NVIC_Init(&NVIC_InitStructure);							  // 初始化NVIC

	TIM_ITConfig(TIM9, TIM_IT_CC1, ENABLE);		// 使能定时器9捕获中断
	TIM_ClearITPendingBit(TIM9, TIM_IT_Update); // 清除定时器9更新中断标志位
	TIM9->CNT = 0;								// 定时器9计数器清零
	TIM_Cmd(TIM9, ENABLE);						// 使能定时器9
}
uint16_t DIR;
void TIM9_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM9, TIM_IT_CC1) != RESET)
	{
		if (TIM_GetCapture1(TIM9) > 0)
		{
			DIR = 1;
		}
		else
		{
			DIR = 0;
		}
		TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);
	}
}
