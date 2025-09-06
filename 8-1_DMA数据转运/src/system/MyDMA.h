#if !defined(__MYDMA_H)
#define __MYDMA_H
void MyDMA_Init(uint32_t source, uint32_t target, uint16_t length);
void MyDMA_Transfer(uint16_t size);

#endif // __MYDMA_H
