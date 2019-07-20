#ifndef __TB6600_TIM_H__
#define __TB6600_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define TB6600_TIMx                        TIM1
#define TB6600_TIM_APBxClock_FUN           RCC_APB2PeriphClockCmd
#define TB6600_TIM_CLK                     RCC_APB2Periph_TIM1

#define TB6600_TIM_GPIO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define TB6600_TIM_GPIO_CLK                (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO )
#define TB6600_TIM_CH1_PORT                GPIOA                 // ��ӦTB6600��PUL+��
#define TB6600_TIM_CH1_PIN                 GPIO_Pin_8            // ��PLU-ֱ�ӽӿ������GND

#define TB6600_DIR_GPIO_CLK                RCC_APB2Periph_GPIOB  // �����ת�������
#define TB6600_DIR_PORT                    GPIOB                 // ��ӦTB6600��DIR+��
#define TB6600_DIR_PIN                     GPIO_Pin_13           // ��DIR-ֱ�ӽӿ������GND

#define TB6600_ENA_GPIO_CLK                RCC_APB2Periph_GPIOB  // ���ʹ�ܿ��ƣ������ſɲ�������
#define TB6600_ENA_PORT                    GPIOB                 // ��ӦTB6600��ENA+��
#define TB6600_ENA_PIN                     GPIO_Pin_14           // ��ENA-ֱ�ӽӿ������GND

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void TB6600_TIMx_PWM_Init(void);

#endif	/* __TB6600_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
