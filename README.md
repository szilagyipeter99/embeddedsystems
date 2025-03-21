# Embedded systems

This repository contains the examples created in the Embedded systems practices. Each folder includes a project with the source code written in the Arduino IDE, tested on the Espressif S3-DevKitC-1U-N8R8.

### Required steps before testing examples:

1. Install Arduino IDE:
[Click here](https://www.arduino.cc/en/software)

2. Install Arduino-ESP32 Core: 
[Click here](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html#installing-using-arduino-ide)

3. Install the CP210x driver:
[Click here](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads)

4. Create separate folders for each example

### Recommended folder structure:

The IDE requires the __.ino__ files to be in a folder with the same name, so an __example-X__ folder is needed for every __example-X.ino__ file! Additional __.h__ and __.cpp__ files are automatically imported.

```bash
embed-examples-folder
├── example-1
│   ├── example-1.ino
│   ├── something.h
│   └── something.cpp
├── example-2
│   ├── example-2.ino
│   └── ...
└── ...
```

### Documentations:

1. Arduino-ESP32 Core:
[Click here](https://docs.espressif.com/projects/arduino-esp32/en/latest/libraries.html#apis)

2. Espressif ESP-IDF(v5.4): 
[Click here](https://docs.espressif.com/projects/esp-idf/en/v5.4/esp32/api-reference/index.html)

### If your old code is cached:

Sometimes the IDE will cache the code and even after changes, it will try to compile and upload the old cached version. If this has happened, change the verbosity level in the __Core Debug Level__ settings, this will force a clean.

### For your use and inspiration

All materials are designed for Mechatronics Engineering students at the University of Debrecen and were created by Péter Szilágyi. Feel free to share, copy, or modify any part of the materials to your heart's content.
