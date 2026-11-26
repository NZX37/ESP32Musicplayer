# Firmware (first draft)

`MiniMusicPlayer/MiniMusicPlayer.ino` is an Arduino starting point for the
Waveshare ESP32-S3-WROOM-1-N8R8. It initializes the ST7789 display and microSD
card, then plays MP3 files in `/music` in card order and outputs I²S audio to the
PCM5102. The screen currently shows startup errors and the playing filename.

## Build

1. Install Arduino IDE 2 and the Espressif ESP32 board package. Select an
   ESP32-S3 board; configure 8 MB flash and 8 MB PSRAM for the N8R8 module.
2. Install these libraries through Library Manager:
   - Adafruit GFX Library
   - Adafruit ST7735 and ST7789 Library
   - ESP8266Audio
3. Open `Firmware/MiniMusicPlayer/MiniMusicPlayer.ino`, select the correct
   serial port, and upload. Open Serial Monitor at 115200 baud for status.
4. Format a card as FAT32 and copy `.mp3` files into `/music`.

## PCB pin map to verify

The assignments in the sketch are a first interpretation of the current KiCad
PCB and schematic. Verify continuity and the exact nets on the manufactured
board before relying on playback. Change the constants near the top of the
sketch if the PCB changes.

| Function | GPIO |
| --- | ---: |
| Shared SPI SCK / MOSI | 12 / 11 |
| TFT CS / DC / RESET / backlight | 38 / 9 / 2 / 21 |
| microSD CS / MISO | 10 / 13 |
| PCM5102 I²S BCLK / LRCK / data | 41 / 40 / 39 |
| TPA6132A2 G0 / G1 | 18 / 17 |

The screen dimensions and rotation may need adjustment for the selected ST7789
module. This sketch has no physical button controls yet, supports MP3 only,
and has not been validated against assembled hardware. Start with a low output
gain and headphones disconnected while checking power, I²S wiring, and the
amplifier stage.
