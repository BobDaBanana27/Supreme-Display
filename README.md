# Supreme Display (EinkPad) 🖊️📄

An open-source, pocket-sized 5.83" DIY e-paper writing tablet and distraction-free notebook built for **Hack Club Half Life 2** — engineered from scratch to come in under a strict **$65 budget** (total cost: **$62.40**!).

Tired of backlit screens, pop-up notifications, and battery anxiety while trying to take notes or brainstorm? Supreme Display gives you the feel of actual pen and paper with the perks of a modern digital workflow: instant partial screen updates, weeks of battery life, automatic saving to MicroSD, and wireless file syncing straight to your phone or laptop.

---

## What It Looks Like & How It's Laid Out

### 1. How the Parts Talk to Each Other
![System Architecture Diagram](docs/images/architecture.svg)

At the heart of the build is an **ESP32-S3** board (dual-core 240 MHz, 8 MB extra PSRAM, and 16 MB flash) driving three main subsystems:
- **5.83" Waveshare E-Paper Display:** Crisp 648 × 480 black-and-white screen with fast partial refreshes (~0.3s) so drawing and typing don't feel sluggish.
- **Touch / Input Layer:** A transparent 4-wire resistive overlay that tracks any fine-tip stylus with zero fuss (or swap in a mini I2C keyboard like the CardKB if you'd rather type like an old-school typewriter!).
- **Storage & Power:** An SD card slot for saving your notes, a rechargeable 1800 mAh LiPo battery, and a standard USB-C port for charging.

### 2. Wiring & Schematic
![Schematic Diagram](docs/images/schematic.svg)

Clean 3.3V power routing, battery protection via TP4056 + DW01A, and dedicated high-speed SPI pins for the display and digitizer.

### 3. Case & Assembly Stackup
![Enclosure Diagram](docs/images/enclosure.svg)

A clean, 3-piece 3D-printable snap-fit enclosure designed to protect the glass display edges, house the battery safely, and store the stylus right in the side bezel. Prints easily on any standard hobby 3D printer using about 90 grams of PLA or PETG.

---

## How It Actually Works Under the Hood

Writing on e-paper is notorious for being laggy, but here's how we made it snappy on cheap hardware:

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
| **Total Build Cost** | **$62.40 USD** (Comfortably under the $65 goal!) |

---

## How the Project Was Built (20-Hour Progress)

We broke down the entire build into 20 bite-sized hourly chunks so every single step is transparent and documented:

- [Check out our 20-Hour Build Log (PROGRESSION.md)](PROGRESSION.md) for the hour-by-hour diary.
- [Full Project Index (INDEX.md)](INDEX.md) for quick links to every design file.
- [Hour 01 Foundation (CHUNK_01/)](CHUNK_01/README.md): [Complete BOM ($62.40)](CHUNK_01/BOM.md), [Hardware Trade Study](CHUNK_01/RESEARCH.md), and [Requirements](CHUNK_01/REQUIREMENTS.md).
- [Hours 02–13](INDEX.md): System architecture, schematics, pin maps, mechanical CAD, battery tests, and the physical assembly guide.
- [Hours 14–20](CHUNK_20/README.md): Incremental firmware development, from FreeRTOS tasks to the display driver, touch calibration, and the web sync server.

---

## Flashing the Code

All the code is ready to compile and flash onto an ESP32-S3 board.

### Using PlatformIO (Recommended)
1. Clone the repo and open the [`firmware/`](firmware/) folder in VS Code with the PlatformIO extension installed.
2. Plug in your ESP32-S3 via USB-C.
3. Hit upload:
   ```bash
   pio run --target upload
   ```
4. Check the diagnostic logs:
   ```bash
   pio device monitor -b 115200
   ```

### Using Arduino IDE
1. Open [`supreme_display.ino`](supreme_display.ino) in the Arduino IDE.
2. In **Tools > Board**, pick **ESP32S3 Dev Module**.
3. Under **Tools**, make sure **PSRAM: OPI PSRAM** is enabled and **Flash Size** is set to **16MB**.
4. Plug in your board and click **Upload**.

Have fun hacking! 🚀
