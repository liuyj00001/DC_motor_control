#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "lcd.h"
// ALIENTEK 探索者STM32F407开发板 实验10
// 输入捕获实验 -库函数版本
// 技术支持：www.openedv.com
// 淘宝店铺：http://eboard.taobao.com
// 广州市星翼电子科技有限公司
// 作者：正点原子 @ALIENTEK

int main(void)
{
	long long temp = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); // 设置系统中断优先级分组2
	NVIC_SetPriority(SysTick_IRQn,15);
	delay_init(168);								// 初始化延时函数
	uart_init(115200);								// 初始化串口波特率为115200
	LCD_Init();
	Adc_Init();

	TIM8_Encoder_Init(); // 初始化编码器
	TIM3_Init();//	计算定时器
	TIM4_Init();//	上报定时器
		POINT_COLOR=RED; 
		LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
		LCD_ShowString(30,70,200,16,16,"DC Motor Control");	
		LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
		LCD_ShowString(30,110,200,16,16,"2024/11/8");	
	while (1)
	{
	}
}
