// to bne implemented into the pwm file 
#include "LPC845.h"

// PWM Capture Results Structure
typedef struct {
    uint32_t period;      // Total period in timer ticks
    uint32_t high_time;   // High time (duty on) in timer ticks
    uint32_t low_time;    // Low time (duty off) in timer ticks
    float duty_cycle;     // Duty cycle as percentage (0-100)
    uint32_t cap0_value;  // Raw CAP0 capture value (rising edge)
    uint32_t cap1_value;  // Raw CAP1 capture value (falling edge)
} PWM_Capture_Data_t;

volatile PWM_Capture_Data_t pwm_capture = {0};
volatile uint8_t capture_ready = 0;
static uint32_t prev_cap0 = 0;

void SCT0_CLOCK_ENABLE(){
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 8); // Enable SCT0 clock (Table 50 – SYSAHBCLKCTRL0)
    SYSCON->PRESETCTRL0 &= ~(1 << 8);   // Assert SCT0 reset
    SYSCON->PRESETCTRL0 |= (1 << 8);    // De-assert SCT0 reset
}

void SCT0_CONFIG(){
    SCT0->CONFIG = (1 << 0) | (1 << 17); // UNIFIED=1, AUTOLIMIT=1 (Table 175 – SCT CONFIG register)
    SCT0->CTRL = (0 << 5);              // PRE=0, no prescaler (Table 176 – SCT CTRL register)
}

void SCT0_INPUT_CONFIG(){
    SCT0->INMUX[0] = 0;                 // Select SCT_PIN0 for SCT_IN0 (Table 217 – SCT INMUX register[0])
    SWM->PINASSIGN6 &= ~(0xFF << 0);    // Clear SCT0_IN0 field (Table 186 – SCT0_IN0 assignment)
    SWM->PINASSIGN6 |= (0 << 0);        // Assign SCT0_IN0 to PIO0_0
    GPIO->DIRCLR[0] = (1 << 0);         // PIO0_0 as input (Table 349 – GPIO DIR register)
}

void SCT0_EVENT0_CONFIG(){
    // Rising edge capture in State 0, transition to State 1
    SCT0->EV[0].CTRL = (0 << 0)         // MATCHSEL=0 (CAP0) (Table 177 – SCT EVCTRL register)
                     | (0 << 4)         // HEVENT=0 (L counter)
                     | (0 << 5)         // IOSEL=0 (SCT_PIN0)
                     | (1 << 9)         // IOCOND=1 (RISING EDGE)
                     | (2 << 10)        // COMBMODE=2 (IO condition only)
                     | (1 << 14)        // STATELD=1 (load new state)
                     | (1 << 15);       // STATEV=1 (go to state 1)
    
    SCT0->EV[0].STATE = (1 << 0)        // Enabled in State 0 (Table 178 – SCT EVSTATE register)
                      | (1 << 12)       // STATEV=1 (transition to state 1)
                      | (1 << 14);      // STATELD=1
}

void SCT0_EVENT1_CONFIG(){
    // Falling edge capture in State 1, return to State 0
    SCT0->EV[1].CTRL = (1 << 0)         // MATCHSEL=1 (CAP1) (Table 177 – SCT EVCTRL register)
                     | (0 << 4)         // HEVENT=0
                     | (0 << 5)         // IOSEL=0 (SCT_PIN0)
                     | (2 << 9)         // IOCOND=2 (FALLING EDGE)
                     | (2 << 10)        // COMBMODE=2 (IO condition only)
                     | (1 << 14)        // STATELD=1
                     | (0 << 15);       // STATEV=0 (return to state 0)
    
    SCT0->EV[1].STATE = (1 << 1)        // Enabled in State 1 (Table 178 – SCT EVSTATE register)
                      | (0 << 12)       // STATEV=0 (transition to state 0)
                      | (1 << 14);      // STATELD=1
}

void SCT0_CAPTURE_ENABLE(){
    SCT0->CAPCTRL = (1 << 0) | (1 << 1); // Event 0->CAP0, Event 1->CAP1 (Table 208 – SCT CAPCTRL register)
    SCT0->STATE = 0;                    // Set initial state to 0 (Table 207 – SCT STATE register)
}

