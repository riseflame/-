/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 使用JTAG功能引脚作为普通IO
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
#include "stm32f10x.h"
#include "bsp/57StepMotor/bsp_TB6600_TIM.h" 
#include "bsp/key/bsp_key.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
uint8_t dir=0; // 0 ：顺时针   1：逆时针 
uint8_t ena=0; // 0 ：正常运行 1：停机

/* 扩展变量 ------------------------------------------------------------------*/
extern __IO uint16_t prescaler; /* 步进电机速度控制，可调节范围为 650 -- 3500 ，值越小速度越快 */

/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 主函数.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
int main(void)
{  
  uint8_t key1_count=1;
  /* 初始化按键 */
  KEY_GPIO_Init();
  
  /* 初始化定时器PWM输出 */
  TB6600_TIMx_PWM_Init();    
  
  /* 无限循环 */
  while (1)    
  {
    if(KEY1_StateRead()==KEY_DOWN)  // 增速
    {
       key1_count++;
       if(key1_count==5)
         key1_count=1;
    }
    if(KEY2_StateRead()==KEY_DOWN)  // 减速
    {
      switch(key1_count)
      {
        case 1:
          prescaler-=50;
          if(prescaler<300)  // 最快速度限制
            prescaler=300;
          break;
        case 2:
          prescaler+=100;
          if(prescaler>3000)         // 最慢速度限制
            prescaler=3000;
          break;  
        case 3:                // 方向控制
          if(dir==0)
          {
            GPIO_SetBits(TB6600_DIR_PORT,TB6600_DIR_PIN);  // 逆时针
            dir=1;
          }
          else
          {
            GPIO_ResetBits(TB6600_DIR_PORT,TB6600_DIR_PIN);  // 顺时针
            dir=0;
          }
          break;  
        case 4:                // 使能控制
          if(ena==0)
          {
            GPIO_SetBits(TB6600_ENA_PORT,TB6600_ENA_PIN); // 停机
            ena=1;
          }
          else
          {
            GPIO_ResetBits(TB6600_ENA_PORT,TB6600_ENA_PIN);  // 正常运行
            ena=0;
          }
          break;
        default:
          break;          
      }
    }      
  }
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
