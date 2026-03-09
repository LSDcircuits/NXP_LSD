void ADC_Init(void){

    SYSCON->SYSAHBCLKCTRL0 |= (1 << 23);   // (Table 95 – SYSAHBCLKCTRL0 register)

    SYSCON->ADCCLKSEL = 0;                 // (Table 110 – ADCCLKSEL register)

    SYSCON->ADCCLKDIV = 1;                 // (Table 111 – ADCCLKDIV register)

    IOCON->PIO[7] &= ~(1 << 7);            // (Table 340 – IOCON PIO register)

    SYSCON->PDRUNCFG &= ~(1 << 4);         // (Table 84 – PDRUNCFG register)

    ADC0->CTRL = (0 << 30);                // (Table 739 – ADC CTRL register)
}

uint16_t ADC_Read_CH0(void) {

    ADC0->SEQ_CTRL[0] = (1 << 0);          // (Table 741 – SEQ_CTRL register)

    ADC0->SEQ_CTRL[0] |= (1 << 31);        // (Table 741 – SEQ_CTRL register)

    while (!(ADC0->SEQ_GDAT[0] & (1UL << 31)));  // (Table 743 – SEQ_GDAT register)

    return (ADC0->SEQ_GDAT[0] >> 4) & 0xFFF;     // (Table 743 – SEQ_GDAT register)
}
