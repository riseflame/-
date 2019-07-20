/**
  ******************************************************************************
  * 文件名程: bsp_TB6600_TIM.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 57步进电机驱动实现
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp/57StepMotor/bsp_TB6600_TIM.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
__IO uint16_t prescaler=400;
__IO uint32_t pulse_count=0; /*  脉冲计数，一个完整的脉冲会增加2 */
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 配置TIMx复用输出PWM时用到的I/O
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void TB6600_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 使能定时器通道引脚GPIO时钟 */
  TB6600_TIM_GPIO_APBxClock_FUN(TB6600_TIM_GPIO_CLK, ENABLE); 
  
  RCC_APB2PeriphClockCmd(TB6600_DIR_GPIO_CLK|TB6600_ENA_GPIO_CLK,ENABLE);

  /* 配置定时器通道1输出引脚模式：复用开漏输出模式 */
  GPIO_InitStructure.GPIO_Pin =  TB6600_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TB6600_TIM_CH1_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =  TB6600_DIR_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(TB6600_DIR_PORT, &GPIO_InitStructure);
  GPIO_ResetBits(TB6600_DIR_PORT,TB6600_DIR_PIN);
  
  GPIO_InitStructure.GPIO_Pin =  TB6600_ENA_PIN;
  GPIO_Init(TB6600_ENA_PORT, &GPIO_InitStructure);
  GPIO_ResetBits(TB6600_ENA_PORT,TB6600_ENA_PIN);
}



/**
  * 函数功能: 配置TIM3输出的PWM信号的模式，如周期、极性、占空比
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT 重新计数
 *                    TIMx_CCR(电平发生变化)
 * 信号周期=(TIMx_ARR +1 ) * 时钟周期
 * 占空比=TIMx_CCR/(TIMx_ARR +1)
 */
static void TB6600_TIMx_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;  
  /* 定义嵌套向量中断控制器初始化结构体变量 */
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 使能定时器始终：设置TIM3CLK 为 72MHZ */
	TB6600_TIM_APBxClock_FUN (TB6600_TIM_CLK, ENABLE);
  
    /* 选择中断优先级配置组为4个抢占式优先级和4个子优先级，可以参考misc.h文件了解相关设置 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 使能KEY1所在的外部中断通道 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  /* 设置抢占式优先级为2 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
  /* 设置子优先级为3 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  /* 使能外部中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  /* 初始化配置嵌套向量中断控制器 */
  NVIC_Init(&NVIC_InitStructure); 
  
  /* 定时器基本参数始终 */		 
  /* 这里固定设置为：0xFFFF，对本例程无影响 */
  TIM_TimeBaseStructure.TIM_Period = (800-1); 
  /* 设置预分频：不预分频，即为72MHz,输出脉冲频率：72MHz/4=18MHz */
  /* 同一个定时器的不同通道PWM的频率只能是相同的，不同通道可以设置为不同的占空比 */
  TIM_TimeBaseStructure.TIM_Prescaler =(90-1);
  /* 设置时钟分频系数：不分频(这里用不到) */
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
  /* 向上计数模式 */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TB6600_TIMx, &TIM_TimeBaseStructure);

  /* 定时器输出通道1模式配置 */
  /* 模式配置：PWM模式1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
  /* 输出状态设置：使能输出 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
  /* 设置跳变值，当计数器计数到这个值时，电平发生跳变 */
  TIM_OCInitStructure.TIM_Pulse = prescaler;
  /* 当定时器计数值小于CCR1_Val时为高电平 */
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  /* 初始化定时器通道1输出PWM */
  TIM_OC1Init(TB6600_TIMx, &TIM_OCInitStructure);
  
  /* 定时器比较输出通道1预装载配置：使能预装载 */
  TIM_OC1PreloadConfig(TB6600_TIMx, TIM_OCPreload_Disable);

  TIM_ITConfig(TB6600_TIMx,TIM_IT_CC1,ENABLE);
  
  /* 使能定时器 */
  TIM_Cmd(TB6600_TIMx, ENABLE);
  
  /* TIM主输出使能 */
  TIM_CtrlPWMOutputs(TB6600_TIMx, ENABLE);
}

/**
  * 函数功能: TIMx 输出PWM信号初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：只要调用这个函数TIMx的四个通道就会有PWM信号输出
  */
void TB6600_TIMx_PWM_Init(void)
{
	TB6600_GPIO_Config();
	TB6600_TIMx_Configuration();	
}

/* 定时比较中断服务函数 */
void TIM1_CC_IRQHandler(void)
{
  if(TIM_GetITStatus(TB6600_TIMx,TIM_IT_CC1)!=RESET)
  {
    TIM_ClearITPendingBit(TB6600_TIMx,TIM_IT_CC1);
    TIM_SetCounter(TB6600_TIMx,0);
    TIM_SetCompare1(TB6600_TIMx,prescaler);
    pulse_count++;
  }
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
