#include "timer.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////
// ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
// ALIENTEK STM32F407������
// ��ʱ�� ��������
// ����ԭ��@ALIENTEK
// ������̳:www.openedv.com
// ��������:2014/5/4
// �汾��V1.0
// ��Ȩ���У�����ؾ���
// Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
// All rights reserved
//////////////////////////////////////////////////////////////////////////////////

void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); /// ʹ��TIM3ʱ��

	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;				// �Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84 - 1;				// ��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure); // ��ʼ��TIM3

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // ����ʱ��3�����ж�


	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				 // ��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // ��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		 // �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	TIM_Cmd(TIM3, ENABLE);					   // ʹ�ܶ�ʱ��3
}
float Encoder_Speed;
int16_t Encoder_CNT_B = 0;
int16_t Encoder_CNT_N = 0;
void TIM3_IRQHandler(void) // ����1ms
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // ���TIM6�����жϷ������
	{
		Encoder_CNT_N = TIM_GetCounter(TIM9); // ��ȡTIM9����ֵ
		if (Encoder_CNT_B > Encoder_CNT_N)
			Encoder_Speed = (Encoder_CNT_N + 0xffff - Encoder_CNT_B) * 1000.0f; // �����ٶ�
		else
			Encoder_Speed = (Encoder_CNT_N - Encoder_CNT_B) * 1000.0f; // �����ٶ�
		Encoder_CNT_B = Encoder_CNT_N;
		printf("TIM6_IRQHandler Encoder_CNT_B \r\n");
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // ���TIM6�����жϱ�־λ
	}
}

void TIM9_Encoder_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);  // ʹ��TIM9ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // ʹ��GPIOEʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9);
	// ��ʱ��9��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;					// ��ʱ����Ƶ �����������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 0;					// ��ʱ��Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);				// ��ʼ��TIM9

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				// ѡ�������
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// �����ز���
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ѡ�������
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			// ���벶���Ƶ
	TIM_ICInitStructure.TIM_ICFilter = 0x0;							// ���벶���˲���
	TIM_ICInit(TIM9, &TIM_ICInitStructure);							// ��ʼ��TIM9���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				// ѡ�������
	TIM_ICInit(TIM9, &TIM_ICInitStructure);							// ��ʼ��TIM9���벶�����

	TIM_EncoderInterfaceConfig(TIM9, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // ʹ�ñ�����ģʽ

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;  // ��ʱ��9�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);							  // ��ʼ��NVIC

	TIM_ITConfig(TIM9, TIM_IT_CC1, ENABLE);		// ʹ�ܶ�ʱ��9�����ж�
	TIM_ClearITPendingBit(TIM9, TIM_IT_Update); // �����ʱ��9�����жϱ�־λ
	TIM9->CNT = 0;								// ��ʱ��9����������
	TIM_Cmd(TIM9, ENABLE);						// ʹ�ܶ�ʱ��9
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
