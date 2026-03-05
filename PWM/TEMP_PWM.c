#include "LPC845.h"

#define PWM_PERIOD 1000
#define PWM_DUTY   500

void pwm_init(void)
{
    /* Enable clock to SCT */
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 8);

    /* Reset SCT */
    SYSCON->PRESETCTRL0 &= ~(1 << 8);
    SYSCON->PRESETCTRL0 |=  (1 << 8);

    /* Unified counter mode */
    SCT0->CONFIG = (1 << 0);

    /* Use system clock */
    SCT0->CTRL &= ~(1 << 2);

    /* MATCH registers */
    SCT0->MATCHREL[0].U = PWM_PERIOD;   // Period
    SCT0->MATCHREL[1].U = PWM_DUTY;     // Duty cycle

    /* Event 0 -> period reset */
    SCT0->EV[0].STATE = 0xFFFFFFFF;
    SCT0->EV[0].CTRL  = (0 << 0) | (1 << 12);  

    /* Event 1 -> duty event */
    SCT0->EV[1].STATE = 0xFFFFFFFF;
    SCT0->EV[1].CTRL  = (1 << 0);

    /* Output 0 SET on event0 */
    SCT0->OUT[0].SET = (1 << 0);

    /* Output 0 CLEAR on event1 */
    SCT0->OUT[0].CLR = (1 << 1);

    /* Start counter */
    SCT0->CTRL &= ~(1 << 2);
}
