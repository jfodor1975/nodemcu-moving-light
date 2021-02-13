#include <Arduino.h>
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ALLOW_INTERRUPTS 0
//#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

//#define ch 0

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
/*


int ch = 4;
//FastLED.clear(false);
for (int f = 0; f < NUM_LEDS; f++)
             
    g_LEDs[f].r = e131.data[ch],
    g_LEDs[f].g = e131.data[ch + 1],
    g_LEDs[f].b = e131.data[ch + 2],
    ch = ch + 3;

//FastLED.show();
}

*/