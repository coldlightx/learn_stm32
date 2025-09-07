#include "stm32f10x.h"

void ADC_calibration()
{
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));

    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
}

void ADC_init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Mode  = GPIO_Mode_AIN;
    gpio_config.GPIO_Pin   = GPIO_Pin_0;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_config);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);

    ADC_InitTypeDef adc_config;
    adc_config.ADC_Mode               = ADC_Mode_Independent;
    adc_config.ADC_DataAlign          = ADC_DataAlign_Right;
    adc_config.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
    adc_config.ADC_ScanConvMode       = DISABLE;
    adc_config.ADC_ContinuousConvMode = ENABLE;
    adc_config.ADC_NbrOfChannel       = 1;
    ADC_Init(ADC1, &adc_config);
    ADC_Cmd(ADC1, ENABLE);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // 保证转换完成一次
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);

    ADC_calibration();
}

uint16_t ADC_get_value()
{
    return ADC_GetConversionValue(ADC1);
}