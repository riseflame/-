#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* 类型定义 --------------------------------------------------------------*/
typedef enum
{
  KEY_UP   = 0,
  KEY_DOWN = 1,
}KEYState_TypeDef;

/* 宏定义 --------------------------------------------------------------------*/
#define KEY1_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOE
#define KEY1_GPIO_PIN                 GPIO_Pin_3
#define KEY1_GPIO                     GPIOE
#define KEY1_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为高电平，所以这里设置为1 */

#define KEY2_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOE
#define KEY2_GPIO_PIN                 GPIO_Pin_2
#define KEY2_GPIO                     GPIOE
#define KEY2_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */

/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void KEY_GPIO_Init(void);
KEYState_TypeDef KEY1_StateRead(void);
KEYState_TypeDef KEY2_StateRead(void);

#endif  // __BSP_KEY_H__

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
