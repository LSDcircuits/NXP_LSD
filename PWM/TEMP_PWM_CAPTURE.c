// to bne implemented into the pwm file 


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
