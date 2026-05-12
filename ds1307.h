#ifndef DS1307_H
#define DS1307_H

#include <stdint.h>
#include <stdbool.h>

#define DS1307_I2C_ADDR 0x68

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;    // 1-7
    uint8_t date;   // 1-31
    uint8_t month;  // 1-12
    uint8_t year;   // 0-99
} rtc_time_t;

bool DS1307_Init(void);
bool DS1307_SetTime(const rtc_time_t* time);
bool DS1307_GetTime(rtc_time_t* time);

#endif // DS1307_H
