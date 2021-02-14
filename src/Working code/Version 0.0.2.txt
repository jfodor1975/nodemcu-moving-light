/*
* Moving light code
*

Node MCU pins

D1 = pan servo signal
D2 = tilt servo signal
D6 = WS2812B data


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
|    26   | Reset/test  |


codeing Notes:
- commented out #define FASTLED_ALLOW_INTERRUPTS 0 to try to remove random colour flicker
- added 20us delay at the end of the main loop to try and remove random flicker
- added FastLED.setDither(0); to combat random led flashing while at 255 bightness.
-nope not it: wondering if a shieled data line is needed for the led data to remove flicker. so far this might be the answer 
        as cross talk between the servo data lines might have interfered with the led data line in bread board testing.
- might need a resitor on the led data line




*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <E131.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ALLOW_INTERRUPTS 0
//#define FASTLED_INTERRUPT_RETRY_COUNT 1
//#include <FastLED.h>
#include <Wifi.h> // wifi information 



#define LED_PIN     D6
#define COLOR_ORDER GRB
#define CHIPSET     WS2812b
#define NUM_LEDS    7
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ400);


//CRGB g_LEDs[NUM_LEDS] = {0};



/*
wifi infromation can be set in the wifi.h file
*/

// moved to Wifi.h file
//const char ssid[] = "--------";         /* Replace with your SSID */
//const char passphrase[] = "-------";   /* Replace with your WPA2 passphrase */

// pan variables
int pan_data;
int pan_data1;
int pan_data2;
int pan16b;
int pan_angle;
int pan_angle_last;

// tilt variables
int tilt_data;
int tilt_data1;
int tilt_data2;
int tilt16b;
int tilt_angle;

Servo pan_servo;
Servo tilt_servo;

E131 e131;

int counter = 0;


// includes for self testing
#include <Led_test.h>
#include <Led_ch.h>
void setup() {
    //pinMode(LED_PIN, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(4, OUTPUT);
    
    Serial.begin(115200);
    delay(10);

    /* Choose one to begin listening for E1.31 data */
    //e131.begin(ssid, passphrase);               /* via Unicast on the default port */
    e131.beginMulticast(ssid, passphrase, 1); /* via Multicast for Universe 1 */
    
    
    // servo setup and test
    pan_servo.attach(5);  // nodemcu D1 output
    tilt_servo.attach(4); // nodemcu D2 output

    // servo test fuction
    Servo_test(); // commented out for LED testing
    delay(500);  // commented out for LED testing



    // Led setup and test    
    //FastLED.addLeds<NEOPIXEL, LED_PIN, 6>(g_LEDs, NUM_LEDS);               // Add our LED strip to the FastLED library
    //FastLED.addLeds<WS2812B, 2, GRB>(g_LEDs, NUM_LEDS);
    //FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs,NUM_LEDS);  // this sort of worked

    // neopixel startup
    
    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    pixels.clear();
    // led self test function
    Led_test();
    //pixels.clear();
    //FastLED.show();
    //FastLED.setDither(0);    
    //FastLED.setBrightness(255);  // set default brightness so that you can control it via RGB input vaules
    
}

void loop() {
    
      
     
    /* Parse a packet */
    uint16_t num_channels = e131.parsePacket();
        
    /* Process channel data if we have it, and print it in the serial monitor*/
    if (num_channels) {
        
/*        
        Serial.printf("Num_channels: %u  Packet#: %u  CH1: %u CH2: %u  CH3: %u  CH4: %u LED: CH5: %u  CH6: %u  CH7: %u  CH8: %u  CH9: %u  CH10: %u  CH11: %u  CH12: %u  CH13: %u  CH14: %u  CH15: %u  CH16: %u  CH17: %u  CH18: %u  CH19: %u  CH20: %u  CH21: %u  CH22: %u  CH23: %u  CH24: %u  CH25: %u  CH26: %u :",
                //e131.universe,              // The Universe for this packet
                num_channels,               // Number of channels in this packet
                e131.stats.num_packets,     // Packet counter
                //e131.stats.packet_errors,   // Packet error counter
                e131.data[0],              // pan 1 data for Channel 1 this chanel for 8bit
                e131.data[1],              // pan 2 data for Channel 2 16b Pan
                e131.data[2],              // tilt 1 data for Channel 3 this chanel for 8bit
                e131.data[3],              // tilt 2 data for Channel 4 16bit tilt
                e131.data[4],e131.data[5],e131.data[6],     // LED 1 RGB Data
                e131.data[7],e131.data[8],e131.data[9],     // LED 2 RGB Data
                e131.data[10],e131.data[11],e131.data[12],  // LED 3 RGB Data
                e131.data[13],e131.data[14],e131.data[15],  // LED 4 RGB Data
                e131.data[16],e131.data[17],e131.data[18],  // LED 5 RGB Data
                e131.data[19],e131.data[20],e131.data[21],  // LED 6 RGB Data
                e131.data[22],e131.data[23],e131.data[24],  // LED 7 RGB Data
                e131.data[25]);                             // Function Data
 */                     
        
        //  Led Code call on every other packet
        Serial.printf("Counter: %u", counter);
        
        if (counter == 0) 
        {
                Serial.printf("calling LED Data");
                set_led_data();
                counter++;
        } 
        else if (counter == 1) 
        {
                Serial.printf("Reseting counter");
                counter--;
        }      

                
        //set_led_data();
/*
        int ch = 4;
        
        for (int f = 0; f < NUM_LEDS; f++)
            pixels.setPixelColor(f, pixels.Color(0, 255, 0)),
            //pixels.setPixelColor(f, pixels.Color(e131.data[ch], e131.data[ch + 1], e131.data[ch + 2])), 
            ch = ch + 3;

        pixels.show();
*/        
/*
        //FastLED.clear(false);
        int ch = 4;
        //FastLED.clear(false);
        for (int f = 0; f < NUM_LEDS; f++)
            pixels.setPixelColor(i, pixels.Color(0, 0, 255)), 
            g_LEDs[f].r = e131.data[ch],
            g_LEDs[f].g = e131.data[ch + 1],
            g_LEDs[f].b = e131.data[ch + 2],
            ch = ch + 3;

        FastLED.show();
        delayMicroseconds(20);
*/


// commented out for led testing

        // get Pan/Tilt channel datat
        pan_data1 = e131.data[0];
        pan_data2 = e131.data[1];
        tilt_data1 = e131.data[2];
        tilt_data2 = e131.data[3];
                       
        // 16 bit math 
        pan_data = (pan_data1 *256) + pan_data2;
        tilt_data = (tilt_data1 * 256) + tilt_data2;


        // 8b angle
        //pan_angle = map(pan_data1,0,255,0,180);
        //tilt_angle = map(tilt_data1,0,255,0,180);

        // 16b angle
        pan_angle = map(pan_data,0,65025,550,2330);
        tilt_angle = map(tilt_data,0,65025,550,2330);
        
        if (pan_angle_last != pan_angle) {
                // for 16b fiture
                pan_servo.writeMicroseconds(pan_angle);
               pan_angle_last = pan_angle;
                tilt_servo.writeMicroseconds(tilt_angle);
                
                // for 8 bit fixture
                //pan_servo.write(pan_angle);
                //tilt_servo.write(tilt_angle);
                
               Serial.printf("Pan Data: %u  Pan angle: %u   Tilt Data %u   Tilt angle: %u\n",
                        pan_data,
                        pan_angle,
                        tilt_data,
                        tilt_angle);
                delayMicroseconds(20);
                }
        else
        {
                printf("\n");
        }
        

        
        delay(20);

        
        

     }
}