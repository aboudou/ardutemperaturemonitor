ArduTemperatureMonitor
======================

ArduTemperatureMonitor is a simple Arduino based temperature monitor, with display of current, minimal and maximal temperature, with EEPROM storage of minimal and maximal values to survive power loss.

* [Project's page](http://goddess-gate.com/projects/en/arduino/ardutemperaturemonitor)


Requirements
------------

ArduTemperatureMonitor use few items:

* An [Arduino Uno](http://arduino.cc/en/Main/ArduinoBoardUno) board.
* A [TMP36](http://www.adafruit.com/products/165) temperature monitor.
* A 0.1 µF capacitor to filter voltage spike on TMP36 input.
* An [Hitachi HD44780](http://www.adafruit.com/products/181) compatible LCD display.
* A 220Ω resistor.
* A 10kΩ potentiometer to set LCD contrast.

You can find all the schematics into “doc/ArduTemperatureMonitor.fzz”, to open with [Fritzing](http://fritzing.org/).

The breadboard assembly and schematics
--------------------------------------

![Breadboard assembly](/imgs/Assembly.png "Breadboard assembly")
![Schematics](/imgs/Schematics.png "Schematics")


How does it work
----------------

The algorithm of ArduTemperatureMonitor is pretty simple:

1. Min and max temperature are read from EEPROM if available.
2. Current temperature is measured ten times, and the median value is kept.
3. Min and max temperature are updated to EEPROM if needed.
4. Current, min and max temperature are displayed.
5. Wait for 5 seconds then go to step 2.

The live assembly
-----------------

![Live assembly](/imgs/TemperatureMonitor.jpg "Live assembly")
![Details of LCD display](/imgs/TemperatureMonitorDetails.jpg "Details of LCD display")
