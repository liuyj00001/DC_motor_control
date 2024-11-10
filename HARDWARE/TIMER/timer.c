#include "timer.h"
#include "led.h"
#include "usart.h"
#include "adc.h"
#include "lcd.h"
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04; // 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		 // 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM3, ENABLE); // 使能定时器3
}
float Encoder_Speed=0;
float voltage =0;
float current = 0;
int16_t Encoder_CNT_D = 0;
int16_t Encoder_CNT_B = 0;
int16_t Encoder_CNT_N = 0;
void TIM3_IRQHandler(void) // 周期1ms
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // 
	{
		Encoder_CNT_N = TIM_GetCounter(TIM8);		// 
		if (Encoder_CNT_B > Encoder_CNT_N)
			Encoder_CNT_D = (Encoder_CNT_N + 0xffff - Encoder_CNT_B); // 计算速度
		else
			Encoder_CNT_D = (Encoder_CNT_N - Encoder_CNT_B); // 计算速度
		Encoder_Speed = Encoder_CNT_D*60/194.0/0.001;
		Encoder_CNT_B = Encoder_CNT_N;

		voltage = (float)(Get_Adc_Average(5,5)*(3.3/4096.0));
		current = (float)(8.25 - 5.0 * voltage);
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); /// 使能TIM4时钟

	TIM_TimeBaseInitStructure.TIM_Period = 5000 - 1;				// 自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 8400 - 1;				// 定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure); // 初始化TIM4

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); // 允许定时器4更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				 // 定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		 // 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM4, ENABLE); // 使能定时器4
}
float temp_float;
int temp_int;
void TIM4_IRQHandler(void) // 周期0.5s
{
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		
		printf("VOLTAGE %f \r\n", voltage);
		printf("TIM3_IRQHandler %f \r\n", Encoder_Speed);
		  
		
		POINT_COLOR=BLUE;//设置字体为蓝色
		//采样电压
		LCD_ShowString(30,130,200,16,16,"VOLTAGE:");	
		temp_float = voltage;	
		temp_int = voltage;
		if (temp_float < 0) {
    // 如果是负数，显示负号
    LCD_ShowChar(134-8,150,'-',16,0); // 假设'-'字符宽度约为8像素
			temp_int = abs(temp_int);
			temp_float= - temp_float;
		}
		else 
		{
			LCD_ShowChar(134-8,150,'+',16,0); // 假设'+'字符宽度约为8像素
		}
		LCD_ShowxNum(134,130,temp_int,4,16,0);	//4位整数
		temp_int = (int)((temp_float-temp_int)*100);
		LCD_ShowString(134+11*4-5-4,130,200,16,16,".");	
		LCD_ShowxNum(134+11*4-5,130,temp_int,2,16,0);	//2位小数
		LCD_ShowString(134+11*6-3,130,200,16,16,"V");	
		//采样电流
		LCD_ShowString(30,150,200,16,16,"CURRENT:");	
		temp_float = current;	
		temp_int = (int)current;
		if (temp_float < 0) {
    // 如果是负数，显示负号
    LCD_ShowChar(134-8,150,'-',16,0); // 假设'-'字符宽度约为8像素
			temp_int = abs(temp_int);
			temp_float= - temp_float;
		}
		else 
		{
			LCD_ShowChar(134-8,150,'+',16,0); // 假设'+'字符宽度约为8像素
		}
		LCD_ShowxNum(134,150,temp_int,4,16,0);	//4位整数
		temp_int = abs((int)((temp_float-temp_int)*100));
		LCD_ShowString(134+11*4-5-4,150,200,16,16,".");	
		LCD_ShowxNum(134+11*4-5,150,temp_int,2,16,0);	//2位小数
		LCD_ShowString(134+11*6-3,150,200,16,16,"A");	

		//电机转速
		LCD_ShowString(30,170,200,16,16,"Speed:");	   
		temp_float = Encoder_Speed;
		temp_int = (int)Encoder_Speed;
		if (temp_float < 0) {
    // 如果是负数，显示负号
    LCD_ShowChar(134-8,150,'-',16,0); // 假设'-'字符宽度约为8像素
			temp_int = abs(temp_int);
			temp_float= - temp_float;
		}
		else 
		{
			LCD_ShowChar(134-8,150,'+',16,0); // 假设'+'字符宽度约为8像素
		}
		LCD_ShowxNum(134,170,temp_int,4,16,0);//4位整数
		temp_int= (int16_t)((temp_float-temp_int)*100);
		LCD_ShowString(134+11*4-5-4,170,200,16,16,".");	
		LCD_ShowxNum(134+11*4-5,170,temp_int,2,16,0);		
		LCD_ShowString(134+11*6-3,170,200,16,16,"RPM");//2位小数	 
		
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
void TIM8_Encoder_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
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

	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge); // 使用编码器模式

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				// 选择输入通道
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// 输入捕获极性
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 输入捕获选择
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			// 输入捕获分频
	TIM_ICInitStructure.TIM_ICFilter = 0x0;							// 输入捕获滤波器
	TIM_ICInit(TIM8, &TIM_ICInitStructure);							// 初始化TIM8输入捕获参数
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				// 选择输入通道
	TIM_ICInit(TIM8, &TIM_ICInitStructure);							// 初始化TIM8输入捕获参数
/*
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;  // 定时器中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // 子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // 使能中断
	NVIC_Init(&NVIC_InitStructure);							  // 初始化NVIC
	TIM_ITConfig(TIM8, TIM_IT_COM, ENABLE);					  // 使能捕获中断
*/
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
