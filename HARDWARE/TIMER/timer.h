#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// ALIENTEK STM32F407开发板
// 定时器 驱动代码
// 正点原子@ALIENTEK
// 技术论坛:www.openedv.com
// 创建日期:2014/5/4
// 版本：V1.2
// 版权所有，盗版必究。
// Copyright(C) 广州市星翼电子科技有限公司 2014-2024
// All rights reserved
//********************************************************************************
// 修改说明
// V1.1 20140504
// 新增TIM14_PWM_Init函数,用于PWM输出
// V1.2 20140504
// 新增TIM5_CH1_Cap_Init函数,用于输入捕获
//////////////////////////////////////////////////////////////////////////////////
extern uint16_t DIR; // 方向
extern float Encoder_Speed;
extern float voltage ;
void TIM8_Encoder_Init(void);
void TIM3_Init(void);
void TIM4_Init(void);
#endif
