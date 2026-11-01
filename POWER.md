# Power Management Subsystem & Battery Budget

## Current Consumption Profile
| Operating State | Active Peripherals | Current (mA) | Expected Runtime |
|---|---|---|---|
| **Active Inking** | CPU @ 240 MHz, FSPI @ 24 MHz, HSPI @ 8 MHz, IT8951 Active | ~115 - 130 mA | **~24 hours continuous** |
| **Idle / Reading** | CPU @ 80 MHz, IT8951 Standby, Display static | ~38 - 45 mA | **~75 hours** |
| **Wi-Fi Sync** | CPU @ 240 MHz, Wi-Fi TX/RX active, SD transfer | ~180 - 220 mA | ~15 hours |
| **Deep Sleep** | All rails gated (AO3401 off), ESP32 ext1 wake armed | **< 25 uA** | **> 30 days standby** |

## Battery Monitoring Circuit
A 100k / 100k precision resistor divider scales the 4.2V LiPo battery voltage down to <= 2.1V for safe sampling on ESP32-S3 ADC1 Channel 8 (GPIO 19).
