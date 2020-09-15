#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid  = "n011";
const char* password = "1234567890";

ESP8266WebServer server(80);

const int led = D1;
bool ledState;

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for(uint8_t i=0; i<server.args(); i++){
  message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain" , message);
}

void handleRoot()
{
  server.send(200, "text/plain" , "hello from esp8266!");
}

void handleLed()
{
  if (server.args() == 1 && server.argName(0) == "light")
  {
  if (server.arg(0) == "0")
  {
    digitalWrite(led,HIGH);
    ledState = false;
  }
  else
  {
    digitalWrite(led,LOW);
    ledState = true;
  }
  }
  String html =
  "<html>\n"
  " <hed><title>IoT</title></hed>\n"
  " <meta http-equiv=refresh content=10>\n"
  " <body>\n"
  "   <form action=\"led.php\" method=\"post\">\n";
  if (ledState)
  html +=
  " <button type=submit name=light value=0>Turn Off</button></td>\n";
  else
  html +=
  " <button type=submit name=light value=1>Turn On</button></td>\n"; 
  html +=
  "   </form>\n"
  " </body>\n"
  "</html>\n";
  server.send(200 , "text/html" , html); 
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid,password);
  Serial.println();
  Serial.print("ssid = ");
  Serial.print(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/",handleRoot);
  server.on("/led.php" , handleLed);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
