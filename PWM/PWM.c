#include "LPC845.h"

void pwm_init(void){
    SYSCON->SYSAHBCLKCRTL0 |= (1 << 8); // clock enable SCT
    SYSCON->PRESETCTRL0 &= ~(1 << 8); // Reset values 
    SCTIMER->UNIFY = 1;
}
