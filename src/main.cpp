/*
 Moving light code
 version 0.0.3
 

Node MCU pins

D1 = pan servo signal
D2 = tilt servo signal
D6 = WS2812B data

7 pixels led board.

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
#include <Wifi.h> // wifi information 

// wifimanager includes
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 


// led defines
#define LED_PIN     D6
#define COLOR_ORDER GRB
#define CHIPSET     WS2812b
#define NUM_LEDS    7
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ400);


/*
wifi infromation can be set in the wifi.h file
*/
WiFiServer server(80);
String header;


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

// HTML, move to file later
ESP8266WebServer WebServer(80);

// my includes for sanity
#include <Led_test.h>
#include <Led_ch.h>
#include <index.h>

// HTML request handeling
void handleTestmode() { 
 Serial.println("LED Test Called");
 WebServer.send(200, "text/html", Ledreset_page); //Send ADC value only to client ajax request 
 pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
 pixels.clear(); // led self test function
 Led_test();
 
}

void handlePanTiltreset() { 
 Serial.println("Reseting light");
 WebServer.send(200, "text/html", PanTilt_page); //Send ADC value only to client ajax request
 Servo_test(); // commented out for LED testing
}


void handleRoot() {
  WebServer.send (200, "text/html", MAIN_page);
}

void handleNotFound(){
  WebServer.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}



void setup() {
    pinMode(5, OUTPUT);
    pinMode(4, OUTPUT);
    
    Serial.begin(115200);
    delay(10);

    /* Choose one to begin listening for E1.31 data */
    //e131.begin(ssid, passphrase);               /* via Unicast on the default port */
    //e131.beginMulticast(ssid, passphrase, 1); /* via Multicast for Universe 1 */
    
    // WiFiManager
   // Local intialization. Once its business is done, there is no need to keep it around
   WiFiManager wifiManager;
  
   // Uncomment and run it once, if you want to erase all the stored information 
   //wifiManager.resetSettings();
  
   // set custom ip for portal
   //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

   // fetches ssid and pass from eeprom and tries to connect
   // if it does not connect it starts an access point with the specified name
   // here  "AutoConnectAP"
   // and goes into a blocking loop awaiting configuration
   wifiManager.autoConnect("Desk_Mover_AP");
   // or use this for auto generated name ESP + ChipID
   //wifiManager.autoConnect();
  
   // if you get here you have connected to the WiFi
   Serial.println("Connected.");
   Serial.println(WiFi.localIP());    


    
    // servo setup and test
    pan_servo.attach(5);  // nodemcu D1 output
    tilt_servo.attach(4); // nodemcu D2 output

    // servo test fuction
    Servo_test(); // commented out for LED testing
    delay(500);  // commented out for LED testing


    // neopixel startup
    
    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    pixels.clear();
    // led self test function
    Led_test();
    
    e131.begin(E131_MULTICAST,1);


    // start webserver    
    
    
    WebServer.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
    WebServer.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
    WebServer.on("/Testmode", handleTestmode);   // call the test mode function
    WebServer.on("/PanTiltreset", handlePanTiltreset); // call the reset function
    WebServer.on("/Mainpage", handlePanTiltreset); // goto the main page

    WebServer.begin();                           // Actually start the server
    Serial.println("HTTP server started");

}




void loop() {
    
    // web server fuctions
    WebServer.handleClient();
      
     
    /* Parse a E131 packet */
    uint16_t num_channels = e131.parsePacket();
        
    /* Process channel data if we have it, and print it in the serial monitor*/
    if (num_channels) {
        
/*      // debug information via the serial console

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
                //Serial.printf("calling LED Data");  // for debuging
                set_led_data();  // calling LED fuction to set the led colours per pixel
                counter++;
        } 
        else if (counter == 1) 
        {
                //Serial.printf("Reseting counter");  // for debuging
                counter--;
        }      

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