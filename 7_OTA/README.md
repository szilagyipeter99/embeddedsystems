## Basic HTTP OTA update flow

1. Connect the device to a Wi-Fi network
2. Visit a remote URL to fetch the latest firmware version number
3. If the current version is older than the latest, continue
4. Request a firmware image from another remote URL
5. Download the .bin file into an available OTA partition
6. Set the new partition as bootable
7. Restart the device
8. Boot into the updated firmware if the update was successful

## Neccessary `sdkconfig` settings:

- Select __Factory app, two OTA definitions__
- Tick __Allow HTTP for OTA__

## Steps to upload a new firmware to the OTA server:

1. Click on __Build Project__
2. Look for the `project-name.bin` file in the __build__ folder
3. Copy the file to another place where it is safe to rename it
4. Rename and ???
5. ???

## Missing features from the example

### Secure transport (HTTPS):
Use HTTPS with server certificate verification to prevent man-in-the-middle attacks and unauthorized OTA servers

### Firmware authenticity verification:
Sign firmware images and verify their signatures on the device to ensure the firmware originates from a trusted source and has not been tampered with

### Partial image download:
Download firmware images in chunks instead of a single continuous transfer, reducing memory usage and improving reliability on unstable network connections

### OTA resumption:
Resume an interrupted OTA download from the last successfully written offset instead of restarting the download from the beginning after a connection failure or reboot
