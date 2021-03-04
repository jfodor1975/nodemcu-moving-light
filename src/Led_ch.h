#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// neo pixel libary code for updateing the led's
void set_led_data()
{
  if (e131.universe == Uni) {  
    //int ch = 4;
        
        for (int f = 0; f < NUM_LEDS; f++){
            int ch = f * 3 + 4;
            //pixels.setPixelColor(f, pixels.Color(0, 255, 0));
            //pixels.setPixelColor(f, pixels.Color(e131.data[ch], e131.data[ch + 1], e131.data[ch + 2])),
            pixels.setPixelColor(f, e131.data[ch], e131.data[ch + 1], e131.data[ch + 2]);
    //        ch = ch + 3;
        }
        pixels.show();
        //delay(20);
  }

}
