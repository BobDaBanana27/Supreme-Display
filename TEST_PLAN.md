# Verification Matrix & Bench Test Plan

| Test ID | Subsystem | Description & Procedure | Acceptance Criteria | Pass/Fail |
|---|---|---|---|---|
| **TP-01** | Power Rail | Measure 3.3V rail voltage under full CPU load. | Voltage within 3.25V - 3.35V. | PASS |
| **TP-02** | Display Full | Issue full GC16 refresh with checkerboard test pattern. | Crisp black/white grid, zero missing lines. | PASS |
| **TP-03** | Display Partial | Issue 100 fast A2 partial refresh updates to 200x200 window. | Refresh latency <140ms per update. | PASS |
| **TP-04** | Digitizer Rate | Sweep stylus across screen for 5 seconds; count samples. | Report rate >= 200 Hz. | PASS |
| **TP-05** | Affine Cal | Measure pen tip visual parallax at 4 corners and center. | Parallax error < 0.5 mm. | PASS |
| **TP-06** | SD Auto-Save | Write 50 strokes, power-cycle after 35 seconds. | Page reloads from SD with all strokes intact. | PASS |
| **TP-07** | Wi-Fi Sync | Connect phone to SoftAP, request `/api/status`. | Valid JSON returned with battery and storage stats. | PASS |
