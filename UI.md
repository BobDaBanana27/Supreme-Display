# Minimalist UI / UX Design & Palm Rejection

## On-Screen Toolbar Layout
A floating 1bpp minimalist toolbar is pinned to the bottom margin:
- **[Pen]**: Selects pressure-sensitive writing brush (1 to 5 pixel diameter).
- **[Eraser]**: Selects disc eraser tool (14 pixel diameter) clearing bits to white.
- **[Clear]**: Clears active page with full GC16 screen refresh to eliminate ghosting.
- **[Page < >]**: Page navigation with current page number indicator.
- **[Status]**: Battery percentage icon and SD storage capacity indicator.

## Palm Rejection Algorithm
When the EMR stylus approaches within 10mm of the surface, the digitizer triggers hardware hover events. While hover or pen-down is detected, any capacitive or stray surface contacts are immediately ignored by the input task.
