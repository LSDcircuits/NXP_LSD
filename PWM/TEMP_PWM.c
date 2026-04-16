#include "LPC845.h"

#define CLK_freq 12000000
#define PWM_freq 50
#define Duty_cycle

#define PWM_PERIOD  6000000
#define PWM_DUTY    3000000
#define PWM_PIN1     15  // PIO0_15 hehe 

// use uint32t since LPC845 has no FPU
// Ill need to adjust this for other LPC series which have FPU :)
// another notice is pin1-6 are pwm pins which can be mapped
// In such manner that pin1 can be PIO5 if 5 is assinged to pin1
typedef struct {
    uint32_t pin1;
    uint32_t pin2;
    uint32_t pin3;
    uint32_t pin4;
    uint32_t pin5;
    uint32_t pin6;
    uint32_t period;
} pwm_var;

void pwm_init(pwm_var *pwm_var) {
    // Enable clocks for SCT, SWM, AND GPIO
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 8)   // SCT clock
                           |  (1 << 7)   // SWM clock
                           |  (1 << 6);  // GPIO clock ← ADD THIS!
    // Reset SCT
    SYSCON->PRESETCTRL0 &= ~(1 << 8);
    SYSCON->PRESETCTRL0 |=  (1 << 8);
    // Configure SCT: unified 32-bit counter, auto-limit
    SCT0->CONFIG = (1 << 0)      // UNIFY = 1 (32-bit counter)
                 | (1 << 17);    // AUTOLIMIT_L (reset at MATCH0)

    SCT0->MATCHREL[0] = pwm_var->period - 1;  // Period

    SCT0->MATCHREL[1] = 0;  // Duty set to 0 for startup
    SCT0->MATCHREL[2] = 0;
    SCT0->MATCHREL[3] = 0;
    SCT0->MATCHREL[4] = 0;
    SCT0->MATCHREL[5] = 0;
    SCT0->MATCHREL[6] = 0;

    // Stop and clar counter
    SCT0->CTRL = (1 << 2) | (1 << 3);  // HALT | CLRCTR (write both at once)

    // Event 0: Match 0 → SET output
    SCT0->EV[0].STATE = 0xFFFFFFFF;
    SCT0->EV[0].CTRL  = (0 << 0) | (1 << 12); 
    
    // set events for duty / for my self to correct
    // Event 1: Match 1 → CLEAR output for EV[CTRL] bit 0:3
    // (Reminder) 0. 0000 / 1. 0001 / 2. 0010 / 3. 0011 n.. (n << 0) = 0x0, 0x1, 0x2 .. 
    SCT0->EV[1].STATE = 0xFFFFFFFF; 
    SCT0->EV[1].CTRL  = (1 << 0) | (1 << 12);

    SCT0->EV[2].STATE = 0xFFFFFFFF; 
    SCT0->EV[2].CTRL  = (2 << 0) | (1 << 12);

    SCT0->EV[3].STATE = 0xFFFFFFFF; 
    SCT0->EV[3].CTRL  = (3 << 0) | (1 << 12);

    SCT0->EV[4].STATE = 0xFFFFFFFF; 
    SCT0->EV[4].CTRL  = (4 << 0) | (1 << 12);

    SCT0->EV[5].STATE = 0xFFFFFFFF; 
    SCT0->EV[5].CTRL  = (5 << 0) | (1 << 12);

    SCT0->EV[6].STATE = 0xFFFFFFFF; 
    SCT0->EV[6].CTRL  = (6 << 0) | (1 << 12);


    // Output 1: SET on EV0, CLEAR on EV1, to be done to the other 6 

    //CH0
    SCT0->OUT[0].SET = (1 << 0); // OUT[n] (set OUT high, (1 << EV[n]))
    SCT0->OUT[0].CLR = (1 << 1); // OUT[n] (set OUT LOW, (1 << EN[n]))
    //CH1
    SCT0->OUT[1].SET = (1 << 0); 
    SCT0->OUT[1].CLR = (1 << 2); 

    SCT0->OUT[2].SET = (1 << 0); 
    SCT0->OUT[2].CLR = (1 << 3); 

    SCT0->OUT[3].SET = (1 << 0); 
    SCT0->OUT[3].CLR = (1 << 4); 

    SCT0->OUT[4].SET = (1 << 0); 
    SCT0->OUT[4].CLR = (1 << 5); 

    SCT0->OUT[5].SET = (1 << 0); 
    SCT0->OUT[5].CLR = (1 << 6); 


    // to be configured for input and output. 
    // Route SCT_OUT0 to pin
    SWM0->PINASSIGN.PINASSIGN7 = (SWM0->PINASSIGN.PINASSIGN7 & 0x00FFFFFF) 
                                | (pwm_var->pin1 << 24); // SCT0
    SWM0->PINASSIGN.PINASSIGN8 = (SWM0->PINASSIGN.PINASSIGN8 & 0x00000000) 
                                | (pwm_var->pin2 << 0)   // SCT1 
                                | (pwm_var->pin3 << 8)   // SCT2
                                | (pwm_var->pin4 << 16)  // SCT3
                                | (pwm_var->pin5 << 24); // SCT4 
    SWM0->PINASSIGN.PINASSIGN9 = (SWM0->PINASSIGN.PINASSIGN9 & 0xFFFFFF00) 
                                | (pwm_var->pin6 << 0); // SCT5

    // Disable digital filter on pin (optional but recommended)
    // IOCON->PIO0_10 &= ~(0x3 << 3);  // Clear MODE bits if needed
    // Start counter
    SCT0->CTRL = 0;  // Clear HALT and CLRCTR
}

