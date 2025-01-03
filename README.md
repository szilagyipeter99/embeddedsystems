# Embedded systems

This repository contains the examples created in the Embedded systems practices. Each folder includes a project with the source code written in the Arduino IDE, tested on the Espressif S3-DevKitC-1U-N8R8.

### Required steps before testing examples:

1. Install Arduino IDE:
[Click here](https://docs.arduino.cc/software/ide/)

2. Install Arduino-ESP32 Core: 
[Click here](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html#installing-using-arduino-ide)

3. Create separate folders for each example

### Recommended folder structure:

By creating a separate folder for every example, the IDE will import all the files by itself. Simply open __example-X.ino__ to view a project. Creating files is possible inside the IDE or using the Terminal. An __example-X__ folder is also required!

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

### If your old code is cached:

Sometimes the IDE will cache the code and even after changes, it will try to compile and upload the old cached version. If this has happened, change the verbosity level in the __Core Debug Level__ settings, this will force a clean.

### For your use and inspiration

All materials are designed for Mechatronics Engineering students at the University of Debrecen and were created by Péter Szilágyi. Feel free to share, copy, or modify any part of the materials to your heart's content.
