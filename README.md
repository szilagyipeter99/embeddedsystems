# Embedded systems

This repository contains the examples created in the Embedded systems practices. Each folder includes the source code for an ESP32 project written in the Arduino IDE. 

### Required steps before testing examples:

1. Install Arduino IDE:
[Click here](https://docs.arduino.cc/software/ide/)

2. Install Arduino-ESP32 support: 
[Click here](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html#installing-using-arduino-ide)

3. Create separate folders for each example

### Recommended folder structure:

By creating a separate folder for every example, the IDE will import all the files by itself. Simply open __main.ino__ to view a project. Creating new files is also possible inside the IDE or using the Terminal.

```bash
embed-examples-folder
├── example-1
│   ├── main.ino
│   ├── something.h
│   ├── something.cpp
│   └── ...
├── example-2
│   ├── main.ino
│   └── ...
└── ...
```

### If your old code is cached:

Sometimes the IDE will cache the currect code and will try to compile and upload the old cached version. If this has happened, change the verbosity level in the __Core Debug Level__ settings, this will force a clean.


### Course website: [HU](https://szilagyipeti.hu/embedded/content.html) | [EN](https://szilagyipeti.hu/embedded/content-en.html)

All materials are designed for Mechatronics Engineering students at the University of Debrecen and were created by Péter Szilágyi. Feel free to share, copy, or modify any part of the materials to your heart's content.
