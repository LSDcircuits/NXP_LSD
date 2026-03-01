#include "LPC845.h"

void DAC_Init(void){
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 28); // enable clock for DAC block (UM_CH_8_Table_147)
    IOCON->PIO[17] |= (1 << 16);        // disable digital on Pin17  
    SYSCON->PDRUNCFG &= ~(1 << 13);    // Power the Anlog device (UM_CH_8_Table_173)
    SWM0->PINENABLE0 &= ~(1 << 26);   // Set Switch matrix for DAC (UM_CH_7_Table_196)
    DAC0->CTRL = 0;                  // Set mode (UM_CH_28_Table_471)
}  
void DAC_Write(uint16_t value){
    value &= 0x3FF;
    DAC0->CR = (value << 6); //(UM_CH_28_Table_470)
} 
