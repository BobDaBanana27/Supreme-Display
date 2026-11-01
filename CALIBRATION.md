# Digitizer Calibration Matrix Math

The 6 affine parameters are solved using a least-squares linear regression against 4 known calibration targets:
- Point 1: (100, 100)
- Point 2: (1772, 100)
- Point 3: (1772, 1304)
- Point 4: (100, 1304)

When touch coordinates are sampled at each target crosshair, the system builds an over-determined system of equations A * x = B and solves for the optimal matrix coefficients, storing them in NVS partition 'cal'.
