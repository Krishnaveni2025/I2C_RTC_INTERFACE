#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "ds1307.h"

// Semihosting initialization
extern void initialise_monitor_handles(void);

// Provide a simple loop delay for main loop (approximate)
void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < (ms * 3000); i++) {
    }
}

// Global struct to inspect in debugger
volatile rtc_time_t current_time;

int main(void) {
    // Initialize Semihosting for printf over OpenOCD
    initialise_monitor_handles();
    printf("Starting Baremetal I2C RTC...\n");

    // Initialize DS1307 (this also initializes I2C pins)
    // and clears the Clock Halt bit if needed to start the oscillator
    bool rtc_ready = DS1307_Init();
    
    if (rtc_ready) {
        // We are setting the initial time here!
        // Once this runs and the time is set, you should comment this block out again
        // so it doesn't reset your clock every time the board restarts.
        rtc_time_t init_time = {
            .seconds = 0,
            .minutes = 59,
            .hours = 15,
            .day = 3,
            .date = 12,
            .month = 5,
            .year = 26
        };
        DS1307_SetTime(&init_time);
    }

    while (1) {
        if (rtc_ready) {
            // Read time into the volatile global variable
            // so you can pause the debugger and inspect it
            DS1307_GetTime((rtc_time_t*)&current_time);
            
            printf("Current Time: 20%02d-%02d-%02d %02d:%02d:%02d\n", 
                   current_time.year, current_time.month, current_time.date,
                   current_time.hours, current_time.minutes, current_time.seconds);
        }
        
        // Wait ~1 second before polling again
        delay_ms(1000);
    }
}
