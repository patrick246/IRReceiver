# IR Receiver
This project utilizes a IR remote, IR receiver, an Arduino and a bit of PC side software to control music playback via the IR remote.

## Arduino wiring
<img src="img/arduino_wiring.png?raw=true" width="350px" alt="IR Vcc to 5V, IR GND to GND, IR Data to Port 11" />

## Hardware Parts
 - Arduino Uno
 - USB A to USB B cable
 - VS1838B or TSOP22.., TSOP24.., TSOP48.., TSOP44.. IR receiver

## How it works
The Arduino sends the IR commands over serial connection to the PC. The software reads them and executes the actions according to its programming. Currently the actions are hardcoded but it is easy to add more codes and actions.

## Setup
You need the IR remote library from [z3t0/Arduino-IRremote](https://github.com/z3t0/Arduino-IRremote) and transfer the IRrecvDemo onto the Arduino. It just acts as a dumb IR receiver to serial bridge.
Next you need to set the correct command mapping. You can see some constants in the Main.cpp source file. Run the IRrecvDemo and open the Arduino serial monitor. Then start pressing buttons you want to map on your remote. You can now replace those constants with the values that appeared in the serial monitor.
After you entered the right values, you need to check that the port and the baud rate matches the constants in the code. If not, change them.
The final step is to build the project. Project files for Visual Studio 2015 are in the IRReceiver folder, it should be possible to build with older versions of Visual Studio, you just need to add the Main.cpp file to the project, or build with another compiler like GCC.

*Please note that the project currently only supports Windows, as the WinAPI is used to control music playback*
