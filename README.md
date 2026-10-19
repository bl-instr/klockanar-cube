# Klockanar Cube

**A configurable 48-LED light ring for engineers who want a fun, wireless Blinky-Lite project.**

<img src="klockanar.gif" alt="klockanar-cube" width="800"/>

## Overview

The Klockanar Cube is a 48-LED ring based on the Raspberry Pi Pico W / RP2040 platform used throughout the Blinky-Lite Instrumentation lineup. It is designed for visual effects, displays, clocks, and creative experimentation.

The LEDs are embedded in a 3D-printed ring enclosure. STL files are included so the enclosure can be printed, reproduced, and adapted for custom installations.

The ring connects over 2.4 GHz Wi-Fi and uses MQTT publish/subscribe communication for remote configuration and integration with Blinky-Lite.

### Features

- 48 controllable LEDs: Individually controllable LEDs arranged in a full ring.
- 3D-printed design: The LED ring is embedded in a printed enclosure; STL files are included.
- Display modes: Glow Ring, Comet, Collide, Explode, Mood, Colorwave, and Clock.
- Adjustable effects: Intensity, hues, and timing can be configured for every mode.
- Wireless control: 2.4 GHz Wi-Fi and MQTT publish/subscribe communication.
- Shared controller platform: Raspberry Pi Pico W / RP2040.
- Open-source design: MIT-licensed firmware and adaptable 3D-print files.

### Technical summary

| Specification | Details |
|---------------|---------|
| LEDs | 48 individually controllable LEDs in a ring |
| Modes | Glow Ring, Comet, Collide, Explode, Mood, Colorwave, Clock |
| Configuration | Adjustable intensity, hue, and timing |
| Enclosure | 3D-printed ring; STL files included |
| Power input |  5 V DC via 2.1 mm barrel connector |
| Wireless | 2.4 GHz Wi-Fi |
| Communication | MQTT publish/subscribe protocol |
| Controller | Raspberry Pi Pico W / RP2040 |

### Typical applications

- Desk, laboratory, and workshop lighting
- Visual status and notification displays
- Clock and ambient-light displays
- Interactive demonstrations and engineering outreach
- Custom LED effects and experimental visualisation

## Getting started


### Hardware

1. Print or obtain the ring enclosure using the included STL files.
2. Install the 48-LED ring in the printed enclosure according to the assembly documentation.
3. Connect a regulated 5 V DC supply through the 2.1 mm barrel connector.
4. Place the ring where its light output and cable routing suit the intended application.

### Firmware build target

The firmware targets the Raspberry Pi Pico W / RP2040 using the Arduino-Pico board support.
- Board: Pico W / RP2040-compatible target
- Flash layout: Sketch 1920 KB, filesystem 128 KB
- FQBN flash option: flash=2097152_131072

### Open-source software and licensing

The firmware is released under the MIT License, giving users broad permission to study, modify, and adapt the code for their own applications. The open hardware design and included STL files can likewise be customised for creative or site-specific requirements. The firmware uses the open-source Arduino-BlinkyPicoW library, which is also available under the MIT License.

### Configuration

1. Build and upload the firmware using the project source and configuration files.
2. Use the onboard web interface to set or update Wi-Fi credentials.
3. Configure the MQTT connection and create the corresponding device integration in Blinky-Lite.
4. Select Glow Ring, Comet, Collide, Explode, Mood, Colorwave, or Clock mode.
5. Adjust the intensity, hues, and timing for the chosen effect.

### Integration model

The Klockanar Cube uses MQTT to receive configuration and mode settings. This event-driven model enables remote control of its visual effects through a Blinky-Lite deployment.

Installation considerations

- Use a regulated 5 V DC supply with adequate capacity for the selected LED intensity and effect.
- Protect the ring and electronics from moisture, heat, and mechanical damage unless the enclosure has been adapted for those conditions.
- Use the included STL files as a starting point when adapting the enclosure, mounting method, or LED presentation.


