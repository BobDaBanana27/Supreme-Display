# Supreme Display (EinkPad) - DIY 5.83" E-Paper Writer & Tablet

Supreme Display is an open-source 5.83" DIY e-paper writing tablet and distraction-free digital notebook engineered around paper-like visual comfort, ultra-long battery endurance, and an accessible sub-$65 budget ($62.40 total). Designed for the Hack Club Half Life 2 grant program, it delivers an open hardware and software platform for notes, drafting, and sketches without notification distractions or blue light eye fatigue.

---

## Hardware Architecture & Visual Overview

### 1. System Interconnect Architecture
![System Bus & Interconnect Architecture](docs/images/architecture.svg)

The hardware revolves around an **ESP32-S3-WROOM-1** microcontroller (dual-core Xtensa LX7 @ 240 MHz with 8 MB Octal PSRAM and 16 MB flash) interfacing with:
- **Display:** 5.83-inch Waveshare E-Paper display (648 × 480 pixels, UC8179 controller) driven over a dedicated 4-wire SPI bus supporting partial fast refresh (~0.3s) for responsive drafting.
- **Input:** Ultra-thin 4-wire digitizer overlay with an XPT2046 SPI controller and fine-tip stylus, with support for an alternate I2C QWERTY keyboard (e.g. CardKB / BB Q20) in typewriter mode.
- **Storage:** MicroSD SPI socket running FAT32 for local binary stroke (`.epg`) and plain-text file storage.
- **Power Management:** 1S 3.7V 1800 mAh LiPo battery, TP4056 USB-C charging with DW01A protection, and low-quiescent LDO regulator.

### 2. Electrical Schematic & Power Distribution
![System Schematic Diagram](docs/images/schematic.svg)

### 3. Mechanical Enclosure & Layer Stackup
![Mechanical Enclosure Cross-Section](docs/images/enclosure.svg)

The custom 3-piece snap-fit enclosure features an integrated stylus silo, recessed display bezel to protect the panel edges, and internal mounting bosses tailored for standard FDM 3D printing (PLA/PETG, ~90g material).

---

## How It Works

1. **Dual-Core Input & Refresh Pipeline:**
   - **Core 0 (Input Task):** Dedicated 200 Hz polling of pen coordinates, pressure levels, and hardware tactile keys via FreeRTOS queues.
   - **Core 1 (Rasterizer & Refresh):** Renders strokes into a 38.8 KB 1bpp framebuffer using a Bresenham algorithm with variable brush sizes and issues windowed partial updates to the UC8179 controller.
2. **Persistence Engine:**
   - An asynchronous FreeRTOS storage task serializes vector strokes and notes into `.epg` files on the FAT32 MicroSD card every 30 seconds, eliminating risk of data loss.
3. **Wireless Sync & Web Viewer:**
   - Onboard Wi-Fi hosts a local HTTP server allowing you to browse, view, and export notebooks as standard SVG vectors or PNG bitmaps directly to your phone or computer.
4. **Power Autonomy:**
   - In active drafting mode, consumption averages ~38 mA (~45 hours of writing time). When idle, the device transitions to deep sleep (<35 µA quiescent draw), while the bistable e-paper retains the last screen image indefinitely with 0W power.

---

## Key Technical Specifications

| Parameter | Specification |
|---|---|
| **Display Panel** | 5.83" diagonal E-Paper Display (Waveshare V2 HAT) |
| **Resolution** | 648 × 480 pixels (138 DPI, monochrome black/white) |
| **Controller** | UC8179 (4-Wire SPI @ 10 MHz) with partial windowed refresh (~0.3s) |
| **Microcontroller** | ESP32-S3-WROOM-1 (Dual-Core 240 MHz, 512KB SRAM, 8MB PSRAM, 16MB Flash) |
| **Input Subsystem** | 4-wire resistive digitizer overlay + fine-point stylus (or I2C CardKB keyboard) |
| **Local Storage** | MicroSD FAT32 slot + 16MB onboard SPI flash |
| **Battery & Charging** | 1800 mAh 3.7V LiPo, TP4056 USB-C charger, >45 hours active / 3+ weeks standby |
| **Wireless** | 2.4 GHz Wi-Fi 802.11 b/g/n, BLE 5.0, local HTTP sync server |
| **BOM Cost** | **$62.40 USD** (Target: < $65.00 USD) |

---

## Repository Structure & 20-Hour Progression

This project is built and documented through 20 discrete hourly milestones:

- [20-Hour Progression Log (PROGRESSION.md)](PROGRESSION.md): Hourly development diary and deliverables breakdown.
- [Master Project Index (INDEX.md)](INDEX.md): Index of design documents, diagrams, and firmware stages.
- [Root Firmware Directory (firmware/)](firmware/): Complete dual-core ESP32-S3 FreeRTOS firmware and PlatformIO project.
- [Standalone Arduino Sketch (supreme_display.ino)](supreme_display.ino): Single-sketch Arduino IDE build.
- [Chunk 01: Project Foundation](CHUNK_01/README.md) - [BOM.md](CHUNK_01/BOM.md) ($62.40 budget) | [RESEARCH.md](CHUNK_01/RESEARCH.md) | [REQUIREMENTS.md](CHUNK_01/REQUIREMENTS.md)
- [Chunk 02: System Architecture](CHUNK_02/README.md)
- [Chunk 03: Electrical Design & Power Tree](CHUNK_03/README.md)
- [Chunk 04: GPIO Pin Allocation & Pinmap](CHUNK_04/PINMAP.md)
- [Chunk 07: Mechanical CAD & Enclosure](CHUNK_07/MECHANICAL.md)
- [Chunk 14–20: Incremental Firmware Implementation](CHUNK_20/README.md)

---

## Quick Start & Firmware Flashing

### Option A: Using PlatformIO (VS Code)
```bash
# Navigate to firmware directory
cd firmware

# Build and flash to ESP32-S3
pio run --target upload

# Open serial diagnostic monitor
pio device monitor -b 115200
```

### Option B: Using Arduino IDE
1. Open the [supreme_display.ino](supreme_display.ino) sketch in the Arduino IDE.
2. Select **ESP32S3 Dev Module** under **Tools > Board > ESP32 Arduino**.
3. Enable **PSRAM: OPI PSRAM** and set **Flash Size: 16MB**.
4. Connect the USB-C cable and click **Upload**.
