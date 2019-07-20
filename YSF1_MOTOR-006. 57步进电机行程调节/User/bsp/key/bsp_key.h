#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� --------------------------------------------------------------*/
typedef enum
{
  KEY_UP   = 0,
  KEY_DOWN = 1,
}KEYState_TypeDef;

/* �궨�� --------------------------------------------------------------------*/
#define KEY1_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOE
#define KEY1_GPIO_PIN                 GPIO_Pin_3
#define KEY1_GPIO                     GPIOE
#define KEY1_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY1����ʱ����Ϊ�ߵ�ƽ��������������Ϊ1 */

#define KEY2_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOE
#define KEY2_GPIO_PIN                 GPIO_Pin_2
#define KEY2_GPIO                     GPIOE
#define KEY2_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY1����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void KEY_GPIO_Init(void);
KEYState_TypeDef KEY1_StateRead(void);
KEYState_TypeDef KEY2_StateRead(void);

#endif  // __BSP_KEY_H__

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
