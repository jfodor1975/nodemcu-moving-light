const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>WiFi Desk Light</h1><br>
Ciclk to turn <a href="Testmode">Led Test</a><br>
Ciclk to turn <a href="PanTiltreset">Pan/Tilt Reset</a><br>
<hr>
<a href="https://circuits4you.com">circuits4you.com</a>
</center>

</body>
</html>
)=====";

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