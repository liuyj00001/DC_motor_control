#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"

// ALIENTEK 探索者STM32F407开发板 实验10
// 输入捕获实验 -库函数版本
// 技术支持：www.openedv.com
// 淘宝店铺：http://eboard.taobao.com
// 广州市星翼电子科技有限公司
// 作者：正点原子 @ALIENTEK

int main(void)
{
	long long temp = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置系统中断优先级分组2
	delay_init(168);								// 初始化延时函数
	uart_init(115200);								// 初始化串口波特率为115200
	printf("	IN     WHILE     LOOP     \r\n");
	TIM9_Encoder_Init(); // 初始化编码器
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
