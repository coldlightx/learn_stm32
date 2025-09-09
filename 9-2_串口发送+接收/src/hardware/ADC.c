#include "stm32f10x.h"
#include "ADC.h"

ADC_DATA adc_data;

uint16_t AD_DATA[4];

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

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_InitTypeDef gpio_config;
    gpio_config.GPIO_Mode  = GPIO_Mode_AIN;
    gpio_config.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    gpio_config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_config);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_1Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_1Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_1Cycles5);

    ADC_InitTypeDef adc_config;
    adc_config.ADC_Mode               = ADC_Mode_Independent;
    adc_config.ADC_DataAlign          = ADC_DataAlign_Right;
    adc_config.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
    adc_config.ADC_ScanConvMode       = ENABLE;
    adc_config.ADC_ContinuousConvMode = ENABLE;
    adc_config.ADC_NbrOfChannel       = 4;
    ADC_Init(ADC1, &adc_config);

    DMA_InitTypeDef dma_config;

    dma_config.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    dma_config.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma_config.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    dma_config.DMA_MemoryBaseAddr     = (uint32_t)AD_DATA;
    dma_config.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    dma_config.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    dma_config.DMA_BufferSize         = 4;
    dma_config.DMA_DIR                = DMA_DIR_PeripheralSRC;
    dma_config.DMA_M2M                = DMA_M2M_Disable; // 使用外部触发源
    dma_config.DMA_Mode               = DMA_Mode_Circular;
    dma_config.DMA_Priority           = DMA_Priority_High;
    DMA_Init(DMA1_Channel1, &dma_config);

    ADC_Cmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_calibration();
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // 保证转换并搬运完成一次
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) != SET);
}