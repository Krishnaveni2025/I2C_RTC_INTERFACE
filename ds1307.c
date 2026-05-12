#include "ds1307.h"
#include "i2c_bitbang.h"

#define DS1307_WRITE_ADDR (DS1307_I2C_ADDR << 1)
#define DS1307_READ_ADDR  ((DS1307_I2C_ADDR << 1) | 1)

// Helper function to convert BCD to decimal
static uint8_t BCD_to_Dec(uint8_t bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

// Helper function to convert decimal to BCD
static uint8_t Dec_to_BCD(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

bool DS1307_Init(void) {
    I2C_Init();
    
    // Read the seconds register to check the CH (Clock Halt) bit
    I2C_Start();
    if (I2C_Write(DS1307_WRITE_ADDR)) {
        I2C_Stop();
        return false; // NACK
    }
    if (I2C_Write(0x00)) { // Register 0 (Seconds)
        I2C_Stop();
        return false;              
    }
    
    I2C_Start(); // Restart
    if (I2C_Write(DS1307_READ_ADDR)) {
        I2C_Stop();
        return false;  // NACK
    }
    uint8_t sec_reg = I2C_Read(I2C_NACK);
    I2C_Stop();
    
    // If CH bit (bit 7) is 1, the clock is halted. Clear it to start the clock.
    if (sec_reg & 0x80) {
        sec_reg &= 0x7F; // Clear CH bit
        
        I2C_Start();
        if (I2C_Write(DS1307_WRITE_ADDR)) {
            I2C_Stop();
            return false;
        }
        if (I2C_Write(0x00)) {
            I2C_Stop();
            return false;
        }
        if (I2C_Write(sec_reg)) {
            I2C_Stop();
            return false;
        }
        I2C_Stop();
    }
    
    return true;
}

bool DS1307_SetTime(const rtc_time_t* time) {
    I2C_Start();
    if (I2C_Write(DS1307_WRITE_ADDR)) {
        I2C_Stop();
        return false;
    }
    
    if (I2C_Write(0x00)) { // Start at register 0
        I2C_Stop();
        return false;
    }
    
    I2C_Write(Dec_to_BCD(time->seconds) & 0x7F); // Ensure CH bit is 0
    I2C_Write(Dec_to_BCD(time->minutes));
    I2C_Write(Dec_to_BCD(time->hours));          // 24-hour format
    I2C_Write(Dec_to_BCD(time->day));
    I2C_Write(Dec_to_BCD(time->date));
    I2C_Write(Dec_to_BCD(time->month));
    I2C_Write(Dec_to_BCD(time->year));
    
    I2C_Stop();
    return true;
}

bool DS1307_GetTime(rtc_time_t* time) {
    I2C_Start();
    if (I2C_Write(DS1307_WRITE_ADDR)) {
        I2C_Stop();
        return false;
    }
    
    if (I2C_Write(0x00)) { // Start at register 0
        I2C_Stop();
        return false;
    }
    
    I2C_Start(); // Restart
    if (I2C_Write(DS1307_READ_ADDR)) {
        I2C_Stop();
        return false;
    }
    
    time->seconds = BCD_to_Dec(I2C_Read(I2C_ACK) & 0x7F);
    time->minutes = BCD_to_Dec(I2C_Read(I2C_ACK));
    
    uint8_t hours_reg = I2C_Read(I2C_ACK);
    // Assuming 24-hour mode (bit 6 is 0)
    time->hours = BCD_to_Dec(hours_reg & 0x3F);
    
    time->day   = BCD_to_Dec(I2C_Read(I2C_ACK));
    time->date  = BCD_to_Dec(I2C_Read(I2C_ACK));
    time->month = BCD_to_Dec(I2C_Read(I2C_ACK));
    time->year  = BCD_to_Dec(I2C_Read(I2C_NACK)); // NACK for last read
    
    I2C_Stop();
    return true;
}
