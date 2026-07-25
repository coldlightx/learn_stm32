#if !defined(__MYCAN_H)
#define __MYCAN_H

void MyCAN_init();
void MyCAN_send_message(uint32_t id, uint8_t length, uint8_t *data);
uint8_t MyCAN_message_count();
void MyCAN_receive_message(uint32_t *id, uint8_t *length, uint8_t *data);

#endif // __MYCAN_H