void SCT0_INTERRUPT_ENABLE(){
    SCT0->EVFLAG = 0xFF;                // Clear all event flags (Table 181 – SCT EVFLAG register)
    SCT0->EVEN = (1 << 1);              // Enable interrupt for Event 1 (Table 180 – SCT EVEN register)
    NVIC_EnableIRQ(SCT_IRQn);           // Enable SCT interrupt in NVIC
}

void SCT0_START(){
    SCT0->CTRL &= ~(1 << 2);            // Clear HALT bit (Table 176 – SCT CTRL register)
    SCT0->CTRL &= ~(1 << 1);            // Clear STOP bit
}

void SCT0_PWM_Capture_Init(){
    SCT0_CLOCK_ENABLE();
    SCT0_CONFIG();
    SCT0_INPUT_CONFIG();
    SCT0_EVENT0_CONFIG();
    SCT0_EVENT1_CONFIG();
    SCT0_CAPTURE_ENABLE();
    SCT0_INTERRUPT_ENABLE();
    SCT0_START();
}

void SCT_IRQHandler(){
    if(SCT0->EVFLAG & (1 << 1)){        // Check Event 1 flag (Table 181 – SCT EVFLAG register)
        pwm_capture.cap0_value = SCT0->CAP[0].U;  // Read CAP0 (Table 209 – SCT CAP register[0])
        pwm_capture.cap1_value = SCT0->CAP[1].U;  // Read CAP1 (Table 209 – SCT CAP register[1])
        
        if(pwm_capture.cap1_value > pwm_capture.cap0_value){
            pwm_capture.high_time = pwm_capture.cap1_value - pwm_capture.cap0_value;
        } else {
            pwm_capture.high_time = (0xFFFFFFFF - pwm_capture.cap0_value) + pwm_capture.cap1_value + 1;
        }
        
        if(prev_cap0 != 0){
            if(pwm_capture.cap0_value > prev_cap0){
                pwm_capture.period = pwm_capture.cap0_value - prev_cap0;
            } else {
                pwm_capture.period = (0xFFFFFFFF - prev_cap0) + pwm_capture.cap0_value + 1;
            }
            pwm_capture.low_time = pwm_capture.period - pwm_capture.high_time;
            if(pwm_capture.period > 0){
                pwm_capture.duty_cycle = ((float)pwm_capture.high_time / (float)pwm_capture.period) * 100.0f;
            }
            capture_ready = 1;
        }
        prev_cap0 = pwm_capture.cap0_value;
        
        SCT0->EVFLAG = (1 << 1);        // Clear Event 1 flag
    }
}

uint8_t SCT0_PWM_Capture_Ready(){
    return capture_ready;
}

PWM_Capture_Data_t SCT0_PWM_Capture_Get(){
    PWM_Capture_Data_t result;
    NVIC_DisableIRQ(SCT_IRQn);
    result = pwm_capture;
    NVIC_EnableIRQ(SCT_IRQn);
    capture_ready = 0;
    return result;
}

int main(void){
    SCT0_PWM_Capture_Init();
    
    while(1){
        if(SCT0_PWM_Capture_Ready()){
            PWM_Capture_Data_t pwm = SCT0_PWM_Capture_Get();
            // pwm.period, pwm.high_time, pwm.low_time, pwm.duty_cycle now available
        }
    }
}
// morre table setting (still deciding on how to implement this)

// Event 0: Capture on rising edge, then toggle between states
SCT0->EV[0].CTRL = (0 << 0)       // CAP0
                 | (1 << 4)       // SCT_PIN0
                 | (1 << 5)       // RISING EDGE
                 | (1 << 6);      // COMBINE

// State machine: alternate between capturing and waiting
SCT0->EV[0].STATE = (1 << 0);     // Enabled in state 0
SCT0->EV[0].STATE |= (1 << 12);   // Set state 1 after event (next capture waits)

SCT0->EV[1].CTRL = (0 << 0)       // Also CAP0 (or CAP1)
                 | (1 << 4)       
                 | (1 << 5)       // RISING EDGE
                 | (1 << 6);
SCT0->EV[1].STATE = (1 << 1);     // Enabled in state 1
SCT0->EV[1].STATE |= (2 << 12);   // Return to state 0

// This ping-pongs between states, capturing consecutive rising edges
// in CAP0 and CAP1 (or overwriting and letting software track)
