void Ctimer_init(void) {
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 25); // (Table 95 – SYSAHBCLKCTRL0 register)
    SYSCON->PRESETCTRL0 &= ~(1 << 25);   // (Table 89 – PRESETCTRL0 register)
    SYSCON->PRESETCTRL0 |=  (1 << 25);   
    CTIMER0->CTCR = 0x0;                 // (Table 701 – CTCR register)
    CTIMER0->PR = 0;                     // (Table 699 – PR register)
    CTIMER0->TCR = (1 << 1);             // Table 697 – TCR register
    CTIMER0->TCR = 0;        
}

void Ctimer_start(void){
    CTIMER0->TCR |= (1 << 0);           // (Table 697 – TCR register)
}

void Ctimer_stop(void){
    CTIMER0->TCR &= ~(1 << 0);        
}

uint32_t read_timer(){
    return CTIMER0->TC;                // (Table 698 – TC register)
}

void clear_timer(){
    CTIMER0->TCR &= ~(1 << 0);
    CTIMER0->TCR |= (1 << 1); 
}

// for tomorrow to check if the timer is working 
int main(){
    Ctimer_init();
    Ctimer_start();
    while(1){
        uint32t val0 = read_timer();
        // make something happen like count up
        uint32t val1 = read_timer();
        Ctimer_stop();
        clear_timer(); 
        }
}
