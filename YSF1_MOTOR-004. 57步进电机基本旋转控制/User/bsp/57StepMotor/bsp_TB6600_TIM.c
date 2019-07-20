/**
  ******************************************************************************
  * �ļ�����: bsp_TB6600_TIM.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: 57�����������ʵ��
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp/57StepMotor/bsp_TB6600_TIM.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
__IO uint16_t prescaler=500;
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ����TIMx�������PWMʱ�õ���I/O
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void TB6600_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* ʹ�ܶ�ʱ��ͨ������GPIOʱ�� */
  TB6600_TIM_GPIO_APBxClock_FUN(TB6600_TIM_GPIO_CLK, ENABLE); 
  
  RCC_APB2PeriphClockCmd(TB6600_DIR_GPIO_CLK|TB6600_ENA_GPIO_CLK,ENABLE);

  /* ���ö�ʱ��ͨ��1�������ģʽ�����ÿ�©���ģʽ */
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
  * ��������: ����TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ���¼���
 *                    TIMx_CCR(��ƽ�����仯)
 * �ź�����=(TIMx_ARR +1 ) * ʱ������
 * ռ�ձ�=TIMx_CCR/(TIMx_ARR +1)
 */
static void TB6600_TIMx_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;  
  /* ����Ƕ�������жϿ�������ʼ���ṹ����� */
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ʹ�ܶ�ʱ��ʼ�գ�����TIM3CLK Ϊ 72MHZ */
	TB6600_TIM_APBxClock_FUN (TB6600_TIM_CLK, ENABLE);
  
    /* ѡ���ж����ȼ�������Ϊ4����ռʽ���ȼ���4�������ȼ������Բο�misc.h�ļ��˽�������� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ʹ��KEY1���ڵ��ⲿ�ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  /* ������ռʽ���ȼ�Ϊ2 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
  /* ���������ȼ�Ϊ3 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  /* ʹ���ⲿ�ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  /* ��ʼ������Ƕ�������жϿ����� */
  NVIC_Init(&NVIC_InitStructure); 
  
  /* ��ʱ����������ʼ�� */		 
  /* ��������Ϊ0xFFFF������Ҫ�õ� */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
  /* ����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz,�������Ƶ�ʣ�72MHz/4=18MHz */
  /* ͬһ����ʱ���Ĳ�ͬͨ��PWM��Ƶ��ֻ������ͬ�ģ���ͬͨ����������Ϊ��ͬ��ռ�ձ� */
  TIM_TimeBaseStructure.TIM_Prescaler =4-1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
  /* ����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���) */
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
  /* ���ϼ���ģʽ */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TB6600_TIMx, &TIM_TimeBaseStructure);

  /* ��ʱ�����ͨ��1ģʽ���� */
  /* ģʽ���ã�PWMģʽ1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
  /* ���״̬���ã�ʹ����� */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
  /* ��������ֵ�������������������ֵʱ����ƽ�������� */
  TIM_OCInitStructure.TIM_Pulse = prescaler;
  /* ����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ */
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  /* ��ʼ����ʱ��ͨ��1���PWM */
  TIM_OC1Init(TB6600_TIMx, &TIM_OCInitStructure);
  
  /* ��ʱ���Ƚ����ͨ��1Ԥװ�����ã�ʹ��Ԥװ�� */
  TIM_OC1PreloadConfig(TB6600_TIMx, TIM_OCPreload_Disable);

  TIM_ITConfig(TB6600_TIMx,TIM_IT_CC1,ENABLE);
  
  /* ʹ�ܶ�ʱ�� */
  TIM_Cmd(TB6600_TIMx, ENABLE);
  
  /* TIM�����ʹ�� */
  TIM_CtrlPWMOutputs(TB6600_TIMx, ENABLE);
}

/**
  * ��������: TIMx ���PWM�źų�ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����ֻҪ�����������TIMx���ĸ�ͨ���ͻ���PWM�ź����
  */
void TB6600_TIMx_PWM_Init(void)
{
	TB6600_GPIO_Config();
	TB6600_TIMx_Configuration();	
}

/* ��ʱ�Ƚ��жϷ����� */
void TIM1_CC_IRQHandler(void)
{
  if(TIM_GetITStatus(TB6600_TIMx,TIM_IT_CC1)!=RESET)
  {
    TIM_ClearITPendingBit(TB6600_TIMx,TIM_IT_CC1);
    TIM_SetCounter(TB6600_TIMx,0);
    TIM_SetCompare1(TB6600_TIMx,prescaler);
  }
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
