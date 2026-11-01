# System Architecture & Bus Topology

## Subsystem Partitioning
The Supreme Display architecture is divided into three key domains:
1. **Real-Time Input & Rendering Domain (Core 1):**
   - EMR Digitizer on HSPI (8 MHz) with hardware interrupt line.
   - Vector stroke rasterizer rendering into 1bpp PSRAM canvas.
   - IT8951 display controller on FSPI (24 MHz) with fast A2 partial refresh.
2. **Persistence & System Services Domain (Core 0):**
   - MicroSD card storage on dedicated SPI bus (25 MHz) with `SdFat`.
   - LVGL UI timer handler and gesture engine.
   - TP4056 / ADC battery voltage monitoring.
3. **Connectivity & Synchronization Domain (Core 0):**
   - Asynchronous HTTP WebServer for wireless notebook browsing.
   - JSON status telemetry and vector SVG note export.

## Memory Budget
- **ESP32-S3 SRAM (512 KB):** FreeRTOS task stacks, event queues, Wi-Fi buffers, DMA buffers.
- **ESP32-S3 Octal PSRAM (8 MB):**
  - Primary 1bpp display framebuffer: 1872 x 1404 / 8 = 328,548 bytes (~321 KB).
  - Background vector stroke buffer: ~1 MB.
  - Scratch render surface: 328 KB.
  - Free PSRAM remaining: >6 MB.
