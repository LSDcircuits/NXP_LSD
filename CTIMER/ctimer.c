void timer_init(void) {
    // Enable clock to CTIMER
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 24);
    // Reset CTIMER
    SYSCON->PRESETCTRL0 &= ~(1 << 24);
    SYSCON->PRESETCTRL0 |=  (1 << 24);
    // Put timer in timer mode 
    CTIMER0->CTCR = 0x0;
    // Prescaler 
    CTIMER0->PR = 0;
    // Reset counter
    CTIMER0->TCR = (1 << 1); // Reset
    CTIMER0->TCR = 0;        // Release reset, keep disabled
}

void timer_start(void){
    // Start timer
    CTIMER0->TCR |= (1 << 0);
}

void timer_stop(void){
    // Stop timer
    CTIMER0->TCR &= ~(1 << 0);
}
