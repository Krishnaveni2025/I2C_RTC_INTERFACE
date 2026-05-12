#include "i2c_bitbang.h"
#include "psoc4_gpio.h"

// Approximate microsecond delay for 24MHz clock
static void delay_us(uint32_t us) {
    for (volatile uint32_t i = 0; i < (us * 3); i++) {
        __asm("nop");
    }
}

#define I2C_DELAY delay_us(5) // ~5us for 100kHz standard mode

void I2C_Init(void) {
    // Configure SCL and SDA as Open Drain, Drives Low
    GPIO_SET_DRIVE_MODE(I2C_PORT, I2C_SCL_PIN, GPIO_DM_OD_LO);
    GPIO_SET_DRIVE_MODE(I2C_PORT, I2C_SDA_PIN, GPIO_DM_OD_LO);
    
    // Set output to High-Z (External pull-ups will pull the line HIGH)
    GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 1);
    GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 1);
    I2C_DELAY;
}

void I2C_Start(void) {
    // Both lines should be HIGH initially
    GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 1);
    GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 1);
    I2C_DELAY;
    
    // SDA goes LOW while SCL is HIGH
    GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 0);
    I2C_DELAY;
    
    // SCL goes LOW
    GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 0);
    I2C_DELAY;
}

void I2C_Stop(void) {
    // Ensure SDA is LOW while SCL is LOW
    GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 0);
    I2C_DELAY;
    
    // SCL goes HIGH
    GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 1);
    I2C_DELAY;
    
    // Wait for SCL to actually go high (clock stretching support)
    while (GPIO_READ_PIN(I2C_PORT, I2C_SCL_PIN) == 0);
    
    // SDA goes HIGH while SCL is HIGH
    GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 1);
    I2C_DELAY;
}

bool I2C_Write(uint8_t data) {
    bool nack;
    
    for (int i = 0; i < 8; i++) {
        // Output data bit
        if (data & 0x80) {
            GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 1);
        } else {
            GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 0);
        }
        data <<= 1;
        I2C_DELAY;
        
        // Clock HIGH
        GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 1);
        while (GPIO_READ_PIN(I2C_PORT, I2C_SCL_PIN) == 0); // Clock stretching
        I2C_DELAY;
        
        // Clock LOW
        GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 0);
    }
    
    // Release SDA to read ACK
    GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 1);
    I2C_DELAY;
    
    // Clock HIGH
    GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 1);
    while (GPIO_READ_PIN(I2C_PORT, I2C_SCL_PIN) == 0); // Clock stretching
    I2C_DELAY;
    
    // Read ACK/NACK (0 = ACK, 1 = NACK)
    nack = GPIO_READ_PIN(I2C_PORT, I2C_SDA_PIN);
    
    // Clock LOW
    GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 0);
    I2C_DELAY;
    
    return nack;
}

uint8_t I2C_Read(bool ack) {
    uint8_t data = 0;
    
    // Release SDA
    GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 1);
    
    for (int i = 0; i < 8; i++) {
        data <<= 1;
        I2C_DELAY;
        
        // Clock HIGH
        GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 1);
        while (GPIO_READ_PIN(I2C_PORT, I2C_SCL_PIN) == 0); // Clock stretching
        I2C_DELAY;
        
        // Read bit
        if (GPIO_READ_PIN(I2C_PORT, I2C_SDA_PIN)) {
            data |= 1;
        }
        
        // Clock LOW
        GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 0);
    }
    
    // Output ACK/NACK
    if (ack == I2C_ACK) {
        GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 0); // ACK
    } else {
        GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 1); // NACK
    }
    I2C_DELAY;
    
    // Clock HIGH
    GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 1);
    while (GPIO_READ_PIN(I2C_PORT, I2C_SCL_PIN) == 0); // Clock stretching
    I2C_DELAY;
    
    // Clock LOW
    GPIO_WRITE_PIN(I2C_PORT, I2C_SCL_PIN, 0);
    
    // Release SDA
    GPIO_WRITE_PIN(I2C_PORT, I2C_SDA_PIN, 1);
    I2C_DELAY;
    
    return data;
}