void set_pwm_freq(pwm_var *pwm_var, uint32_t freq) {
    pwm_var->period = CLK_freq/(freq);
    SCT0->MATCHREL[0] = pwm_var->period - 1;
}

void set_pwm_duty(pwm_var *pwm_var, uint8_t pwm_duty[6]){
    SCT0->MATCHREL[1] = (pwm_var->period * pwm_duty[0]/100);
    SCT0->MATCHREL[2] = (pwm_var->period * pwm_duty[0]/100);
    SCT0->MATCHREL[3] = (pwm_var->period * pwm_duty[0]/100);
    SCT0->MATCHREL[4] = (pwm_var->period * pwm_duty[0]/100);
    SCT0->MATCHREL[5] = (pwm_var->period * pwm_duty[0]/100);
    SCT0->MATCHREL[6] = (pwm_var->period * pwm_duty[0]/100);
}

void disable_pwm(void) { 
    SCT0->CTRL |= (1 << 2);      // HALT
    SCT0->OUTPUT = 0x00;          // Force outputs low immediately
}

void enable_pwm(void) {
    SCT0->CTRL &= ~((1 << 2) | (1 << 3));
}

int main(void) {
    pwm_var PWM;
    PWM.pin1 = 1;
    PWM.pin2 = 2;
    PWM.pin3 = 3;
    PWM.pin4 = 4;
    PWM.pin5 = 5;
    PWM.pin6 = 6;
    PWM.period = CLK_freq / PWM_freq;  // Pre-calculate period

    pwm_init(&PWM);      // Now MATCHREL[0] gets correct value
    // pwm_init should initialize MATCHREL[1-6] to 0 or safe values
    set_pwm_freq(&PWM, PWM_freq);  // Redundant if period already set, but OK
    uint8_t pwm_d[6] = {0};  // Start at 0% duty
    set_pwm_duty(&PWM, pwm_d);
    enable_pwm();  // Now safe to start — outputs start at 0%

    // Ramp up loop
    for(uint8_t i = 0; i < 50; i++) {
        pwm_d[0] = i + 10;  // 10-59%
        pwm_d[1] = i + 20;  // 20-69%
        pwm_d[2] = i + 30;  // 30-79%
        pwm_d[3] = i + 40;  // 40-89%
        pwm_d[4] = i + 45;  // 45-94%
        pwm_d[5] = i + 50;  // 50-99%
        set_pwm_duty(&PWM, pwm_d);
        for(volatile uint32_t j = 100000; j--; );  // Delay
    }
    // PWM continues running at final values in hardwar
    while(1);  // Or return 0;
    return 0;
}

