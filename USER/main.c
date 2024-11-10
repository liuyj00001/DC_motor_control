#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "lcd.h"
// ALIENTEK ̽����STM32F407������ ʵ��10
// ���벶��ʵ�� -�⺯���汾
// ����֧�֣�www.openedv.com
// �Ա����̣�http://eboard.taobao.com
// ������������ӿƼ����޹�˾
// ���ߣ�����ԭ�� @ALIENTEK

int main(void)
{
	long long temp = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // ����ϵͳ�ж����ȼ�����2
	NVIC_SetPriority(SysTick_IRQn,15);
	delay_init(168);								// ��ʼ����ʱ����
	uart_init(115200);								// ��ʼ�����ڲ�����Ϊ115200
	LCD_Init();
	Adc_Init();

	TIM8_Encoder_Init(); // ��ʼ��������
	TIM3_Init();//	���㶨ʱ��
	TIM4_Init();//	�ϱ���ʱ��
		POINT_COLOR=RED; 
		LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
		LCD_ShowString(30,70,200,16,16,"DC Motor Control");	
		LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
		LCD_ShowString(30,110,200,16,16,"2024/11/8");	
	while (1)
	{
	}
}
