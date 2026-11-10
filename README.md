## Build
Dependencies:
- `arduino-cli`
- Libraries:
  - `Servo.h` (for stepper motor)
  - `MD_MAX72xx.h` (for LED matrix)

 Clone this repository and run `make upload` to compile upload the code to your Arduino.

## Wiring
- 8x8 LED matrix:
  - VCC: 5V
  - GND: GND
  - DIN: Pin 10
  - CS: Pin 11
  - CLK: Pin 13
- LM35 Temperature sensor:
  - GND: GND
  - VCC: VCC
  - OUT: A1
- Servo motor:
  - GND: GND
  - VCC: VCC
  - OUT: A9
