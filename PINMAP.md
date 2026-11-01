# Detailed GPIO Pin Allocation & Strapping Pin Protection

## 1. Bus Partitioning & SPI Frequency Allocation
- **FSPI (Fast SPI) @ 24 MHz:** IT8951 e-paper controller (SCK: GPIO 12, MOSI: GPIO 11, CS: GPIO 10, DC: GPIO 9).
- **HSPI @ 8 MHz:** EMR Digitizer sensor controller (SCK: GPIO 4, MISO: GPIO 5, MOSI: GPIO 3, CS: GPIO 2, INT: GPIO 1).
- **Dedicated SPI @ 25 MHz:** MicroSD Card socket (SCK: GPIO 14, MISO: GPIO 13, MOSI: GPIO 15, CS: GPIO 16).

## 2. Strapping Pin Considerations
- **GPIO 0:** Strapping pin for boot mode. Utilized for Power / Wake button with external 10k pull-up resistor to ensure clean boot into flash execution.
- **GPIO 45 / 46:** Reserved for Octal PSRAM / Flash bus lines (do not connect externally).
