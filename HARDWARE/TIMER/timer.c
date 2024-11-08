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
	TIM_Cmd(TIM3, ENABLE); // 使能定时器3
}
float Encoder_Speed;
int16_t Encoder_CNT_B = 0;
int16_t Encoder_CNT_N = 0;
void TIM3_IRQHandler(void) // 周期1ms
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // 检查TIM6更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除TIM6更新中断标志位
		Encoder_CNT_N = TIM_GetCounter(TIM9);		// 读取TIM9计数值
		if (Encoder_CNT_B > Encoder_CNT_N)
			Encoder_Speed = (Encoder_CNT_N + 0xffff - Encoder_CNT_B) * 1000.0f; // 计算速度
		else
			Encoder_Speed = (Encoder_CNT_N - Encoder_CNT_B) * 1000.0f; // 计算速度
		Encoder_CNT_B = Encoder_CNT_N;
		printf("TIM3_IRQHandler %d \r\n", TIM_GetCounter(TIM9));
	}
}

void TIM8_Encoder_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);		// 使能TIM8时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		// 使能GPIOC时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;		// PC6,PC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			// 速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				// 上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);						// 初始化GPIOC
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);		// PC6复用为TIM8
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);		// PC7复用为TIM8
	TIM_TimeBaseStructure.TIM_Period = 0xffff;					// 定时器周期
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;				// 定时器分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 定时器时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);				// 初始化TIM8

	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // 使用编码器模式

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				// 选择输入通道
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// 输入捕获极性
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 输入捕获选择
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			// 输入捕获分频
	TIM_ICInitStructure.TIM_ICFilter = 0x0;							// 输入捕获滤波器
	TIM_ICInit(TIM8, &TIM_ICInitStructure);							// 初始化TIM8输入捕获参数
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				// 选择输入通道
	TIM_ICInit(TIM8, &TIM_ICInitStructure);							// 初始化TIM8输入捕获参数

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;  // 定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // 子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // 使能中断
	NVIC_Init(&NVIC_InitStructure);							  // 初始化NVIC
	TIM_ITConfig(TIM8, TIM_IT_COM, ENABLE);					  // 使能捕获中断
	TIM_SetCounter(TIM8, 0);								  // 定时器计数器清零
	TIM_Cmd(TIM8, ENABLE);									  // 使能定时器
}
void TIM8_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM8, TIM_IT_COM) != RESET) // 检测中断是否发生
	{
		TIM_ClearITPendingBit(TIM8, TIM_IT_COM); // 清除中断标志位
	}
}
