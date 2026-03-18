#include "LPC845.h"

#define PWM_PERIOD  1000
#define PWM_DUTY    500
#define PWM_PIN     10  // PIO0_10

void pwm_init(void) {
    // Enable clocks
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 8)   // SCT clock
                           |  (1 << 7);  // SWM clock
    
    // Reset SCT
    SYSCON->PRESETCTRL0 &= ~(1 << 8);
    SYSCON->PRESETCTRL0 |=  (1 << 8);
    
    // Configure SCT: unified 32-bit counter, auto-limit
    SCT0->CONFIG = (1 << 0)      // UNIFY = 1 (32-bit counter)
                 | (1 << 17);    // AUTOLIMIT_L (reset at MATCH0)
    
    // Stop and clear counter
    SCT0->CTRL |= (1 << 2);      // HALT
    SCT0->CTRL |= (1 << 3);      // CLRCTR
    
    // Set match values
    SCT0->MATCHREL[0] = PWM_PERIOD - 1;  // Period
    SCT0->MATCHREL[1] = PWM_DUTY;        // Duty cycle
    
    // Event 0: Match 0 → SET output (start of period)
    SCT0->EV[0].STATE = 0xFFFFFFFF;
    SCT0->EV[0].CTRL  = (0 << 0) | (1 << 12); // Match 0 with timer condition only
    
    // Event 1: Match 1 → CLEAR output (end of duty)
    SCT0->EV[1].STATE = 0xFFFFFFFF;
    SCT0->EV[1].CTRL  = (1 << 0) | ( 1 << 12;  // Match 1 with timer condition only
    
    // Output 0: SET on EV0, CLEAR on EV1
    SCT0->OUT[0].SET = (1 << 0); // when event 0 happens set  
    SCT0->OUT[0].CLR = (1 << 1); // when event 1 happens clear
    
    // Route SCT_OUT0 to pin PIO0_10
    // PINASSIGN7: SCT_OUT0_O is bits [31:24] (not [23:16]!)
    SWM0->PINASSIGN_7 = (SWM0->PINASSIGN_7 & 0x00FFFFFF) | (PWM_PIN << 24);
    
    // Start counter
    SCT0->CTRL &= ~(1 << 2);
}

// i should complete this by the end of the day, kindof.
void pwm_set_duty(uint32_t duty) {
    if (duty > PWM_PERIOD) duty = PWM_PERIOD;
    SCT0->MATCHREL[1] = duty;
}

int main(void) {
    pwm_init();
    
    while (1) {
        // PWM running
    }
}
