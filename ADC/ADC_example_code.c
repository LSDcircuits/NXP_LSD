#include "LPC845.h"

void ADC_Init(void)
{
    // Enable ADC clock (bit 24 in SYSAHBCLKCTRL0)
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 24);

    // Select FRO as ADC clock source (already 30 MHz in your setup)
    SYSCON->ADCCLKSEL = 0;   // FRO

    // Set ADC clock divider (divide by 1)
    SYSCON->ADCCLKDIV = 1;

    // Configure PIO0_7 as analog input (disable digital mode)
    IOCON->PIO0_7 &= ~(1 << 7);   // ADMODE = 0 (analog)

    // Power up ADC
    SYSCON->PDRUNCFG &= ~(1 << 4);

    // Basic ADC config:
    // - Software trigger
    // - 12-bit mode
    // - No burst
    ADC0->CTRL = (0 << 30);  // ensure ADC enabled
}

uint16_t ADC_Read_CH0(void)
{
    // Select channel 0
    ADC0->SEQ_CTRL[0] = (1 << 0);

    // Start conversion
    ADC0->SEQ_CTRL[0] |= (1 << 31);

    // Wait for conversion complete
    while (!(ADC0->SEQ_GDAT[0] & (1UL << 31)));

    // Return result (12-bit)
    return (ADC0->SEQ_GDAT[0] >> 4) & 0xFFF;
}

int main(void)
{
    ADC_Init();

    while (1)
    {
        uint16_t value = ADC_Read_CH0();

        // value now contains analog reading (0–4095)
    }
}
