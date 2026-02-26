#include "LPC845.h"


void DAC_Init(void){
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 28);
    IOCON->PIO[17] &= ~(1 << 17);   
    SYSCON->PDRUNCFG &= ~(1 << 13);
}

// System clock control
// SYSAHBCLKCTRL0 (ADD 0x4004 8080) (UM_CH_8_Table_147)
// SYSAHBCLKCTRL1 (ADD 0x4004 8084) (UM_CH_8_Table_148)
// PERIPHERAL CLOCK SOURCE (UM_CH_8_Table_151)

// Power configuration
// PDRUNCFG (ADD 0x4004 8238) (UM_CH_8_Table_173)

// Preset control
// PRESETCTRL0 (ADD 0x4004 8088) (UM_CH_8_Table_149)
// PRESETCTRL1 (ADD 0x4004 808C) (UM_CH_8_Table_150)
