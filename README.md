# Wireless Sensor Signal Tester

[![Arduino Compile Sketches](https://github.com/Andy4495/Wireless-Signal-Tester/actions/workflows/arduino-compile-sketches.yml/badge.svg)](https://github.com/Andy4495/Wireless-Signal-Tester/actions/workflows/arduino-compile-sketches.yml)
[![Check Markdown Links](https://github.com/Andy4495/Wireless-Signal-Tester/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/Wireless-Signal-Tester/actions/workflows/CheckMarkdownLinks.yml)

I have several wireless sensors based on TI's CC110L BoosterPack [430BOOST-CC110L][1] placed around my house and yard.

Since I sometimes need to place sensors at the fringe of the reception area, it is useful to have a signal tester with a display showing signal strength and link quality. This tester can also be used as a debugging tool if one of the sensors stops sending data or sends incorrect data.

This design is based on an [MS430G2ET LaunchPad][2] and a 2x16 NewHaven [OLED display][3], along with the [CC110L BoosterPack][1].

The code should port without changes to other MSP430 LaunchPads. Other displays can be used with minor software updates.

## Hardware

The sketch currently uses the following pin configuration for the display. These can be changed up updating the associated `#define` in the sketch:

    CS    - Pin 11  // CS_PIN
    SDIN  - Pin 12  // SDIN_PIN
    SCLK  - Pin 13  // SCLK_PIN
    RESET - Pulled High external from MSP430 (not controlled by sketch)

## RX Channel

By default, the Signal Tester listens on CHANNEL_1 (as defined by the AIR430BoostUSAFCC library included with Energia). Pull pin 5 low during reset to select CHANNEL_3, or pin 6 to select CHANNEL_4. I currently don't have a sensor transmitting on CHANNEL_2, but the sketch can be easily modified to support CHANNEL_2.

## External Libraries

* [NewhavenOLED][4]

## References

* [CC110L BoosterPack][1]
* [MSP430G2ET LaunchPad][2]
* Newhaven 2x16 [OLED Display][3]
* CC110L-based Low Power [Temperature Sensor][5]
* CC110L-based Outdoor [Weather Sensor][6]
* CC110L-based Sensor [Receiver Hub][7]

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

[1]: https://www.ti.com/lit/ml/swru312b/swru312b.pdf
[2]: http://www.ti.com/tool/MSP-EXP430G2ET
[3]: https://www.newhavendisplay.com/specs/NHD-0216CW-AY3.pdf
[4]: https://github.com/Andy4495/NewhavenOLED
[5]: https://github.com/Andy4495/MSP430LowPowerTempSensor
[6]: https://github.com/Andy4495/Outdoor-Weather-Sensor
[7]: https://github.com/Andy4495/Wireless-Sensor-Receiver-Hub
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[200]: https://github.com/Andy4495/Wireless-Signal-Tester
