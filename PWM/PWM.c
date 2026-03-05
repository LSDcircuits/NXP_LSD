#include "LPC845.h"

// counters:
// 16 bit or 32 bit

// Match registers:
// Match conditions:

// Counter:
// counter value can be used in conjection with match registers
// to create time proportioned PWM signal.

// For output:
// SCT_OUT0_O () -> value is the Pin assigned table 188

// SCTIMER0->CONFIG &= ~(1 << 0) | UNIFY = 0 | TWO 16 bit timers
// SCTIMER0->CONFIG |=  (1 << 0) | UNIFY = 1 | SINGLE 32 bit timer



// 1. Enable    | SYSAHBCLKCTRL	    | bit 8
// 2. Clear     | PRESETCTRL	    | bit 8 
// 3. Interrupt | NVIC		        | 
// 4. input     | INPUT MUX	        | 
// 5. output    | SWIM   		    |
// 6. Request   | DMA_ITRIG_PINMUX  |

// counter // match // Event // output // SCT_OUT 
// SCTIMER CONFIG:

// SCTIMER0->CONFIG &= ~(1 << 0) | UNIFY = 0 | TWO 16 bit timers  
// SCTIMER0->CONFIG |=  (1 << 0) | UNIFY = 1 | SINGLE 32 bit timer

// If UNIFY = 1 in the CONFIG register, only the _L bits of this register are used. In this case,
// REGMODE_H is not used.
// find match registers

// | Field        | Purpose                                        |
// | ------------ | ---------------------------------------------- |
// | **MATCHSEL** | Select which MATCH register triggers the event |
// | **COMBMODE** | How match/input conditions combine             |
// | **STATELD**  | Load a new state (not needed for simple PWM)   |
// | **STATEV**   | State value to load                            |
// | **IOSEL**    | Input pin selection                            |
// | **IOCOND**   | Input condition                                |
// | **OUTSEL**   | Output selection                               |
// | **IOEN**     | Enable input trigger                           |
// | **MATCHMEM** | Enable match trigger                           |

SCTIMER CTRL 

SCTIMER0->CTRL |= (1 << 4);

void pwm_init(void){
    SYSCON->SYSAHBCLKCRTL0 |= (1 << 8); // clock enable SCT
    SYSCON->PRESETCTRL0 &= ~(1 << 8); // Reset values 
    SCTIMER0->CTRL &= ~(1 << 4); 
    SCTIMER0->CONFIG |=  (1 << 0);
}

void pwm_out(){
    SCTIMER0->MATCHL[0] = period;
    SCTIMER0->MATCHL[1] = duty;
}





