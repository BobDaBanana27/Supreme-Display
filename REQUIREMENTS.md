# Product Requirements & System Scope - Supreme Display 5.83"

## 1. Product Goal
Supreme Display is an ultra-affordable, open-source 5.83" DIY e-paper writing tablet and distraction-free digital notebook. Designed to be completely buildable under a strict **$65 USD** budget for the Hack Club Half Life 2 grant program.

## 2. Functional Requirements
- **FR1 (Display Performance):** 5.83" high-contrast monochrome e-paper display (648 x 480 pixels, 138 DPI) using UC8179 controller with fast partial refresh support (~0.3s) for responsive drafting.
- **FR2 (Input Versatility):** Support responsive pen writing/sketching via an ultra-thin 4-wire digitizer overlay with fine-tip stylus, or physical QWERTY typing via I2C/USB keyboard.
- **FR3 (Storage & Persistence):** Automatically serialize strokes and text documents to an onboard FAT32 MicroSD card every 30 seconds with zero risk of document loss during power-off.
- **FR4 (Wireless Sync):** Built-in Wi-Fi 802.11 b/g/n (ESP32-S3) hosting an embedded web server to export notes, SVG vector sketches, and text files directly to phones and laptops.
- **FR5 (Battery Autonomy):** 1800mAh 1S LiPo battery delivering >45 hours of active writing and >3 weeks of deep-sleep standby time (<35 µA quiescent draw).

## 3. Physical & Technical Constraints
- **Total Budget:** Bill of Materials (BOM) strictly capped under **$65 USD** ($62.40 achieved).
- **Enclosure Size:** Compact pocket footprint (~135 mm x 108 mm x 14 mm), easily 3D printable on any standard hobby printer.
- **Microcontroller:** ESP32-S3 (Dual-core 240 MHz, 16MB Flash, 8MB PSRAM) with native USB-C charging and debugging.
