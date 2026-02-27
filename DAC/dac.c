#include "LPC845.h"

void DAC_Init(void){
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 28);
    IOCON->PIO[17] |= (1 << 16);   // disable digital mode for analog
    SYSCON->PDRUNCFG &= ~(1 << 13);
    SWM0->PINENABLE0 &= ~(1 << 26);
    DAC0->CTRL = 0;
}

void DAC_Write(uint16_t value)
{
    value &= 0x3FF;
    DAC0->CR = (value << 6);
}
