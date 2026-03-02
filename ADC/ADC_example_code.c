void ADC_Init(void){
    
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 23);
    
    SYSCON->ADCCLKSEL = 0;   
    
    SYSCON->ADCCLKDIV = 1;
    
    IOCON->PIO[7] &= ~(1 << 7); 
    
    SYSCON->PDRUNCFG &= ~(1 << 4);
    
    ADC0->CTRL = (0 << 30); 
}

uint16_t ADC_Read_CH0(void) {
    
    ADC0->SEQ_CTRL[0] = (1 << 0);
    
    ADC0->SEQ_CTRL[0] |= (1 << 31);
    
    while (!(ADC0->SEQ_GDAT[0] & (1UL << 31)));
    
    return (ADC0->SEQ_GDAT[0] >> 4) & 0xFFF;
    
}
