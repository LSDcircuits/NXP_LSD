# NXP_LSD
This repository is dedicated to using the LPC84X series.
Made for the Purpose of having a simpler approach to programming
Interfaces and low frequency control systems (<1MHZ).


Functions:

PWM    X base register 0x5000 8000
CTIMER √
GPIO   √
DAC    √
ADC    √
I2C    x


Accesing Hardware regs:

1. Power: PDRUNCFG (power up analog blocks like ADC/DAC, sometimes FRO/PLL pieces)
2. Clock gate: SYSAHBCLKCTRL0/1 (enable the peripheral’s bus clock)
3. Reset (optional but recommended): PRESETCTRL0/1 (toggle peripheral reset for clean state)
4. Clock source/divider (if applicable): e.g. ADCCLKSEL, ADCCLKDIV, FRG, etc.
5. Pin function routing (chip-dependent):
6. SWM for movable functions: PINASSIGNx and/or PINENABLE0
7. Some functions are fixed and don’t use PINASSIGN (depends on peripheral)
8. Pin electrical config: IOCON (pull ups, digital/analog mode, hysteresis, filter, OD, etc.)
9. Peripheral registers: configure and run (CTRL/CFG/CR/SEQ/etc.)
