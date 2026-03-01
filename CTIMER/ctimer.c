
void timer_init(void)
{
    // 1. Enable clock to CTIMER
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 24);
    // 2. Reset CTIMER
    SYSCON->PRESETCTRL0 &= ~(1 << 24);
    SYSCON->PRESETCTRL0 |=  (1 << 24);

    // 3. Put timer in timer mode 
    CTIMER0->CTCR = 0x0;
    // 4. Prescaler 
    CTIMER0->PR = 0;
    // 5. Reset counter
    CTIMER0->TCR = (1 << 1); // Reset
    CTIMER0->TCR = 0;        // Release reset, keep disabled
}

void timer_start(void){
    // 6. Start timer
    CTIMER0->TCR |= (1 << 0);
}

void timer_stop(void){
    // 6. Start timer
    CTIMER0->TCR &= ~(1 << 0);
}
