# Mini Music Player

A rechargeable music player built around an ESP32-S3. The design is intended to play music stored on a microSD card and send stereo audio to standard headphones through a 3.5 mm jack. I made it to learn how the digital, audio, charging, PCB, and enclosure parts of a small portable device fit together.

## How it works

The ESP32-S3 is the central controller. It reads music data from the microSD socket and sends digital audio over I²S to a PCM5102 digital-to-analog converter. The analog stereo signal then passes through a TPA6132A2 headphone amplifier and out through the 3.5 mm headphone jack. A ST7789 TFT is included as the user display. A TP4056-based circuit handles single-cell Li-ion charging, and a boost converter provides the higher supply rail required by the audio section. The battery, display, card socket, audio parts, and connectors are placed on the custom PCB and fit into the two-piece printed enclosure.

The KiCad schematic is the wiring reference for this design. The connections are intended to be made by the PCB, rather than by a separate hand-wired harness. Review the schematic and component datasheets before ordering or powering a board; this design has not yet been documented as a verified working build.

## Design files and images

Open the KiCad project in `PCB/` to inspect or edit the schematic and board. `Minimusicplayer.step` is the 3D assembly export, while `Case/CASE.stl` and `Case/LID.stl` are the enclosure parts.

### Schematic

![Full Mini Music Player schematic](https://halflife.hackclub-assets.com/sessions/u4daV3AON4Vg7wKuOmDpozDubBrsNyfk/c51166c4348e8c3b99aee869cab086759e380f646002a8d1210d19d6dc8d3293.png)

### PCB layout

![Mini Music Player PCB layout](https://halflife.hackclub-assets.com/sessions/u4daV3AON4Vg7wKuOmDpozDubBrsNyfk/4a613dc1209c3dc8749f77b8ff9950280e3eab495506b3e4bdd9225b35f10c44.png)

### Enclosure model

![3D view of the case and lid](https://halflife.hackclub-assets.com/sessions/u4daV3AON4Vg7wKuOmDpozDubBrsNyfk/eb6a17143430eff194e03bf3eea2d0e0715f57599777651ccee519e5ebebf4b8.png)

The case and lid STL files are included. A photo of the physical build will be added after the device has been assembled.

## Bill of materials

Prices are the estimates recorded during design and may change by seller, region, and shipping. The total below sums the listed line items; it excludes shipping and does not include parts without a recorded price.

| Part | Qty. | Supplier | Unit price | Line total | Purchase link |
| --- | ---: | --- | ---: | ---: | --- |
| 2.25-inch ST7789 TFT LCD, 2.8–3.3 V | 1 | AliExpress | $6.05 | $6.05 | [Product listing](https://www.aliexpress.com/item/1005009266053584.html) |
| Waveshare ESP32-S3-WROOM-1-N8R8 | 1 | BerryBase / Waveshare | $16.88 | $16.88 | [Product page](https://www.berrybase.de/en/waveshare-esp32-s3-wroom-1-n8r8-microcontroller-aiot-wi-fi-bt-ble-240mhz-dual-core-without-header) |
| PCM5102 I²S DAC | 1 | Adafruit | $4.95 | $4.95 | [Product page](https://www.adafruit.com/product/6250) |
| DC-DC boost converter | 1 | Kuriosity | $4.40 | $4.40 | [Product page](https://kuriosity.sg/products/dc-dc-step-up-boost-converter-2-5-vout-to-5-12v-1a-5w) |
| SJ1-3535NG 3.5 mm stereo jack | 1 | DigiKey | $2.22 | $2.22 | [Product page](https://www.digikey.sg/en/products/detail/same-sky-formerly-cui-devices/SJ1-3535NG/738699) |
| Custom PCB | 1 | JLCPCB | $2.00 | $2.00 | [PCB service](https://jlcpcb.com) |
| PCB assembly (PCBA) | 1 | JLCPCB | $29.83 | $29.83 | [Assembly service](https://jlcpcb.com) |
| **Listed parts subtotal** |  |  |  | **$66.33** |  |

The schematic also contains a TP4056 charger, TPA6132A2 headphone amplifier, microSD socket, and other passives/connectors. Their individual purchase links and prices are not present in the current `BOM.csv`, so the subtotal is incomplete. Check `BOM.csv` for the source list and update it as these costs are confirmed.

## Build status

The schematic, PCB layout, STEP model, and enclosure meshes are available in this repository. Firmware, assembly instructions, a hand-wiring diagram, and real-device photos have not been added yet. If you build the design, verify the PCB against the schematic and check all battery and power connections before inserting a cell.
