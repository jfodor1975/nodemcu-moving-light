#include <Arduino.h>
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ALLOW_INTERRUPTS 0
//#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

//Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void Led_test()
{

        //FastLED.setBrightness(255);
        // Cycle all thru red    
        for (int i = 0; i < NUM_LEDS; i++)

                
                pixels.setPixelColor(i, pixels.Color(255, 0, 0)),
                //g_LEDs[i].r = 255,
                //g_LEDs[i].g = 0,
                //g_LEDs[i].b = 0,
                //FastLED.show(),
                pixels.show(),
                delay(150);
        
        //FastLED.show();
        delay(250);
        pixels.clear();
        pixels.show();


        // Cycle all thru Green
        for (int i = 0; i < NUM_LEDS; i++)
                pixels.setPixelColor(i, pixels.Color(0, 255, 0)),
                //g_LEDs[i].r = 0,
                //g_LEDs[i].g = 255,
                //g_LEDs[i].b = 0,
                //FastLED.show(),
                pixels.show(),
                delay(150);
        
        //FastLED.show();
        delay(250);
        pixels.clear();
        pixels.show();

        for (int i = 0; i < NUM_LEDS; i++)

                pixels.setPixelColor(i, pixels.Color(0, 0, 255)),
                //g_LEDs[i].r = 0,
                //g_LEDs[i].g = 0,
                //g_LEDs[i].b = 255,
                //FastLED.show(),
                pixels.show(),
                delay(150);
        
        //FastLED.show();
        
        delay(250);
        pixels.clear();
        pixels.show();
        //for (int i = 0; i < NUM_LEDS; i++)
        //    g_LEDs[i] = CRGB::Black;
        
        
        
        //FastLED.show();
        //delay(1000);
}

void Servo_test()
{
    
    pan_servo.write(90);
    tilt_servo.write(90);
    delay(500);
    pan_servo.write(0);
    tilt_servo.write(0);
    delay(500);
    pan_servo.write(180);
    tilt_servo.write(180);
    delay(500);
    pan_servo.write(90);
    tilt_servo.write(90);


}