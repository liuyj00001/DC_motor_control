#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"

// ALIENTEK ̽����STM32F407������ ʵ��10
// ���벶��ʵ�� -�⺯���汾
// ����֧�֣�www.openedv.com
// �Ա����̣�http://eboard.taobao.com
// ������������ӿƼ����޹�˾
// ���ߣ�����ԭ�� @ALIENTEK

int main(void)
{
	long long temp = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ����ϵͳ�ж����ȼ�����2
	delay_init(168);								// ��ʼ����ʱ����
	uart_init(115200);								// ��ʼ�����ڲ�����Ϊ115200
	printf("	IN     WHILE     LOOP     \r\n");
	TIM9_Encoder_Init(); // ��ʼ��������
	TIM3_Init();
	printf("	IN     WHILE     LOOP     \r\n");
	printf("	IN     WHILE     LOOP     \r\n");
	printf("	IN     WHILE     LOOP     \r\n");
	while (1)
	{
		printf("	IN     WHILE     LOOP     \r\n");
		delay_ms(1000);
	}
}
