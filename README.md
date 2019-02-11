Wireless Sensor Signal Tester
=============================

I have several wireless sensors based on TI's CC110L BoosterPack [430BOOST-CC110L][1] placed around my house and yard.

Since I sometimes need to place sensors at the fringe of the reception area, it is useful to have a signal tester with a display showing signal strength and link quality. This tester can also be used as a debugging tool if one of the sensors stops sending data or sends incorrect data.

This design is based on an [MS430G2ET LaunchPad][2] and a 2x16 NewHaven [OLED display][3], along with the [CC110L BoosterPack][1].

The code should port without changes to other MSP430 LaunchPads. Other displays can be used with minor software updates.

## Hardware ##
The sketch currently assumes the following pin configuration for the display:

    CS    - Pin 11
    SDIN  - Pin 12
    SCLK  - Pin 13
    RESET - Pulled High (not controlled by sketch)



## External Libraries ##
* [NewhavenOLED][4]


## References ##
* [CC110L BoosterPack][1]
* [MSP430G2ET LaunchPad][2]
* Newhaven 2x16 [OLED Display][3]
* CC110L-based Low Power [Temperature Sensor][5]
* CC110L-based Outdoor [Weather Sensor][6]
* CC110L-based Sensor [Receiver Hub][7]

[1]: http://www.ti.com/tool/430BOOST-CC110L
[2]: http://www.ti.com/tool/MSP-EXP430G2ET
[3]: https://www.newhavendisplay.com/specs/NHD-0216CW-AY3.pdf
[4]: https://gitlab.com/Andy4495/NewhavenOLED
[5]: https://gitlab.com/Andy4495/MSP430G2-Temp_Sensor
[6]: https://gitlab.com/Andy4495/Outdoor-Weather-Sensor
[7]: https://gitlab.com/Andy4495/Sensor-Receiver
