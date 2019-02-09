Wireless Sensor Signal Tester
=============================

I have several wireless sensors based on CC110L <add link and better description> placed around my house and yard.

There are times when my receiver hub <reference> stops receiving signals from the wireless sensors.

Also when I want to place a new sensor, need to test signal strength and quality.

Explain hardware design.
- Note other LaunchPads will work, check pin compatibility
   -- Check if current pin layout is compatible with G2 old style
   -- Spot check other Energia supported MSP430 LaunchPads
- 2x16 NewHaven OLED
   - Could be adapted to other displays

Explain software design.

Explain pin map.

Explain channel selection.

Explain display layout, including the status indicator.
--> Update code header to indicate position of status indicator with other info.

External libraries:
Newhaven

References:
CC110L
MSP430G2ET
Fuel Tank II
Weather station
G2 temp sensor
Pond sensor (upload to Github if not already, shows how to use oneWire and DS temp sensor)
Rx hub



## External Libraries ##
* [NewhavenOLED][3]


## References ##
* [MSP430FR6989 LaunchPad][1]
* [CC110L BoosterPack][2]


[1]: http://www.ti.com/tool/MSP-EXP430FR6989
[2]: http://www.ti.com/tool/430BOOST-CC110L
[3]: https://gitlab.com/Andy4495/NewhavenOLED
