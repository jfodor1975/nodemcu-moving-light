#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

//Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void Led_test()
{

        
        // Cycle all thru red    
        for (int i = 0; i < NUM_LEDS; i++)
              
                pixels.setPixelColor(i, pixels.Color(255, 0, 0)),
                pixels.show(),
                delay(150);
        
        delay(250);
        pixels.clear();
        pixels.show();


        // Cycle all thru Green
        for (int i = 0; i < NUM_LEDS; i++)
                pixels.setPixelColor(i, pixels.Color(0, 255, 0)),
                pixels.show(),
                delay(150);
        
        delay(250);
        pixels.clear();
        pixels.show();

        for (int i = 0; i < NUM_LEDS; i++)

                pixels.setPixelColor(i, pixels.Color(0, 0, 255)),
                pixels.show(),
                delay(150);
        
        delay(250);
        pixels.clear(); // clear the pixels, Blackout
        pixels.show();
 
}

void Servo_test()
{
    
    pan_servo.write(90);
    tilt_servo.write(90);
    delay(750);
    pan_servo.write(0);
    tilt_servo.write(0);
    delay(750);
    pan_servo.write(180);
    tilt_servo.write(180);
    delay(750);
    pan_servo.write(90);
    tilt_servo.write(90);


}