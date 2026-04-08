#include "LPC845.h"

#define PWM_PERIOD  100000
#define PWM_DUTY    50000
#define PWM_PIN     15  // PIO0_15

#include "LPC845.h"

// 6 PWM outputs using only 7 events (1 shared period + 6 duty events)
void pwm_init_6ch(void) {
    // Enable clocks
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 8) | (1 << 7) | (1 << 6);
    SYSCON->PRESETCTRL0 &= ~(1 << 8);
    SYSCON->PRESETCTRL0 |=  (1 << 8);
    
    // Set 6 pins as outputs (example: PIO0_15, 16, 17, 18, 19, 20)
    GPIO->DIR[0] |= (0x1F << 15);  // Pins 15-20
    
    // Unified 32-bit, auto-limit on MATCH0
    SCT0->CONFIG = (1 << 0) | (1 << 17);
    
    // Shared period for all PWMs
    SCT0->MATCHREL[0] = PWM_PERIOD - 1;  // MATCH0 = period
    
    // Individual duty cycles (can be updated independently)
    SCT0->MATCHREL[1] = duty1;  // PWM1 duty
    SCT0->MATCHREL[2] = duty2;  // PWM2 duty  
    SCT0->MATCHREL[3] = duty3;  // PWM3 duty
    SCT0->MATCHREL[4] = duty4;  // PWM4 duty
    SCT0->MATCHREL[5] = duty5;  // PWM5 duty
    SCT0->MATCHREL[6] = duty6;  // PWM6 duty
    
    SCT0->CTRL = (1 << 2) | (1 << 3);  // Halt & clear
    
    // Event 0 (Period): MATCH0 → SET all outputs high, reset counter
    SCT0->EV[0].STATE = 0xFFFFFFFF;
    SCT0->EV[0].CTRL  = (0 << 0) | (1 << 12);  // Match 0 only
    
    // Events 1-6: Individual duty matches → CLEAR respective output
    SCT0->EV[1].STATE = 0xFFFFFFFF;
    SCT0->EV[1].CTRL  = (1 << 0) | (1 << 12);  // Match 1
    
    SCT0->EV[2].STATE = 0xFFFFFFFF;
    SCT0->EV[2].CTRL  = (2 << 0) | (1 << 12);  // Match 2
    
    SCT0->EV[3].STATE = 0xFFFFFFFF;
    SCT0->EV[3].CTRL  = (3 << 0) | (1 << 12);  // Match 3
    
    SCT0->EV[4].STATE = 0xFFFFFFFF;
    SCT0->EV[4].CTRL  = (4 << 0) | (1 << 12);  // Match 4
    
    SCT0->EV[5].STATE = 0xFFFFFFFF;
    SCT0->EV[5].CTRL  = (5 << 0) | (1 << 12);  // Match 5
    
    SCT0->EV[6].STATE = 0xFFFFFFFF;
    SCT0->EV[6].CTRL  = (6 << 0) | (1 << 12);  // Match 6
    
    // Configure outputs: All SET on EV0 (period), each CLEAR on respective EV
    SCT0->OUT[0].SET = (1 << 0);  SCT0->OUT[0].CLR = (1 << 1);  // EV1 clears OUT0
    SCT0->OUT[1].SET = (1 << 0);  SCT0->OUT[1].CLR = (1 << 2);  // EV2 clears OUT1
    SCT0->OUT[2].SET = (1 << 0);  SCT0->OUT[2].CLR = (1 << 3);  // EV3 clears OUT2
    SCT0->OUT[3].SET = (1 << 0);  SCT0->OUT[3].CLR = (1 << 4);  // EV4 clears OUT3
    SCT0->OUT[4].SET = (1 << 0);  SCT0->OUT[4].CLR = (1 << 5);  // EV5 clears OUT4
    SCT0->OUT[5].SET = (1 << 0);  SCT0->OUT[5].CLR = (1 << 6);  // EV6 clears OUT5
    
    // Route outputs via SWM (SCT_OUT0-5 to pins of your choice)
    // See PINASSIGN7, PINASSIGN8, PINASSIGN9 registers
    
    SCT0->CTRL = 0;  // Start
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
