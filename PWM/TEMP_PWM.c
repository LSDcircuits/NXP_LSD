#include "LPC845.h"

#define PWM_PERIOD 1000
#define PWM_DUTY   500
// fml it will be gfreat t be backt9 tgh
void pwm_init(uint8_t duty) {
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 8); // clock enable
    SYSCON->PRESETCTRL0 &= ~(1 << 8); // disable
    SYSCON->PRESETCTRL0 |=  (1 << 8); // enable
    
    SCT0->CONFIG = (1 << 0); // countr 32 bit
    SCT0->CTRL &= ~(1 << 2); // use system clcok

    SCT0->MATCHREL[0].U = PWM_PERIOD;   // Period      | MATCHREL0 TABLE409
    SCT0->MATCHREL[1].U = PWM_DUTY;     // Duty cycle  | 

    // EVENT 0
    SCT0->EV[0].STATE = 0xFFFFFFFF;
    SCT0->EV[0].CTRL  = (0 << 0) | (1 << 12);  

    // EVENY 1 
    SCT0->EV[1].STATE = 0xFFFFFFFF;
    SCT0->EV[1].CTRL  = (1 << 0);

    // Output 0 SET on event
    SCT0->OUT[0].SET = (1 << 0);

    // Output 0 CLEAR on event1 
    SCT0->OUT[0].CLR = (1 << 1);

    // Start counter
    SCT0->CTRL &= ~(1 << 2);
}

void PWM_setup(uint32_t period){
    
}
