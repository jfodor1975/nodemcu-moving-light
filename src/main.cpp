#include <FastLED.h>  // include FastLED *before* Artnet
#include <ESP32Servo.h>
#include <ArtnetWiFi.h>
#include <WiFiManager.h>
#include <LittleFS.h>
#include <FS.h>
#include <ArduinoJson.h>

/*
ESP32 C3 super mini board

D7 = pan servo signal
D6 = tilt servo signal
D2 = WS2812B data

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
*/


/////////////////////////////////////////////////////
// ensure we format the file sytem?
#define FORMAT_LITTLEFS_IF_FAILED true
/////////////////////////////////////////////////////


// can proably remover this once wifi managere works
// WiFi stuff
bool TEST_CP         = true; // always start the configportal, even if ap found
int  TESP_CP_TIMEOUT = 30; // test cp timeout


ArtnetWiFiReceiver artnet;
uint16_t Fixture_universe = 1; // 0 - 32767
uint8_t net = 0;        // 0 - 127
uint8_t subnet = 0;     // 0 - 15
uint16_t Fixture_address = 1;
  // HTML converstion and display 
  char convertedUniverse[6];
  char convertedAddresss[3];


//
// LED stuff
#define NUM_LEDS  7
// #define LED_PIN   2 // LED D2 pin 
const uint8_t PIN_LED_DATA = 2;
CRGB leds[NUM_LEDS];
//////////////////////////////////////////


//
// Servo Stuff
Servo Pan_servo;
Servo Tilt_servo;
uint8_t Pan_Pos = 90;
uint8_t Tilt_Pos = 90;

int pan_data1;
int pan_data2;
int pan_16gb;
int pan_angle;
int pan_angle_old;

int tilt_data1;
int tilt_data2;
int tilt_16b;
int tilt_angle;
int tilt_angle_old;



int minUs = 600;
int maxUs = 2350;

int Pan_Pin = 7; // Servo pin D7
int Tilt_Pin = 6; // Servo pin D6

int pos = 90;      // position in degrees
ESP32PWM pwm;


// file properties and usage
// config file
#define Json_config_file "/Fixture_config.json"
// add format line here
bool remove_Config = false;
//bool remove_Config = true;

// should save the config for testing?
//bool shouldSaveConfig = false;
bool shouldSaveConfig = true;





void callback(const uint8_t *data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
    // you can also use pre-defined callbacks
}

