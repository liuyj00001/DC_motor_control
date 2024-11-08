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
	TIM_Cmd(TIM3, ENABLE); // ʹ�ܶ�ʱ��3
}
float Encoder_Speed;
int16_t Encoder_CNT_B = 0;
int16_t Encoder_CNT_N = 0;
void TIM3_IRQHandler(void) // ����1ms
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // ���TIM6�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // ���TIM6�����жϱ�־λ
		Encoder_CNT_N = TIM_GetCounter(TIM9);		// ��ȡTIM9����ֵ
		if (Encoder_CNT_B > Encoder_CNT_N)
			Encoder_Speed = (Encoder_CNT_N + 0xffff - Encoder_CNT_B) * 1000.0f; // �����ٶ�
		else
			Encoder_Speed = (Encoder_CNT_N - Encoder_CNT_B) * 1000.0f; // �����ٶ�
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);		// ʹ��TIM8ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		// ʹ��GPIOCʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;		// PC6,PC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				// ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			// �ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				// ���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				// ����
	GPIO_Init(GPIOC, &GPIO_InitStructure);						// ��ʼ��GPIOC
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);		// PC6����ΪTIM8
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);		// PC7����ΪTIM8
	TIM_TimeBaseStructure.TIM_Period = 0xffff;					// ��ʱ������
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;				// ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// ��ʱ��ʱ�ӷ�Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);				// ��ʼ��TIM8

	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); // ʹ�ñ�����ģʽ

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				// ѡ������ͨ��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// ���벶����
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ���벶��ѡ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			// ���벶���Ƶ
	TIM_ICInitStructure.TIM_ICFilter = 0x0;							// ���벶���˲���
	TIM_ICInit(TIM8, &TIM_ICInitStructure);							// ��ʼ��TIM8���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				// ѡ������ͨ��
	TIM_ICInit(TIM8, &TIM_ICInitStructure);							// ��ʼ��TIM8���벶�����

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;  // ��ʱ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);							  // ��ʼ��NVIC
	TIM_ITConfig(TIM8, TIM_IT_COM, ENABLE);					  // ʹ�ܲ����ж�
	TIM_SetCounter(TIM8, 0);								  // ��ʱ������������
	TIM_Cmd(TIM8, ENABLE);									  // ʹ�ܶ�ʱ��
}
void TIM8_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM8, TIM_IT_COM) != RESET) // ����ж��Ƿ���
	{
		TIM_ClearITPendingBit(TIM8, TIM_IT_COM); // ����жϱ�־λ
	}
}
