#include <stm32f10x.h>

void MyDMA_Init(uint32_t source, uint32_t target, uint16_t length)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef dma_config;
    dma_config.DMA_PeripheralBaseAddr = source;
    dma_config.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma_config.DMA_PeripheralInc      = DMA_PeripheralInc_Enable;
    dma_config.DMA_MemoryBaseAddr     = target;
    dma_config.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
    dma_config.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    dma_config.DMA_BufferSize         = length;
    dma_config.DMA_DIR                = DMA_DIR_PeripheralSRC;
    dma_config.DMA_M2M                = DMA_M2M_Enable;
    dma_config.DMA_Mode               = DMA_Mode_Normal;
    dma_config.DMA_Priority           = DMA_Priority_Medium;

    DMA_Init(DMA1_Channel1, &dma_config);
    DMA_Cmd(DMA1_Channel1, DISABLE);
}

void MyDMA_Transfer(uint16_t size)
{
    DMA_Cmd(DMA1_Channel1, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1, size);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) != SET);
}