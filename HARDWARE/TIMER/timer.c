#include "timer.h"
#include "led.h"
#include "usart.h"
#include "adc.h"
#include "lcd.h"
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04; // 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		 // 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM3, ENABLE); // ʹ�ܶ�ʱ��3
}
float Encoder_Speed=0;
float voltage =0;
float current = 0;
int16_t Encoder_CNT_D = 0;
int16_t Encoder_CNT_B = 0;
int16_t Encoder_CNT_N = 0;
void TIM3_IRQHandler(void) // ����1ms
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // 
	{
		Encoder_CNT_N = TIM_GetCounter(TIM8);		// 
		if (Encoder_CNT_B > Encoder_CNT_N)
			Encoder_CNT_D = (Encoder_CNT_N + 0xffff - Encoder_CNT_B); // �����ٶ�
		else
			Encoder_CNT_D = (Encoder_CNT_N - Encoder_CNT_B); // �����ٶ�
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); /// ʹ��TIM4ʱ��

	TIM_TimeBaseInitStructure.TIM_Period = 5000 - 1;				// �Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 8400 - 1;				// ��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure); // ��ʼ��TIM4

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); // ����ʱ��4�����ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;				 // ��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		 // 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM4, ENABLE); // ʹ�ܶ�ʱ��4
}
float temp_float;
int temp_int;
void TIM4_IRQHandler(void) // ����0.5s
{
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		
		printf("VOLTAGE %f \r\n", voltage);
		printf("TIM3_IRQHandler %f \r\n", Encoder_Speed);
		  
		
		POINT_COLOR=BLUE;//��������Ϊ��ɫ
		//������ѹ
		LCD_ShowString(30,130,200,16,16,"VOLTAGE:");	
		temp_float = voltage;	
		temp_int = voltage;
		if (temp_float < 0) {
    // ����Ǹ�������ʾ����
    LCD_ShowChar(134-8,150,'-',16,0); // ����'-'�ַ����ԼΪ8����
			temp_int = abs(temp_int);
			temp_float= - temp_float;
		}
		else 
		{
			LCD_ShowChar(134-8,150,'+',16,0); // ����'+'�ַ����ԼΪ8����
		}
		LCD_ShowxNum(134,130,temp_int,4,16,0);	//4λ����
		temp_int = (int)((temp_float-temp_int)*100);
		LCD_ShowString(134+11*4-5-4,130,200,16,16,".");	
		LCD_ShowxNum(134+11*4-5,130,temp_int,2,16,0);	//2λС��
		LCD_ShowString(134+11*6-3,130,200,16,16,"V");	
		//��������
		LCD_ShowString(30,150,200,16,16,"CURRENT:");	
		temp_float = current;	
		temp_int = (int)current;
		if (temp_float < 0) {
    // ����Ǹ�������ʾ����
    LCD_ShowChar(134-8,150,'-',16,0); // ����'-'�ַ����ԼΪ8����
			temp_int = abs(temp_int);
			temp_float= - temp_float;
		}
		else 
		{
			LCD_ShowChar(134-8,150,'+',16,0); // ����'+'�ַ����ԼΪ8����
		}
		LCD_ShowxNum(134,150,temp_int,4,16,0);	//4λ����
		temp_int = abs((int)((temp_float-temp_int)*100));
		LCD_ShowString(134+11*4-5-4,150,200,16,16,".");	
		LCD_ShowxNum(134+11*4-5,150,temp_int,2,16,0);	//2λС��
		LCD_ShowString(134+11*6-3,150,200,16,16,"A");	

		//���ת��
		LCD_ShowString(30,170,200,16,16,"Speed:");	   
		temp_float = Encoder_Speed;
		temp_int = (int)Encoder_Speed;
		if (temp_float < 0) {
    // ����Ǹ�������ʾ����
    LCD_ShowChar(134-8,150,'-',16,0); // ����'-'�ַ����ԼΪ8����
			temp_int = abs(temp_int);
			temp_float= - temp_float;
		}
		else 
		{
			LCD_ShowChar(134-8,150,'+',16,0); // ����'+'�ַ����ԼΪ8����
		}
		LCD_ShowxNum(134,170,temp_int,4,16,0);//4λ����
		temp_int= (int16_t)((temp_float-temp_int)*100);
		LCD_ShowString(134+11*4-5-4,170,200,16,16,".");	
		LCD_ShowxNum(134+11*4-5,170,temp_int,2,16,0);		
		LCD_ShowString(134+11*6-3,170,200,16,16,"RPM");//2λС��	 
		
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
void TIM8_Encoder_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
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

	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge); // ʹ�ñ�����ģʽ

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;				// ѡ������ͨ��
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		// ���벶����
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ���벶��ѡ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			// ���벶���Ƶ
	TIM_ICInitStructure.TIM_ICFilter = 0x0;							// ���벶���˲���
	TIM_ICInit(TIM8, &TIM_ICInitStructure);							// ��ʼ��TIM8���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;				// ѡ������ͨ��
	TIM_ICInit(TIM8, &TIM_ICInitStructure);							// ��ʼ��TIM8���벶�����
/*
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;  // ��ʱ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);							  // ��ʼ��NVIC
	TIM_ITConfig(TIM8, TIM_IT_COM, ENABLE);					  // ʹ�ܲ����ж�
*/
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
