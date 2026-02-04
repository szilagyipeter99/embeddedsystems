## Basic HTTP OTA update flow

1. Connect the device to a Wi-Fi network
2. Request a firmware image from a remote URL
3. Download the .bin file into an available OTA partition
4. Set the new partition as bootable
5. Restart the device
6. Boot into the updated firmware if the update was successful

## Missing features from the example

### Version checking:
Compare the currently running firmware version with the latest available version on the server and skip the update if they are the same

### Secure transport (HTTPS):
Use HTTPS with server certificate verification to prevent man-in-the-middle attacks and unauthorized OTA servers

### Firmware authenticity verification:
Sign firmware images and verify their signatures on the device to ensure the firmware originates from a trusted source and has not been tampered with

### Partial image download:
Download firmware images in chunks instead of a single continuous transfer, reducing memory usage and improving reliability on unstable network connections

### OTA resumption:
Resume an interrupted OTA download from the last successfully written offset instead of restarting the download from the beginning after a connection failure or reboot
