String SendHTML(int universe, int address){ //float TemperatureWeb,float HumidityWeb, String TimeWeb,String DateWeb){
  String ptr = "<!DOCTYPE html> <html>\n";
    ptr += "<html><body onload='document.refresh()'><center>\n";
    ptr += "<h1>WiFi Desk Light</h1><br>\n";
    ptr += "<p>Current Universe: ";
    ptr += (int)universe;
    ptr += "<p>";
    ptr += "<p>Current Address: ";
    ptr += (int)address;
    ptr += "<p>";
    ptr += "<a>Rua a </a>";
    ptr += "<a href='Testmode'>Led Test</a><br>";
    ptr += "<a>Run a </a>";
    ptr += "<a href='PanTiltreset'>Pan/Tilt Reset</a><br>";
    ptr += "<hr> ";
    ptr += "<form action='/' method='POST'>";
    ptr += "SACN Universe #: <input type='text' name='Universe' value=";
    ptr += (int)universe;
    ptr += ">";
    ptr += "<input type='submit' value='Submit'>";
    ptr += "</form><br>";
    ptr += "<form action='/' method='POST'>";
    ptr += "Starting Address #: <input type='text' name='Address' value=";
    ptr += (int)address;
    ptr += ">";
    ptr += "<input type='submit' value='Submit'>";
    ptr += "<br><br></center></body></html>\n";
  return ptr;
}

//ptr += " ";

String Confirm_Changes_HTML(int universe, int address){ //float TemperatureWeb,float HumidityWeb, String TimeWeb,String DateWeb){
  String ptr = "<!DOCTYPE html> <html>\n";
    ptr += "<html><body><center>\n";
    ptr += "<h1>WiFi Desk Light</h1><br>\n";
    ptr += "<h1>Setings Confirmed</h1><br>\n";
    ptr += "<h1>Rebooting</h1><br>\n";
    ptr += "<p>Current Universe: ";
    ptr += (int)universe;
    ptr += "<p>";
    ptr += "<p>Current Address: ";
    ptr += (int)address;
    ptr += "<hr>";
    ptr += "<input type='button' value='Go Back' onclick='history.back(-1)' />";
    ptr += "<br><br></center></body></html>\n";
  return ptr;
}


const char PanTilt_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>WiFi Desk Light</h1><br>
Reseting the Pan and Tilt<br>
this will move thru a full range of motion on the Pan and the tilt axis<br>
<hr>
<input type="button" value="Go Back" onclick="history.back(-1)" />
</center>

</body>
</html>
)=====";

const char Ledreset_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>WiFi Desk Light</h1><br>
Reseting the LED's<br>
You should see an ramp of RGB thru all 7 leds<br>
SACN will resme control when complete<br>
<hr>
<input type="button" value="Go Back" onclick="history.back(-1)" />
</center>

</body>
</html>
)=====";