bool saveConfig() {
  JsonDocument doc;
  doc["Fixture_Universe"] = Fixture_universe;
  doc["Fixture_Address"] = Fixture_address;

  File configFile = LittleFS.open(Json_config_file, "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  // saves the document here
  serializeJson(doc, configFile);
  
  Serial.print("Saved Fixture Universe: ");
  Serial.println(Fixture_universe);
  Serial.print("Saved Fixture Address: ");
  Serial.println(Fixture_address);
  
  return true;
}


bool loadConfig() {
  File configFile = LittleFS.open(Json_config_file, "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  // this is where it reads the json document
  JsonDocument doc;
  auto error = deserializeJson(doc, configFile);
  if (error) {
    Serial.println("Failed to parse config file");
    return false;
  }
  
  Fixture_universe = doc["Fixture_Universe"].as<int>();
  Fixture_address = doc["Fixture_Address"].as<int>();
//  const char* serverName = doc["serverName"];
//  const char* accessToken = doc["accessToken"];

  // Real world application would store these values in some variables for
  // later use.

  Serial.print("Loaded Fixture Universe: "); Serial.println(Fixture_universe);
  Serial.print("Loaded Fixture Universe Json: "); Serial.println(doc["Fixture_Universe"].as<String>());
  Serial.print("Loaded Fixture Address: "); Serial.println(Fixture_address);
  Serial.print("Loaded Fixture Address Json: "); Serial.println(doc["Fixture_Address"].as<String>());
  return true;
}

void saveConfigCallback()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  
  FastLED.addLeds<WS2812B, PIN_LED_DATA,GRB>(leds, NUM_LEDS);

  Serial.begin(115200);
  delay(2000);

  if (!LittleFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  
  // load the config file if it exsits.
    if (!loadConfig()) {
    Serial.println("Failed to load config");
  } else {
    Serial.println("Config loaded");
  }

// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
	
	Pan_servo.setPeriodHertz(50);      // Standard 50hz servo
	Tilt_servo.setPeriodHertz(50);      // Standard 50hz servo
      
// WiFi stuff
WiFiManager wm;

wm.setConfigPortalTimeout(60);
wm.setAPClientCheck(true); // avoid timeout if client connected to softap
wm.setMinimumSignalQuality(40);  // set min RSSI (percentage) to show in scans, null = 8%
wm.setParamsPage(true);
wm.setDarkMode(true);
wm.setTitle("Desk Lamp");

// uncomment for testing new configs, and wanting to resest the saved settings 
//wm.resetSettings();



// HTML code to display custom vaules and settings for the fixutre
  sprintf(convertedUniverse, "%d", Fixture_universe); // Need to convert to string to display a default value.
  sprintf(convertedAddresss, "%d", Fixture_address);


  WiFiManagerParameter custom_universe("HTML_uni", "Universe", convertedUniverse, 3); // 7 == max length
  WiFiManagerParameter custom_adderess("HTML_add", "Address", convertedAddresss, 3); // 7 == max length

wm.addParameter(&custom_universe);
wm.addParameter(&custom_adderess);



  if(!wm.autoConnect("Desk_LED","desk1234")) {
    Serial.println("failed to connect and hit timeout");
  }
  else if(TEST_CP) {
    // start configportal always
    delay(1000);
    Serial.println("TEST_CP ENABLED");
    wm.setConfigPortalTimeout(TESP_CP_TIMEOUT);
    wm.startConfigPortal("Desk_LED_config","12345678");
  }
  else {
    //if you get here you have connected to the WiFi
     Serial.println("connected...yeey :)");
  }

  Serial.println("i'm passed the portal opening");


  // ensure we write univers and address to be the current vaules.
  Fixture_universe = atoi(custom_universe.getValue());
  Fixture_address = atoi(custom_adderess.getValue());

/*
  if (!wm.autoConnect()) {
    saveConfig();
    Serial.println("Saving; hit timeout");
  }
*/
  if (shouldSaveConfig)
  {
    Serial.println("Calling the save function now");
    saveConfig();
  }
/*

bool res;
 res = wm.autoConnect("Desk_LED","desk1234"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");

        universe1 = atoi(custom_universe.getValue());
        address = atoi(custom_adderess.getValue());

        Serial.print("Universe: "); Serial.print(universe1); Serial.println();
        Serial.print("Address: "); Serial.print(address); Serial.println();
        if (shouldSaveConfig)
        {
            saveConfigFile();
        }
    }
*/

// copy universe and address so they can be used.


    artnet.begin();

    // if Artnet packet comes to this universe, forward them to fastled directly
    //artnet.forwardArtDmxDataToFastLED(universe1, leds, NUM_LEDS);

    // this can be achieved manually as follows
    // if Artnet packet comes to this universe, this function (lambda) is called
     artnet.subscribeArtDmxUniverse(Fixture_universe, [&](const uint8_t* data, uint16_t size, const ArtDmxMetadata &metadata, const ArtNetRemoteInfo &remote) {
    //     // set led
    //     // artnet data size per packet is 512 max
    //     // so there is max 170 pixel per packet (per universe)
         for (size_t pixel = 0; pixel < NUM_LEDS; ++pixel) {
             size_t idx = pixel * 3 + 4;
             leds[pixel].r = data[idx + 0];
             leds[pixel].g = data[idx + 1];
             leds[pixel].b = data[idx + 2];
             Serial.print ("Pixel = "); Serial.print(pixel);
             Serial.print (" Red = ");Serial.print(data[idx +0]);
             Serial.print (" Green = ");Serial.print(data[idx + 1]); 
             Serial.print (" Blue = ");Serial.print(data[idx + 2]);
             Serial.println();
         }
//          pan_angle = map(data[3],0,255,minUs,maxUs);
//          tilt_angle = map(data[4],0,255,minUs,maxUs);

        pan_data1 = data[0];
        pan_data2 = data[1];
        tilt_data1 = data[2];
        tilt_data2 = data[3];
        
        pan_16gb = (pan_data1 * 256) + pan_data2;
        tilt_16b = (tilt_data1 * 256) + tilt_data2;
        pan_angle = map(pan_16gb,0,65025,minUs,maxUs);
        tilt_angle = map(tilt_16b,0,65025,minUs,maxUs);
        


//         Serial.print("Data 1= "); Serial.print(pan_data1);
//         Serial.print (" Data 2 = "); Serial.print(pan_data2);
//         Serial.print (" 16b value = "); Serial.print(pan_16gb);
//         Serial.print (" Pan Angle = "); Serial.print(pan_angle);
//         Serial.print (" Tilt Angle = "); Serial.print(tilt_angle);
         
        if (pan_angle != pan_angle_old or tilt_angle != tilt_angle_old) {
          Pan_servo.writeMicroseconds(pan_angle);
          Tilt_servo.writeMicroseconds(tilt_angle); 
          pan_angle_old = pan_angle;
          tilt_angle_old = tilt_angle;
          Serial.print (" Pan Angle = "); Serial.print(pan_angle);
          Serial.print (" Tilt Angle = "); Serial.print(tilt_angle);
          Serial.println();
          //Serial.print (" Pan Angle = Update ");
          }  
        
//        if (tilt_angle != tilt_angle_old) {
//         Tilt_servo.writeMicroseconds(tilt_angle); 
//         Serial.print (" Tilt Angle = Update ");
//         } 
        
        
//        Serial.println();

     });

  Pan_servo.attach(Pan_Pin, minUs, maxUs);
	Tilt_servo.attach(Tilt_Pin, minUs, maxUs);
#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32C3)
	pwm.attachPin(21, 10000);//10khz
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
	pwm.attachPin(7, 10000);//10khz
#else
	pwm.attachPin(27, 10000);//10khz
#endif

/// =====  Basic FastLED code
//  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
//  FastLED.setBrightness(50);

}




void loop() {
 
  artnet.parse();  // check if artnet packet has come and execute callback
  FastLED.show();

}





























