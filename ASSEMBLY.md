# Hardware Assembly Guide & Bring-Up Manual

## Step 1: Pre-Assembly Inspection & Flashing
1. Flash ESP32-S3 with initial diagnostic test build via USB-C.
2. Verify 3.3V rail voltage on development board pins before connecting displays.

## Step 2: Soldering & Harness Preparation
1. Solder 100k/100k battery divider resistors to GPIO 19.
2. Wire TP4056 output pads to battery connector and LDO input.
3. Wire tactile pushbuttons (Power, Home, Back) with 10k pull-up resistors.

## Step 3: Mechanical Stack Installation
1. Press 6x M2.5 brass heat-set inserts into front bezel using a soldering iron at 220 deg C.
2. Clean inside of front bezel; seat 10.3" e-paper display with cushioning foam gasket.
3. Adhere EMR sensor digitizer film directly onto back of e-paper panel, routing FPC upward.
4. Mount internal mid-frame chassis and secure with 4x M2.5 screws.
5. Secure ESP32-S3 board, SD adapter, and TP4056 on standoffs.
6. Connect FPC cables: IT8951 ribbon to display, digitizer ribbon to controller board.

## Step 4: Battery & Final Closure
1. Place 3500 mAh LiPo battery in rear bay using double-sided high-bond adhesive tape.
2. Connect battery JST connector to TP4056 board.
3. Snap rear enclosure shell onto front bezel; fasten 6x M2.5 perimeter machine screws.
4. Slide stylus into side silo to test magnetic latch.
