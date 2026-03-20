#include "LPC845.h"

#define PWM_PERIOD  100000
#define PWM_DUTY    50000
#define PWM_PIN     15  // PIO0_10

void pwm_init(void) {
    // Enable clocks for SCT, SWM, AND GPIO
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 8)   // SCT clock
                           |  (1 << 7)   // SWM clock
                           |  (1 << 6);  // GPIO clock ← ADD THIS!
    // Reset SCT
    SYSCON->PRESETCTRL0 &= ~(1 << 8);
    SYSCON->PRESETCTRL0 |=  (1 << 8);
    
    // Configure pin as OUTPUT before routing
    GPIO->DIR[0] |= (1 << PWM_PIN);  // Set PIO0_10 as output ← ADD THIS!
    
    // Configure SCT: unified 32-bit counter, auto-limit
    SCT0->CONFIG = (1 << 0)      // UNIFY = 1 (32-bit counter)
                 | (1 << 17);    // AUTOLIMIT_L (reset at MATCH0)

    SCT0->MATCHREL[0] = PWM_PERIOD - 1;  // Period
    SCT0->MATCHREL[1] = PWM_DUTY;        // Duty cycle
    
    // Stop and clear counter
    SCT0->CTRL = (1 << 2) | (1 << 3);  // HALT | CLRCTR (write both at once)
    
    // Event 0: Match 0 → SET output
    SCT0->EV[0].STATE = 0xFFFFFFFF;
    SCT0->EV[0].CTRL  = (0 << 0) | (1 << 12); 
    
    // Event 1: Match 1 → CLEAR output
    SCT0->EV[1].STATE = 0xFFFFFFFF; 
    SCT0->EV[1].CTRL  = (1 << 0) | (1 << 12);

    // Output 0: SET on EV0, CLEAR on EV1
    SCT0->OUT[0].SET = (1 << 0);
    SCT0->OUT[0].CLR = (1 << 1);
    
    // Route SCT_OUT0 to pin PIO0_10
    SWM0->PINASSIGN.PINASSIGN7 = (SWM0->PINASSIGN.PINASSIGN7 & 0x00FFFFFF) | (PWM_PIN << 24);
    
    // Disable digital filter on pin (optional but recommended)
    // IOCON->PIO0_10 &= ~(0x3 << 3);  // Clear MODE bits if needed
    
    // Start counter
    SCT0->CTRL = 0;  // Clear HALT and CLRCTR
}


void set_pwm(uint32_t duty, uint32_t period) {
    if (period < 2) period = 2;
    if (duty >= period) duty = period - 1;
    
    SCT0->MATCHREL[0] = period - 1;
    SCT0->MATCHREL[1] = duty;
}

void disable_pwm(void) { 
    SCT0->CTRL |= (1 << 2);    // HALT
    SCT0->CTRL |= (1 << 3);    // CLRCTR
}

void enable_pwm(void) {
    SCT0->CTRL &= ~(1 << 2);
}

int main(void) {
    pwm_init();
        while (1) {
        // PWM running in hardware
    }

}
