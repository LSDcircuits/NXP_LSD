#include "LPC845.h"

int main(void) {
    // Quick LED blink to verify CPU is running
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 6);  // GPIO clock
    GPIO->DIR[0] |= (1 << 13);           // LED on PIO0_13 (common on LPC845 boards)
    
    for (int i = 0; i < 5; i++) {
        GPIO->B[0][13] = 1;  // LED on
        for (volatile int j = 0; j < 1000000; j++);
        GPIO->B[0][13] = 0;  // LED off
        for (volatile int j = 0; j < 1000000; j++);
    }
    
    pwm_init();  // Then start PWM
    
    while (1);
}
