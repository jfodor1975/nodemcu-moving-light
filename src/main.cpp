/*
* ESP8266_Test.ino - Simple sketch to listen for E1.31 data on an ESP8266 
*                    and print some statistics.
*
* Project: E131 - E.131 (sACN) library for Arduino
* Copyright (c) 2015 Shelby Merrick
* http://www.forkineye.com
*
*  This program is provided free for you to use in any way that you wish,
*  subject to the laws and regulations where you are using it.  Due diligence
*  is strongly suggested before using this code.  Please give credit where due.
*
*  The Author makes no warranty of any kind, express or implied, with regard
*  to this program or the documentation contained in this document.  The
*  Author shall not be liable in any event for incidental or consequential
*  damages in connection with, or arising out of, the furnishing, performance
*  or use of these programs.
*
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <E131.h>
#include <Servo.h>

const char ssid[] = "-------";         /* Replace with your SSID */
const char passphrase[] = "-------";   /* Replace with your WPA2 passphrase */

// pan variables
int pan_data;
int pan_data1;
int pan_data2;
int pan116b;
int pan_angle;

// tilt variables
int tilt_data;
int tilt_data1;
int tilt_data2;
int tilt16b;
int tilt_angle;

Servo pan_servo;
Servo tilt_servo;

E131 e131;

void setup() {
    Serial.begin(115200);
    delay(10);

    /* Choose one to begin listening for E1.31 data */
    //e131.begin(ssid, passphrase);               /* via Unicast on the default port */
    e131.beginMulticast(ssid, passphrase, 1); /* via Multicast for Universe 1 */
    

    pan_servo.attach(5);
    pan_servo.write(90);
    tilt_servo.attach(4);
    tilt_servo.write(90);

}

void loop() {
    /* Parse a packet */
    uint16_t num_channels = e131.parsePacket();
    
    /* Process channel data if we have it, and print it in the serial monitor*/
    if (num_channels) {
        Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH1: %u CH2: %u  CH3: %u  CH4: %u  CH5: %u\n",
                e131.universe,              // The Universe for this packet
                num_channels,               // Number of channels in this packet
                e131.stats.num_packets,     // Packet counter
                e131.stats.packet_errors,   // Packet error counter
                e131.data[0],              // pan 1 data for Channel 1 this chanel for 8bit
                e131.data[1],              // pan 2 data for Channel 2
                e131.data[2],              // tilt 1 data for Channel 3 this chanel for 8bit
                e131.data[3],              // tilt 2 data for Channel 4
                e131.data[4]);              // intenisty data for Channel 5

        // get channel datat
        pan_data1 = e131.data[0];
        pan_data2 = e131.data[1];
        tilt_data1 = e131.data[2];
        tilt_data2 = e131.data[3];
                       
        // 16 bit math
        pan_data = pan_data1 * pan_data2;
        tilt_data = tilt_data1 * tilt_data2;

        // 8b angle
        //pan_angle = map(pan_data1,0,255,0,180);
        //tilt_angle = map(tilt_data1,0,255,0,180);

        // 16b angle
        pan_angle = map(pan_data,0,65025,700,2300);
        tilt_angle = map(tilt_data,0,65025,700,2300);
        
        // for 16b fiture
        pan_servo.writeMicroseconds(pan_angle);
        tilt_servo.writeMicroseconds(tilt_angle);
        
        // for 8 bit fixture
        //pan_servo.write(pan_angle);
        //tilt_servo.write(tilt_angle);
        
        Serial.printf("Pan Data: %u  Pan angle: %u   Tilt Data %u   Tilt angle: %u\n",
                pan_data,
                pan_angle,
                tilt_data,
                tilt_angle);

        }
}