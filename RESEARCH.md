# Hardware Platform Research & Trade Study (5.83" Pocket E-Paper Writer)

## 1. Project Scope & Architecture Pivot
- **Target Budget:** ~$65.00 USD total (grant-friendly, high feasibility).
- **Form Factor:** Compact handheld / pocket digital writer (5.83" diagonal screen, ~125 x 99 mm envelope).
- **Core Goal:** Low-latency text drafting, note-taking, and micro-sketching with battery autonomy measured in weeks.

---

## 2. Display Subsystem Trade Study
| Display Option | Active Area & Res | Controller & Bus | Unit Cost | Refresh Specs | Assessment & Verdict |
|---|---|---|---|---|---|
| **Waveshare 5.83" E-Paper HAT (V2, B/W)** | **119.2 x 88.3 mm (648 x 480, 138 DPI)** | **UC8179 (4-Wire SPI)** | **~$34.00** | **~0.3s partial refresh, ~3.5s global** | **SELECTED:** Fits the $65 budget perfectly. UC8179 supports fast windowed partial refresh for responsive typing & stroke rendering. Level-shifted breakout HAT handles 3.3V power and booster circuits. |
| Waveshare 10.3" Carta E-Paper | 211.5 x 158.6 mm (1872 x 1404) | IT8951 (SPI/Parallel) | ~$125.00 | ~120ms A2 mode | **Rejected:** Over budget ($125 alone exceeds the $65 total ceiling). |
| 4.2" E-Paper Module | 84.8 x 63.6 mm (400 x 300) | UC8176 / SSD1683 | ~$24.00 | ~0.3s partial | **Rejected:** 5.83" offers ~80% more screen real estate (648x480 vs 400x300) while still meeting the $65 budget limit. |
| 5.83" 3-Color (B/W/Red) E-Paper | 119.2 x 88.3 mm (648 x 480) | UC8179 | ~$36.00 | ~15–20s full refresh | **Rejected:** Tri-color electrophoretic displays cannot perform fast partial refreshes, causing intolerable typing and drawing lag. |

---

## 3. Input & Interaction Architecture: Dual Mode (Stylus & Keyboard)
On a $65 budget, active Wacom EMR digitizer stacks ($45+) exceed total funding. We evaluated input strategies for a 5.83" writer:

| Input Mechanism | Hardware Components | Added Cost | Usability & Power | Assessment & Verdict |
|---|---|---|---|---|
| **Resistive Touch Overlay + Passive Stylus (Drawing/Handwriting)** | 5.8"–6.0" 4-wire PET film overlay + XPT2046 SPI controller | ~$7.50 | Works with fingernail, plastic stylus, or fine point. Ultra-low power SPI polling. | **SELECTED FOR HYBRID WRITER:** Provides instant sketching, handwriting, and menu navigation directly on the screen without needing a costly active digitizer. |
| **I2C Compact Keyboard (Typing / WriterDeck)** | M5Stack CardKB / BB Q20 I2C module or Mini Bluetooth keyboard | ~$8.50 | 50-key QWERTY keyboard with tactile dome switches. 2-wire I2C bus interface. | **COMPATIBLE ADD-ON:** Perfect for distraction-free novel writing and journaling. |
| Wacom EMR Digitizer Film | Inductive sensor PCB + EMR Stylus | ~$48.00 | Exceptional handwriting, zero finger touch | **Rejected:** Cost is prohibitive for a sub-$65 project build. |
| Capacitive Touch Screen (Custom bonded) | GoodDisplay custom CTP + FT6336 | ~$32.00 | Finger touch only; poor fine stylus accuracy | **Rejected:** Expensive and requires high minimum order quantities. |

---

## 4. Microcontroller Platform Evaluation
| MCU Platform | Architecture & Clock | RAM & Storage | Wireless | Price | Assessment & Verdict |
|---|---|---|---|---|---|
| **ESP32-S3-DevKitC-1 (N16R8)** | **Dual-Core Xtensa LX7 @ 240 MHz** | **512 KB SRAM + 8 MB Octal PSRAM + 16 MB Flash** | **Wi-Fi 802.11 b/g/n + BLE 5.0** | **~$6.50** | **SELECTED:** Framebuffer for 648x480 1bpp is ~38.8 KB (fits comfortably into internal SRAM or PSRAM). Native USB-OTG/HID, FreeRTOS dual-core multitasking, and low-power deep sleep. |
| Raspberry Pi Pico W | Dual-Core Cortex-M0+ @ 133 MHz | 264 KB SRAM + 2 MB Flash | Wi-Fi 4 + BLE | ~$6.00 | **Strong alternative:** Great for MicroPython, but lacks hardware floating-point and large PSRAM for multi-layer framebuffers. |
| STM32F401 BlackPill | Cortex-M4 @ 84 MHz | 96 KB SRAM + 512 KB Flash | None | ~$4.00 | **Rejected:** No built-in wireless for cloud/browser document synchronization. |

---

## 5. Storage & Power Architecture
1. **Local Document Storage:**
   - Onboard SPI MicroSD breakout (~$1.20) + 16GB/32GB Card (~$3.80) formatted as FAT32.
   - Dual-backup: 16 MB onboard SPI flash stores persistent system settings and cached notes in ESP-IDF NVS/LittleFS.
2. **Power Supply & Battery Management:**
   - 3.7V 1800–2000 mAh 1S LiPo pouch battery (~$5.50).
   - TP4056 USB-C Li-Ion charging module with integrated DW01A reverse/over-discharge protection (~$0.80).
   - Ultra-low quiescent current LDO (ME6211 or RT9193, Iq < 30 µA) providing clean 3.3V rail.
3. **Power Budget Breakdown:**
   - Standby / Sleep: < 35 µA (ESP32-S3 deep sleep + E-paper static display power 0W).
   - Active Writing (partial refresh @ 5 Hz): ~38 mA average (~45–50 hours of continuous active writing on a single charge).
