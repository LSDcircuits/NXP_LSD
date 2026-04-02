// file contain code to set up the clock

void clock_init_fro_30mhz(void) {
    // 1. Set FRO to 30MHz
    SYSCON->FROCTRL = (SYSCON->FROCTRL & ~0x3) | 0x2;
    
    // 2. Wait for stable (FRO stable bit)
    while (!(SYSCON->FROCTRL & (1 << 26)));
    
    // 3. Ensure main clock is FRO (it is by default, but be explicit)
    SYSCON->MAINCLKSEL = 0;
    
    // 4. Set divider to 1 (full 30MHz)
    SYSCON->SYSAHBCLKDIV = 1;
    
    // 5. Update flash wait states for 30MHz (1 WS required)
    SYSCON->FLASHCFG = (SYSCON->FLASHCFG & ~0x3) | 1;
    
    // Update CMSIS variable
    SystemCoreClock = 30000000;
} can you exlain this code and reference every table used in the manual for this code.



