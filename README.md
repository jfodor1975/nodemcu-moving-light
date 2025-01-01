# nodemcu-moving-light

Artnet moving light using WS2812b led's and Sg90 servo's


ESP32 C3 super mini board

D7 = pan servo signal
D6 = tilt servo signal
D2 = WS2812B data

7 pixels led board.

25 channels are used, channel 26 is for later deveolpment if required.

Channel mapping
| Channel |   Function  | 
|    1    | Pan 8 bit   |
|    2    | Pan 16 bit  |
|    3    | Tilt 8 bit  |
|    4    | Tilt 16 bit |
|    5    | Led 1 Red   |
|    6    | Led 1 Green |
|    7    | Led 1 Blue  |
|    8    | Led 2 Red   |
|    9    | Led 2 Green |
|    10   | Led 2 Blue  |
|    11   | Led 3 Red   |
|    12   | Led 3 Green |
|    13   | Led 3 Blue  |
|    14   | Led 4 Red   |
|    15   | Led 4 Green |
|    16   | Led 4 Blue  |
|    17   | Led 5 Red   |
|    18   | Led 5 Green |
|    19   | Led 5 Blue  |
|    20   | Led 6 Red   |
|    21   | Led 6 Green |
|    22   | Led 6 Blue  |
|    23   | Led 7 Red   |
|    24   | Led 7 Green |
|    25   | Led 7 Blue  |
|    26   | Reset/test  | this channel is not implemented
*/

Wire colours
Servo:
Red - 5v+
Brown - Gnd
orange - Data

LED borad:
Red - 5v+
Black - Gnd
Blue - Data



change log
Dec 2024: rewrote all the code to work with the Esp32 c3 super mini board.

ToDo:
- figure out how to get smoother sero movements, maybe try the servo easing library.
- add  feed back from led's on wifi connection. 
    - single LED dim about 10% led #7 thats the one i the center 
    - green to show a good conection 
    - purple for config protal open
    - red for no protal or connection 

- see about chaning the servo timing to relate to the value change of the artnet vaules.

