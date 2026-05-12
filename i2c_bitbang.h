#ifndef I2C_BITBANG_H
#define I2C_BITBANG_H

#include <stdint.h>
#include <stdbool.h>

// I2C Port and Pins (Defaulting to Port 3, Pins 0 and 1)
// Adjust these macros if you have wired the DS1307 to different pins
#define I2C_PORT 3
#define I2C_SCL_PIN 0
#define I2C_SDA_PIN 1

// ACK/NACK definitions
#define I2C_ACK  0
#define I2C_NACK 1

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
bool I2C_Write(uint8_t data);
uint8_t I2C_Read(bool ack);

#endif // I2C_BITBANG_H
