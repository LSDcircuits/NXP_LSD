#include "LPC845.h"
#define CLK_freq 12000000
#define PWM_freq 50

void disable_pwm(void) { 
    SCT0->CTRL |= (1 << 2);      // HALT
    SCT0->OUTPUT = 0x00;          // Force outputs low immediately
}

void enable_pwm(void) {
    SCT0->CTRL &= ~((1 << 2) | (1 << 3));
}

void pwm_init() {

    SYSCON->SYSAHBCLKCTRL0 |= (1 << 8)   
                           |  (1 << 7);  
    SYSCON->PRESETCTRL0 &= ~(1 << 8);
    SYSCON->PRESETCTRL0 |=  (1 << 8);

    SCT0->CONFIG = (1 << 0)      
                 | (1 << 17);    

    SCT0->MATCHREL[0] = 0; 
    SCT0->MATCHREL[1] = 0;  
    SCT0->MATCHREL[2] = 0;
    SCT0->MATCHREL[3] = 0;
    SCT0->MATCHREL[4] = 0;
    SCT0->MATCHREL[5] = 0;
    SCT0->MATCHREL[6] = 0;

    SCT0->CTRL = (1 << 2) | (1 << 3);  

    SCT0->EV[0].STATE = 0xFFFFFFFF;
    SCT0->EV[0].CTRL  = (0 << 0) | (1 << 12); 
    
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

    SCT0->OUT[0].SET = (1 << 0); 
    SCT0->OUT[0].CLR = (1 << 1); 

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

    SCT0->CTRL = 0; 
}

void clear_pwm(void) {
    SWM0->PINASSIGN.PINASSIGN7 &= ~(0xFFU << 24); // SCT0_OUT0
    SWM0->PINASSIGN.PINASSIGN8 &= ~(0xFFU << 0);  // SCT0_OUT1
    SWM0->PINASSIGN.PINASSIGN8 &= ~(0xFFU << 8);  // SCT0_OUT2
    SWM0->PINASSIGN.PINASSIGN8 &= ~(0xFFU << 16); // SCT0_OUT3
    SWM0->PINASSIGN.PINASSIGN8 &= ~(0xFFU << 24); // SCT0_OUT4
    SWM0->PINASSIGN.PINASSIGN9 &= ~(0xFFU << 0);  // SCT0_OUT5
}

void set_pwm_ch0(uint32_t pin) {
    SWM0->PINASSIGN.PINASSIGN7 = (SWM0->PINASSIGN.PINASSIGN7 & ~(0xFFU << 24)) | (pin << 24);
}
void set_pwm_ch1(uint32_t pin){
    SWM0->PINASSIGN.PINASSIGN8 = (SWM0->PINASSIGN.PINASSIGN8  & ~(0xFFU << 0))  | (pin << 0); 
}
void set_pwm_ch2(uint32_t pin){
    SWM0->PINASSIGN.PINASSIGN8 = (SWM0->PINASSIGN.PINASSIGN8  & ~(0xFFU << 8))  | (pin << 8); 
}
void set_pwm_ch3(uint32_t pin){
    SWM0->PINASSIGN.PINASSIGN8 = (SWM0->PINASSIGN.PINASSIGN8  & ~(0xFFU << 16))  | (pin << 16); 
}
void set_pwm_ch4(uint32_t pin){
    SWM0->PINASSIGN.PINASSIGN8 = (SWM0->PINASSIGN.PINASSIGN8  & ~(0xFFU << 24))  | (pin << 24);
}
void set_pwm_ch5(uint32_t pin){
    SWM0->PINASSIGN.PINASSIGN9 = (SWM0->PINASSIGN.PINASSIGN9  & ~(0xFFU << 0))  | (pin << 0);
}

//Table 125 - for the register overview to fidn tables
//Registers to check to get freq for SCT TIMER & NO EXTERNAL CLOCK
// MAINCLKSEL[1:0]
// MAINCLKPLLSEL[1:0]
// SCTCLKSEL[1:0]
// SCTCLKDIV[0-255]

get_clk_freq(uint8_t freq){
    uint32_t ext_sel = (SYSCON->EXTCLKSEL & 0x01); 
    uint32_t main_sel = (SYSCON->MAINCLKSEL & 0x03);
    uint32_t main_pll = (SYSCON->MAINCLKPPLSEL & 0x00);
    uint32_t sct_sel = (SYSCON->SCTCLKSEL & 0x01);
    uint32_t div = (SYSCON->SCTCLKDIV);

}

void set_pwm_freq(uint32_t freq) {
    uint32_t period = CLK_freq/(freq);
    SCT0->MATCHREL[0] = period - 1;
    // , optional IOCON->PIO0[pin] = 0x80;   // DIGIMODE = 1, FUNC = 0 (GPIO/SWM digital), no pull-up
}

void set_pwm_duty(uint32_t pwm_duty[6]){
    SCT0->MATCHREL[1] = (SCT0->MATCHREL[0] * pwm_duty[0]/100);
    SCT0->MATCHREL[2] = (SCT0->MATCHREL[0] * pwm_duty[1]/100);
    SCT0->MATCHREL[3] = (SCT0->MATCHREL[0] * pwm_duty[2]/100);
    SCT0->MATCHREL[4] = (SCT0->MATCHREL[0] * pwm_duty[3]/100);
    SCT0->MATCHREL[5] = (SCT0->MATCHREL[0] * pwm_duty[4]/100);
    SCT0->MATCHREL[6] = (SCT0->MATCHREL[0] * pwm_duty[5]/100);
}

void pwm_pin(uint8_t chanel, uint8_t pin){
    if      (chanel == 0) set_pwm_ch0(pin);
    else if (chanel == 1) set_pwm_ch1(pin);
    else if (chanel == 2) set_pwm_ch2(pin);
    else if (chanel == 3) set_pwm_ch3(pin);
    else if (chanel == 4) set_pwm_ch4(pin);
    else if (chanel == 5) set_pwm_ch5(pin);
}

int main(void) {
    pwm_init();
    disable_pwm();
    pwm_pin(0, 3);
    pwm_pin(3, 10);
    pwm_pin(2, 3);
    set_pwm_freq(PWM_freq);  

    // find a way to initialize
    uint32_t pwm_d[6];
    pwm_d[0] = 50;
    pwm_d[2] = 50;
    pwm_d[3] = 50;
    set_pwm_duty(pwm_d);
    enable_pwm();
    while(1){
    }
}




// void pwm_pin(uint8_t ch, uint8_t pin) {
//     if (ch > 5 || pin > 31) return;

//     // Register pointers live in flash (const)
//     static volatile uint32_t * const regs[] = {
//         &SWM0->PINASSIGN.PINASSIGN7,   // ch0
//         &SWM0->PINASSIGN.PINASSIGN8,   // ch1
//         &SWM0->PINASSIGN.PINASSIGN8,   // ch2
//         &SWM0->PINASSIGN.PINASSIGN8,   // ch3
//         &SWM0->PINASSIGN.PINASSIGN8,   // ch4
//         &SWM0->PINASSIGN.PINASSIGN9    // ch5
//     };
//     static const uint8_t shifts[] = {24, 0, 8, 16, 24, 0};

//     uint32_t shift = shifts[ch];
//     uint32_t mask  = ~(0xFFU << shift);

//     *regs[ch] = (*regs[ch] & mask) | ((uint32_t)pin << shift);
// }
