# I2C_RTC_INTERFACE
# PSoC 4100S Plus Baremetal I2C RTC (DS1307)

This repository contains a purely baremetal, bit-banged I2C implementation to interface a Cypress/Infineon **PSoC 4100S Plus** (CY8CKIT-149) with a **DS1307 Real-Time Clock (RTC)** module.

It leverages direct GPIO register manipulation without any vendor-specific HAL (Hardware Abstraction Layer) libraries, providing a highly educational example of baremetal embedded development on the ARM Cortex-M0+ architecture.

## Features
- **Baremetal I2C Driver:** A custom bit-banged I2C driver (`i2c_bitbang.c`) capable of ~100kHz standard mode operation.
- **DS1307 Driver:** Abstraction for reading/writing BCD (Binary Coded Decimal) time data, enabling the oscillator, and interacting with the RTC's registers.
- **ARM Semihosting:** Uses OpenOCD semihosting to route `printf()` output from the microcontroller natively into the VS Code Debug Console.
- **Custom Build System:** A standalone `Makefile` and custom linker script (`psoc4_linker.ld`) tailored for the 128KB Flash / 16KB SRAM limits of the CY8C4147AZI-S475.
- **VS Code Ready:** Includes `.vscode/tasks.json` and `.vscode/launch.json` for seamless 1-click building and flashing via the `Cortex-Debug` extension.

## Hardware Connections

Connect the DS1307 module to the CY8CKIT-149 (PSoC 4100S Plus) as follows:

| PSoC 4 Pin | DS1307 Pin | Description |
| :--- | :--- | :--- |
| **P3.0** | **SCL** | Serial Clock |
| **P3.1** | **SDA** | Serial Data |
| **5V** | **VCC** | Power (DS1307 requires 5V nominal) |
| **GND** | **GND** | Common Ground |

> **⚠️ IMPORTANT:** The I2C protocol requires pull-up resistors! If your DS1307 module does not have them built-in, you must physically connect a 4.7kΩ resistor between SCL and 5V, and another 4.7kΩ resistor between SDA and 5V.

## Prerequisites
- **VS Code** with the [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) extension.
- **Arm GNU Toolchain** (`arm-none-eabi-gcc`) installed and added to your system `PATH`.
- **Make** installed and added to your system `PATH`.
- **OpenOCD** installed (usually configured in `C:/OpenOCD/`).

## How to Build and Run

### Option 1: Visual Studio Code (Recommended)
1. Open this repository folder in VS Code.
2. Press `Ctrl + Shift + B` to build the firmware via the predefined Make task.
3. Switch to the **Run and Debug** tab on the left sidebar.
4. Select **"Cortex Debug (OpenOCD)"** from the dropdown menu.
5. Click the green Play button (or press `F5`).
6. The firmware will flash automatically. Open the **Debug Console** in VS Code to see the live RTC time being printed via `printf`!

### Option 2: Command Line
Open a terminal in the project directory:
```bash
# Clean previous builds
make clean

# Compile the firmware
make all

# Flash the firmware via OpenOCD
make flash
```

## Setting the Initial Time
Because the DS1307 loses its time when power is removed (without a coin-cell battery), you may need to initialize it.
1. Open `main.c`.
2. Locate the `rtc_time_t init_time` block.
3. Uncomment it, set your desired initial time, and flash the board.
4. **Crucial:** Once you verify the time is correct in the console, **comment the block back out** and flash the board one final time. This ensures the PSoC doesn't continually overwrite the time with the hardcoded value every time you reset the microcontroller!


<img width="1919" height="1012" alt="image" src="https://github.com/user-attachments/assets/e61401b2-7b6c-4625-b780-d74721d19aea" />



<img width="1536" height="1536" alt="WhatsApp Image 2026-05-13 at 02 10 35" src="https://github.com/user-attachments/assets/f846d3e2-315d-4b35-9225-af82b6c5deed" />
