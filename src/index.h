String SendHTML(int universe, int address){ //float TemperatureWeb,float HumidityWeb, String TimeWeb,String DateWeb){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<html><body><center>\n";
  ptr += "<h1>WiFi Desk Light</h1><br>\n";
  //ptr += "<h1>Run a </h1>";
  ptr += "<p>Current Universe: ";
  ptr += (int)universe;
  ptr += "<p>";
  ptr += "<p>Current Address: ";
  ptr += (int)address;
  ptr += "<p>";
  ptr += "<a>Rua a </a>";
  ptr += "<a href='Testmode'>Led Test</a><br>";
  ptr += "<a>Rua a </a>";
  ptr += "<a href='PanTiltreset'>Pan/Tilt Reset</a><br>";
  ptr += "<hr> ";

  

  //ptr += "<a href="Testmode">Led Test</a>";
  ptr += "<br><br>\n";
  //ptr += "<h1>Run a <a href="PanTiltreset">Pan/Tilt Reset</a><br>\n";
 
 
  return ptr;
}

//ptr += " ";


/*
const char MAIN_page[] PROGMEM = R"=====(
 <!DOCTYPE html>
<html>
<body>
<center>
<h1>WiFi Desk Light</h1><br>
<h1>Run a </h1><a href="Testmode">Led Test</a><br><br>
Run a <a href="PanTiltreset">Pan/Tilt Reset</a><br>
<br><br>
<p>Current Universe : 
Clients_Response1
</p>
<hr>
<form action="/" method="POST">
SACN Universe #: <input type="text" name="Universe">
<input type="submit" value="Submit">
</form><br>
<form action="/" method="POST">
Starting Address #: <input type="text" name="Address">
<input type="submit" value="Submit">
</form><br>
</center>
</body>
</html>
//)=====";
*/

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