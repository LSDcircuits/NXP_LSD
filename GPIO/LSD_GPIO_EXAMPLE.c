#include "LPC845.h"

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

int main(void) {
    GPIO_ENABLE();
    GPIO_PIN_CONFIG(0, 8, 1);
    while (1){
        GPIO->NOT[0] = (1 << 8);
        for (volatile int i = 0; i < 100000; i++);
    }
}
