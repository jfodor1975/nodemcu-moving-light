#include <Arduino.h>
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>


void set_led_data()
{

int ch = 4;
//FastLED.clear(false);
for (int f = 0; f < NUM_LEDS; f++)
             
    g_LEDs[f].r = e131.data[ch],
    g_LEDs[f].g = e131.data[ch + 1],
    g_LEDs[f].b = e131.data[ch + 2],
    ch = ch + 3;

//FastLED.show();
}