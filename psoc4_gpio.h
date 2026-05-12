#ifndef PSOC4_GPIO_H
#define PSOC4_GPIO_H

#include <stdint.h>

#define CYREG_GPIO_BASE      0x40040000U
#define GPIO_PRT_OFFSET(prt) (0x100U * (prt))

#define GPIO_PRT_DR(prt)     (*(volatile uint32_t *)(CYREG_GPIO_BASE + GPIO_PRT_OFFSET(prt) + 0x00U))
#define GPIO_PRT_PS(prt)     (*(volatile uint32_t *)(CYREG_GPIO_BASE + GPIO_PRT_OFFSET(prt) + 0x04U))
#define GPIO_PRT_PC(prt)     (*(volatile uint32_t *)(CYREG_GPIO_BASE + GPIO_PRT_OFFSET(prt) + 0x08U))

// Drive modes (3 bits per pin in PC register)
#define GPIO_DM_HIGHZ_ANALOG 0x00U
#define GPIO_DM_HIGHZ_DIG    0x01U
#define GPIO_DM_PULLUP       0x02U
#define GPIO_DM_PULLDOWN     0x03U
#define GPIO_DM_OD_LO        0x04U
#define GPIO_DM_OD_HI        0x05U
#define GPIO_DM_STRONG       0x06U
#define GPIO_DM_PULLUP_DOWN  0x07U

// Helper macro to set drive mode for a specific pin
#define GPIO_SET_DRIVE_MODE(prt, pin, mode) \
    do { \
        uint32_t temp = GPIO_PRT_PC(prt); \
        temp &= ~(0x07U << ((pin) * 3)); \
        temp |= ((mode) << ((pin) * 3)); \
        GPIO_PRT_PC(prt) = temp; \
    } while(0)

// Helper macros to write/read pins
#define GPIO_WRITE_PIN(prt, pin, val) \
    do { \
        if (val) { \
            GPIO_PRT_DR(prt) |= (1U << (pin)); \
        } else { \
            GPIO_PRT_DR(prt) &= ~(1U << (pin)); \
        } \
    } while(0)

#define GPIO_READ_PIN(prt, pin) ((GPIO_PRT_PS(prt) >> (pin)) & 0x01U)

#endif // PSOC4_GPIO_H
