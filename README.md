# Supreme Display (EinkPad) 

An open-source, pocket-sized 5.83" DIY e-paper display with the tier 2 **$65 budget** (total cost: **$62.40**!).

---

## What It Looks Like & How It's Laid Out

### 1. How the Parts Will Talk to Each Other

At the heart of the build is an **ESP32-S3** board (dual-core 240 MHz, 8 MB extra PSRAM, and 16 MB flash) driving three main subsystems:
- **5.83" Waveshare E-Paper Display:** Crisp 648 × 480 black-and-white screen with fast partial refreshes (~0.3s) so drawing and typing don't feel sluggish.
- **Touch / Input Layer:** A transparent 4-wire resistive overlay that tracks any fine-tip stylus with zero fuss (or swap in a mini I2C keyboard like the CardKB if you'd rather type like an old-school typewriter!).
- **Storage & Power:** An SD card slot for saving your notes, a rechargeable 1800 mAh LiPo battery, and a standard USB-C port for charging.

### 2. Wiring & Schematic

Clean 3.3V power routing, battery protection via TP4056 + DW01A, and dedicated high-speed SPI pins for the display and digitizer.

---

## How It Will Actually Work 

Writing on e-paper is notorious for being laggy, but here's how I made it snappy on cheap hardware:

1. **Splitting the Work Across Two Cores:**  
   The ESP32-S3 has two CPU cores. Core 0 focuses 100% on listening for pen touches (polling at 200 Hz so every flick of the pen is captured). Core 1 takes those pen coordinates, turns them into smooth lines using a custom Bresenham algorithm, and immediately flashes just the changed box on the screen via the UC8179's partial refresh mode.
2. **Never Losing Your Work:**  
   Every stroke and page is serialized in the background into lightweight `.epg` binary files and written to the FAT32 MicroSD card every 30 seconds. Even if the battery dies mid-sentence, your notes are safe.
3. **Grab Your Notes Over Wi-Fi:**  
   Flip on sync mode and the device fires up a lightweight web server. Open your phone or laptop browser, view your notebook pages, and download them cleanly as vector SVGs or images.
4. **Insane Battery Life:**  
   Because e-ink doesn't need any power to keep an image on screen, the display draws 0 Watts when you're just reading. When writing actively, the whole board pulls around 38 mA (~45+ hours of nonstop drawing), and in deep sleep between sessions it sips under 35 µA — easily lasting weeks on a single charge.

---

## Specs at a Glance

| Feature | Details |
|---|---|
| **Screen** | 5.83-inch Black & White E-Paper (Waveshare V2 HAT) |
| **Resolution** | 648 × 480 pixels (138 DPI, crisp paper-like finish) |
| **Refresh Speed** | ~0.3s partial refresh for smooth writing; ~3.5s full clear |
| **Brain / MCU** | ESP32-S3-DevKitC-1 (Dual-Core @ 240 MHz, 8MB PSRAM, 16MB Flash) |
| **Input Options** | 4-wire digitizer overlay + fine stylus (or mini QWERTY keyboard) |
| **Storage** | MicroSD card slot (FAT32) + onboard flash cache |
| **Battery & Charging** | 1800 mAh LiPo battery, USB-C charging, 45+ hrs active / weeks standby |
| **Connectivity** | 2.4 GHz Wi-Fi, Bluetooth LE, USB-C Serial & Mass Storage |
| **Total Build Cost** | **$62.40 USD** |
