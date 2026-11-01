# Storage Engine Architecture & .epg File Format

## MicroSD Filesystem Organization
```
/sdcard/
|-- config/
|   `-- settings.json       # Wi-Fi credentials, sleep timers, user prefs
`-- notes/
    |-- metadata.json       # Notebook index and page counters
    `-- nb_001/
        |-- page_0001.epg   # Binary vector stroke data
        `-- page_0002.epg
```

## Binary .epg (E-Paper Graphics) Specification
- **Bytes 0-3:** Magic Identifier "EPG1" (0x45, 0x50, 0x47, 0x31).
- **Bytes 4-5:** File Format Version (uint16_t, value = 1).
- **Bytes 6-7:** Canvas Width in Pixels (uint16_t, 1872).
- **Bytes 8-9:** Canvas Height in Pixels (uint16_t, 1404).
- **Bytes 10-11:** Screen DPI (uint16_t, 226).
- **Bytes 12-15:** Creation Timestamp (uint32_t, UNIX seconds).
- **Payload:** Variable-length stroke records (Tool ID, stroke color, point count, packed (X, Y, P) tuples).
