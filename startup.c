#include <stdint.h>

extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

extern int main(void);

void Reset_Handler(void) {
    uint32_t *src, *dest;

    // Copy .data section from FLASH to RAM
    src = &_etext;
    for (dest = &_sdata; dest < &_edata; ) {
        *dest++ = *src++;
    }

    // Zero-initialize .bss section
    for (dest = &_sbss; dest < &_ebss; ) {
        *dest++ = 0;
    }

    // Branch to main
    main();
    
    // Fallback infinite loop
    while (1);
}

void Default_Handler(void) {
    while (1);
}

// Cortex-M0+ Vector Table
__attribute__((section(".isr_vector"), used))
void (*const g_pfnVectors[])(void) = {
    (void (*)(void))((uint32_t)&_estack), // Initial Stack Pointer
    Reset_Handler,                        // Reset Handler
    Default_Handler,                      // NMI Handler
    Default_Handler,                      // Hard Fault Handler
    0, 0, 0, 0, 0, 0, 0,                  // Reserved
    Default_Handler,                      // SVC Handler
    0, 0,                                 // Reserved
    Default_Handler,                      // PendSV Handler
    Default_Handler                       // SysTick Handler
    // IRQs can be added here if needed
};
