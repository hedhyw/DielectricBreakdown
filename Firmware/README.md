# Dielectric Breakdown Device Firmware
The sources are designed for the Atmega48a microcontroller.

#### How to build?
1. Clone the repository
2. Enter the _Firmware_ folder
3. Run `make` in the terminal.

#### How to flash?
1. Connect usbasp programmer.
2. Set fuse bytes yourself. Be careful. External quartz is installed. My low fuse byte is `0xE6`. High fuse byte is `0xDF`.
_You can read your fuse bytes, for this run `make fuse_read`._
3. Run `make upload` for uploading firmware.