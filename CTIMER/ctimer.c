
void timer_init(void)
{
    // 1. Enable clock to CTIMER
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 24); 
    // 2. Reset CTIMER
    SYSCON->PRESETCTRL0 &= ~(1 << 24);
    SYSCON->PRESETCTRL0 |=  (1 << 24);

    // 3. Put timer in timer mode 
    CTIMER->CTCR = 0x0;
    // 4. Prescaler 
    CTIMER->PR = 0;
    // 5. Reset counter
    CTIMER->TCR = (1 << 1); // Reset
    CTIMER->TCR = 0;        // Release reset, keep disabled
}

void timer_start(void){
    // 6. Start timer
    CTIMER->TCR |= (1 << 0);
}

void timer_stop(void){
    // 6. Start timer
    CTIMER->TCR &= ~(1 << 0);
}
