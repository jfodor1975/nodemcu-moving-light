#include <Arduino.h>
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

void Led_test()
{

        FastLED.setBrightness(255);
    
        for (int i = 0; i < NUM_LEDS; i++)
                g_LEDs[i].r = 255,
                g_LEDs[i].g = 0,
                g_LEDs[i].b = 0,
                FastLED.show(),
                delay(250);
        
        FastLED.show();
        delay(500);

        for (int i = 0; i < NUM_LEDS; i++)
                g_LEDs[i].r = 0,
                g_LEDs[i].g = 255,
                g_LEDs[i].b = 0,
                FastLED.show(),
                delay(250);
        
        FastLED.show();
        delay(500);

        for (int i = 0; i < NUM_LEDS; i++)
                g_LEDs[i].r = 0,
                g_LEDs[i].g = 0,
                g_LEDs[i].b = 255,
                FastLED.show(),
                delay(250);
        
        FastLED.show();
        delay(500);

        for (int i = 0; i < NUM_LEDS; i++)
            g_LEDs[i] = CRGB::Black;
        
        
        
        FastLED.show();
        delay(1000);
}

void Servo_test()
{
    
    pan_servo.write(90);
    tilt_servo.write(90);
    delay(1000);
    pan_servo.write(0);
    tilt_servo.write(0);
    delay(1000);
    pan_servo.write(180);
    tilt_servo.write(180);
    delay(1000);
    pan_servo.write(90);
    tilt_servo.write(90);


}