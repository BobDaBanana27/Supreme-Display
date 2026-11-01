# Display and Digitizer Integration Specifications

## 1. IT8951 Waveform Timing
- **Full Refresh (GC16 Mode):** 16-level grayscale waveform (~800 ms duration). Used when opening documents or clearing the canvas to remove residual ghosting particles.
- **Fast Partial Refresh (A2 Mode):** 1-bit monochrome binary waveform (~120 ms duration). Streams bounding-box dirty rectangles around pen strokes with high responsiveness.

## 2. Affine Coordinate Calibration
Because manufacturing tolerances introduce mechanical translation, scaling, and slight rotation between the EMR digitizer film and the e-paper glass, a 6-parameter affine transformation matrix maps raw digitizer ADC values (x_raw, y_raw) into screen pixel space (x_disp, y_disp):

x_disp = a * x_raw + b * y_raw + c
y_disp = d * x_raw + e * y_raw + f

Coefficients a, b, c, d, e, f are computed during 4-point crosshair calibration and saved into ESP32 non-volatile storage (NVS).
