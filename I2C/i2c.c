#include "LPC845.h"
// just a start, still to complete pwm input and documentation 
void I2C_Init(void)
{
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 5);  // I2C0 clock

    // Assign pins via SWM
    SWM->PINASSIGN0 = (10 << 0) | (11 << 8);

    // Enable open drain
    IOCON->PIO0_10 |= (1 << 10);
    IOCON->PIO0_11 |= (1 << 10);

    // Clock config
    SYSCON->I2C0CLKSEL = 0;
    I2C0->CLKDIV = 30;

    // Enable master
    I2C0->CFG |= 1;
}
