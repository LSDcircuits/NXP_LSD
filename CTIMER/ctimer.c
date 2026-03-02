void Ctimer_init(void) {
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 25);
    SYSCON->PRESETCTRL0 &= ~(1 << 25);
    SYSCON->PRESETCTRL0 |=  (1 << 25);
    CTIMER0->CTCR = 0x0;
    CTIMER0->PR = 0;
    CTIMER0->TCR = (1 << 1); 
    CTIMER0->TCR = 0;        
}

void Ctimer_start(void){
    CTIMER0->TCR |= (1 << 0);
}

void Ctimer_stop(void){
    CTIMER0->TCR &= ~(1 << 0);
}

uint32_t read_timer(){
    return CTIMER0->TC;
}

void clear_timer(){
    CTIMER0->TCR &= ~(1 << 0);
    CTIMER0->TCR |= (1 << 1); 
}
