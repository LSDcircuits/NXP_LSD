#include "LPC845.h"

void GPIO_ENABLE(){
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 6) | (1 << 18); // (Table 50 – SYSAHBCLKCTRL0)
}
void GPIO_PIN_CONFIG(uint8_t port, uint8_t pin, uint8_t out){
    if(out){
        GPIO->DIR[port] |= (1 << pin); // (Table 349 – GPIO DIR register)
    } else {
        GPIO->DIR[port] &= ~(1 << pin); // (Table 349 – GPIO DIR register)
    }
}

void GPIO_OUT(uint8_t port, uint8_t pin, uint8_t state){
    if(state){
        GPIO->SET[port] = (1 << pin); // (Table 351 – GPIO SET register)
    }  else{
        GPIO->CLR[port] = (1 << pin); // (Table 352 – GPIO CLR register)
    }
}

int main(void) {
    GPIO_ENABLE();
    GPIO_PIN_CONFIG(0, 8, 1);
    while (1){
        GPIO->NOT[0] = (1 << 8); // (Table 353 – GPIO NOT register)
        for (volatile int i = 0; i < 100000; i++);
    }
}
