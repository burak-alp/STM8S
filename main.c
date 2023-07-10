/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_clk.h"
#include "stm8s_gpio.h"
#include "stm8s_adc1.h"
/* Private defines -----------------------------------------------------------*/
#define PORT_LED GPIOC
#define PIN_LED GPIO_PIN_3
#define NUM_BLINKS 3

/* Uncomment to set the delay behaviouor of the program to be active-halt */
volatile uint16_t adcValue;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void delay(uint32_t milliseconds) {
   while(milliseconds--);
}
void multi_blink(uint8_t num) {
  for(uint8_t i=0; i<num; i++) {
    GPIO_WriteHigh(PORT_LED, PIN_LED);
    delay(50);
    GPIO_WriteLow(PORT_LED, PIN_LED);
    delay(50);
  }
}
void ADC_Halt_Blink(void)
{
 
  //ADC_Single();         // single ADC read
  ADC1_StartConversion();
  adcValue=ADC1_GetConversionValue();
  ADC1_ClearFlag((ADC1_Flag_TypeDef)ADC1_IT_EOC);
  
  if(adcValue<=100) {
    multi_blink(NUM_BLINKS);
    ADC1_StartConversion();
    adcValue=ADC1_GetConversionValue();
    ADC1_ClearFlag((ADC1_Flag_TypeDef)ADC1_IT_EOC);
    for(int i=0;i<60;i++){
      halt(); /* Program halted */
      }
    }
  else{
     ADC1_StartConversion();
    adcValue=ADC1_GetConversionValue();
    ADC1_ClearFlag((ADC1_Flag_TypeDef)ADC1_IT_EOC);
    for(int j=0;j<2;j++){
    halt();
      }
    }
  }
/**
  * @brief  Configure the AWU time base to 12s
  * @param  None
  * @retval None
  */
void AWU_Config(void)
{
  /* Initialization of AWU */
  /* LSI calibration for accurate auto wake up time base*/
  AWU_LSICalibrationConfig(LSI_VALUE);  
  
  /* The delay corresponds to the time we will stay in Halt mode */
  AWU_Init(AWU_TIMEBASE_2S);
  AWU_Cmd(ENABLE);
}

void main(void)
{
  //CLK_DeInit();
  CLK_HSECmd(DISABLE);
  CLK_LSICmd(ENABLE);
  CLK_HSICmd(DISABLE);
  
  //CLK_ClockSwitchCmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV128);
  
  /* GPIO Config */
  GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
  GPIO_WriteLow(GPIOC,GPIO_PIN_3);
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);
  
  //Awu config
  AWU_Config();
  
  /* ADC Config */
  ADC1_DeInit();
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_6, ADC1_PRESSEL_FCPU_D2, 
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL6,
            DISABLE);
  ADC1_Cmd(ENABLE);
 
  /* Infinite loop */
  while (1) {
    
    ADC_Halt_Blink();
  }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
