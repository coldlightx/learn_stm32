#if !defined(MY_STORE_H_)
#define MY_STORE_H_

void MyStore_save();
void MyStore_init();
void MyStore_clear();
uint16_t MyStore_get_key();
extern uint16_t MyStore_cache[];

#endif // MY_STORE_H_
