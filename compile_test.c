#include "LPC845.h"

//{PWM}
// The MATCH is just a sensor.
// The EVENT is the decision.
// The OUTPUT action is what changes the pin.

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

void DAC_Init(void){
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 28); 
    IOCON->PIO[17] &= ~(1 << 16);   // disable digital mode fCor analog for PIN 17
    SYSCON->PDRUNCFG &= ~(1 << 13);
    SWM0->PINENABLE0 &= ~(1 << 26);
    DAC0->CTRL = 0;
}

void DAC_Write(uint16_t value){
    value &= 0x3FF;
    DAC0->CR = (value << 6);
}

void ADC_Init(void){
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 23);
    SYSCON->ADCCLKSEL = 0;   
    SYSCON->ADCCLKDIV = 1;
    IOCON->PIO[7] &= ~(1 << 6);   
    SYSCON->PDRUNCFG &= ~(1 << 4);
    ADC0->CTRL = (0 << 30); 
}

uint16_t ADC_Read_CH0(void) {
    ADC0->SEQ_CTRL[0] = (1 << 0);
    ADC0->SEQ_CTRL[0] |= (1 << 31);
    while (!(ADC0->SEQ_GDAT[0] & (1UL << 31)));
    return (ADC0->SEQ_GDAT[0] >> 4) & 0xFFF;
}

void GPIO_ENABLE(){
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 6) | (1 << 18);
}

void GPIO_PIN_CONFIG(uint8_t port, uint8_t pin, uint8_t out){
    if(out){
        GPIO->DIR[port] |= (1 << pin);
    } else {
        GPIO->DIR[port] &= ~(1 << pin);
    }
}

void GPIO_OUT(uint8_t port, uint8_t pin, uint8_t state){
    if(state){
        GPIO->SET[port] = (1 << pin);
    }  else{
        GPIO->CLR[port] = (1 << pin);
    }
}

int main(void){
    ADC_Init();
    GPIO_ENABLE();
    GPIO_PIN_CONFIG(0, 8, 1);
    while (1){
        uint16_t value = ADC_Read_CH0();
        DAC_Write(value);
        GPIO->NOT[0] = (1 << 8);
        for (volatile int i = 0; i < (value * 1000); i++);
    }
}
