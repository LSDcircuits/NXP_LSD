// example to point to regs without struct using base reg offset

int main(void) {
    // Volatile pointers to the exact register addresses
    volatile uint32_t *sysconSysAhbClkCtrl0 = (volatile uint32_t *)0x40048080;
    volatile uint32_t *gpioDir0             = (volatile uint32_t *)0xA0002000;
    volatile uint32_t *gpioNot0             = (volatile uint32_t *)0xA0002300;

    // Enable clocks: bit 6 = GPIO0, bit 18 = IOCON
    *sysconSysAhbClkCtrl0 |= (1 << 6) | (1 << 18);

    // Set PIO0_29 as output
    *gpioDir0 |= (1 << 29);

    while (1) {
        // Toggle PIO0_29
        *gpioNot0 = (1 << 29);

        for (volatile int i = 0; i < 100000; i++);
    }
}
