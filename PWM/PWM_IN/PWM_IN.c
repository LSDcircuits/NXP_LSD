#include "LPC845.h"

pwm_in_init(){
        // Enable clocks for SCT, SWM, AND GPIO
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 8)   // SCT clock
                           |  (1 << 7);  // SWIM
    SYSCON->SYSAHBCLKCTRL1 |= (1 << 7);  // INPUTMUX
    // Reset SCT
    SYSCON->PRESETCTRL0 &= ~(1 << 8);
    SYSCON->PRESETCTRL0 |=  (1 << 8);

    SCT0->CTRL = (1 << 2) | (1 << 3);  // HALT | CLRCTR (write both at once)
    SCT0->REGMODE |=(1 << 0); // MAKE CAP REG 

    SCT0->EV[0].STATE = 0xFFFFFFFF;
    SCT0->EV[0].CTRL= (0 << 0)  // MATCH SEL SET 0 
                    | (0 << 4)  // LOW UNIFIED
                    | (0 << 5)  // OUSEL set 0 INPUT
                    | (0 << 6)  // IOSEL = 0 SCT0 (0-3= SCT[0-3] ,.. bit[6-9])
                    | (1 << 10) // [IOCOND bit[10-12] 0x0 = low, 0x1 = rise, 0x2  = fall, 0x3  = high]
                    | (2 << 12) // COMBMODE = 2 (I/O only)
                    | (1<< 14)  // STATELD  = 1 (load STATEV)
                    | (0 <<15); // STATEV   = 0 (stay in state 0) <- check

    SCT0->CAPCTRL[0] = (1 << 0);

    // ena ev[0] interupt 
    ST0->EVEN = (1 << 0);
    SCT0->EVFLAG = 0xFFFFFFFF // CLR stale flags

    SCT->CTRL &= ~(1 << 2); // clr hlt start    

}


// set pin to channel
void set_pin(uint32_t pin) {
    // set input pin
    SWM0->PINASSIGN.PINASSIGN6 = (SWM0->PINASSIGN.PINASSIGN6 & ~(0xFFU << 24)) | (pin << 24);
}


// outxmp
void set_pwm_ch1(uint32_t pin){
    SWM0->PINASSIGN.PINASSIGN8 = (SWM0->PINASSIGN.PINASSIGN8  & ~(0xFFU << 0))  | (pin << 0); 
}

pwm_in(){
    SCT0->EV[0].CTRL = 
}
