#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

//Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void set_led_data()
{
int ch = 4;
        
        for (int f = 0; f < NUM_LEDS; f++)
            //pixels.setPixelColor(f, pixels.Color(0, 255, 0));
            pixels.setPixelColor(f, pixels.Color(e131.data[ch], e131.data[ch + 1], e131.data[ch + 2])), 
            ch = ch + 3;

        pixels.show();


}
