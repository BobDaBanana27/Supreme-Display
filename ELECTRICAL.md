# Electrical Design & Power Distribution

## Power Tree
- **Primary Input:** USB-C (5.0V bus power).
- **Battery Charging:** TP4056 CC/CV controller (set to 750 mA charge current via 1.6k R_PROG).
- **Battery Protection:** DW01A + 8205A dual N-MOSFET (over-charge at 4.28V, over-discharge cutoff at 2.4V, over-current at 3.0A).
- **System Rail (3.3V):** Torex / TI Low-Dropout Regulator (LDO) rated for 800 mA continuous output.
- **Peripheral Power Gating:** P-channel MOSFET (AO3401A) controls 3.3V rail to the IT8951 e-paper controller (gated via GPIO 6) to eliminate quiescent draw during sleep.

## Pin Assignment Strategy
Three independent SPI buses are configured to prevent bus contention:
- **FSPI (Fast SPI, 24 MHz):** Dedicated to IT8951 e-paper controller.
- **HSPI (8 MHz):** Dedicated to EMR Digitizer controller.
- **Standard SPI (25 MHz):** Dedicated to MicroSD card socket.
