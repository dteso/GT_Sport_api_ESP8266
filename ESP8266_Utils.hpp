/********************************WifI connections methods ******************************************/
void ConnectWiFi_STA(bool useStaticIP = false)
{
  Serial.println("");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (useStaticIP) WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print('.');
  }

  Serial.println("");
  Serial.print("Iniciado STA:\t");
  Serial.println(ssid);
  Serial.print("Conectado a RED Local con IP:\t");
  Serial.println(WiFi.localIP());
  delay(1000);
}

void ConnectWiFi_AP(bool useStaticIP = false)
{
  Serial.println("");
  WiFi.mode(WIFI_AP);
  while (!WiFi.softAP(ssid, password))
  {
    Serial.println(".");
    delay(100);
  }
  if (useStaticIP) WiFi.softAPConfig(ip, gateway, subnet);

  Serial.println("");
  Serial.print("Iniciado AP:\t");
  Serial.println(ssid);
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());
}
/************************************************************************************************/

/***************************************** Web Server  ***********************************************/
void renderWebServer() {
  String header;
  WiFiClient client = webServer.available();
  if (client) { // If a new client connects,
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected()) { // loop while the client's connected
      if (client.available()) { // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c); // print it out the serial monitor
        header += c;
        if (c == '\n') { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:

          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            int pos1 = header.indexOf("user_no=");
            int pos2 = header.indexOf("$_314");
            if (pos1 >= 0) {
              String USER = header.substring(pos1 + 9, pos2);
              Serial.println ("USER" + USER );
              user_no = header.substring(pos1 + 8, pos2).toInt();
              digitalWrite(D4, HIGH);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");


            client.println("<link rel=\"icon\" href=\"data:,\">");
            //Bootstrap 5.0.0
            client.println("<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/5.0.0-alpha1/css/bootstrap.min.css' integrity='sha384-r4NyP46KrjDleawBgD5tp8Y7UzmLA05oM1iAEQ17CSuDqnUK2+k9luXQOfXJCJ4I' crossorigin='anonymous'>");
            client.println("<script src='https://stackpath.bootstrapcdn.com/bootstrap/5.0.0-alpha1/js/bootstrap.min.js' integrity='sha384-oesi62hOLfzrys4LxRF63OJCXdXDipiYWBnvTl9Y9/TRlw5xlKIEHpNyvvDShgf/' crossorigin='anonymous'></script>");
            client.println("<link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css' integrity='sha384-wvfXpqpZZVQGK6TAh5PVlGOfQNHSoD2xbE+QkPxCAFlNEevoEH3Sl0sibVcOQVnN' crossorigin='anonymous'>");
            // CSS <styles>
            client.println("<style>html{font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} .bckg_decaration{background: no-repeat center/100% url(https://www.gran-turismo.com/images/c/i1HEdtA8EzVRNhH.jpg);} .text-center{ text-align: center; justify-content: center !important} .w-100{width: 100%;}");
            client.println("</style></head>");
            // Web Page Heading
            client.println("<body class='row text-center'><div class='card col-lg-4 col-md-4 mt-5'><div class='card-header' style='background-color: black; color: lightgrey;'><h1>GT Sport API</h1></div><div class='card-body'>");
            client.println("<nav class='navbar navbar-expand-lg navbar-dark bg-dark mb-3'>");
            client.println("<div class='navbar-collapse row' id='navbarSupportedContent'><ul class='navbar-nav mr-auto'>");
            client.println("<li class='nav-item'>");
            client.println("<a class='nav-link' style='color: white; font-size: 20px;'><strong class='text-center'>" + _PROFILE + "</strong></a></li>");
            client.println("<li class='nav-item'>");
            client.println("<a class='nav-link'>Points <strong>" + (String)_DRIVER_POINTS + "</strong></a></li>");
            client.println("<li class='nav-item'>");
            client.println("<a class='nav-link'>Rank <strong>" + _DRIVER_CLASS + "</strong></a></li>");
            client.println("<li class='nav-item'>");
            client.println("<a class='nav-link'>Last Race: " +  (String)(_DRIVER_POINTS - lastScore) + "</a></li>");
            client.println("</ul></div></nav>");            
            client.println("<div class='row'><div class='col-lg-8 col-md-8 col-8'><input type='text'class='form-control' id='inputUser' placeholder='Insert user number...'></input></div>");
            client.println("<div class='col-lg-4 col-md-4 col-4'><button type='button' class='btn btn-dark w-100' id='submitBtn' onclick='getTextValue()'><i class='fa fa-arrow-right'></i></button></div></div>");
            client.println("<label for='user_lbl' id='lblUser' class='mt-2'>GT USER NUMBER</label>");
            client.println("<h3 id='user_lbl'>"+ (String)user_no +"</h3>");
            client.println("<a href='http://www.gran-turismo.com'>Which is my user number?</a><br>");
            client.println("<button class='btn btn-dark mt-3' style='color: white;' onclick=location.href='https://dteso.github.io/GT_Sport_api_ESP8266/'>See more <i class='fa fa-github'></i></button>");
            client.println("</div></body>");
            client.println("<script> getTextValue = () => {let input = document.getElementById('inputUser').value; console.log(input); window.location='./user_no='+input+'$_314';}</script>");//$_314 es un código establecido voluntariamente pra detectar el fin de cadena pero podría ser otro cualquiera
            client.println("</html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
/************************************************************************************************/


/***************************************** NTP ***********************************************/
void ntp(){
    timeClient.update(); //sincronizamos con el server NTP
    realHour = timeClient.getFormattedTime().substring(0,2).toInt() + 5; //+5 para que coja la hora de Madrid
    realMinute = timeClient.getFormattedTime().substring(3,5).toInt();
    realSec = timeClient.getFormattedTime().substring(6,8).toInt();
    if(realSec==0){
      Serial.print("IP address:\t");
      Serial.println(WiFi.localIP());
      Serial.print(realHour);Serial.print(":");
        if(realMinute<10){
          Serial.print('0');Serial.println(realMinute);
        }else{
          Serial.println(realMinute);
        }
    delay(1000);
    }
    delay(50);
}
/************************************************************************************************/



String timeLeft (int currentHor,int currentMin, int currentSecond, int nextHor, int nextMin ){
  int nextSecond = 0;
  int minutesLeft = 0;
  int secondsLeft = 0;
  String ml = "";
  String sl = "";
  String timeLeft = "";
  if(nextMin > currentMin){
    minutesLeft = nextMin - (currentMin + 1);          
  }else{
    minutesLeft = (60 - (currentMin+1)) + nextMin;
  }
  secondsLeft = 60 - currentSecond;
  if(secondsLeft<10){
    sl = "0"+(String)secondsLeft;
  }else{
    sl = (String)secondsLeft;
  }
  if(minutesLeft<10){
    ml= "0"+(String)minutesLeft;
  }else{
    ml = (String)minutesLeft;
  }
  timeLeft = ml+ ":" +sl;
  if(currentMin == nextMin){
    timeLeft = "..WARM";
  }
  Serial.println ("Tiempo Restante: " + timeLeft);
  return timeLeft;
}
