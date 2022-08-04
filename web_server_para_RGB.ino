/*En este script se hara un servidor
   web para poder controlar una luces leds
   mediante una esp8266

   aunque las pruebas sercan con esp32
*/

//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiAP.h> //comment this line if using Client Mode
#include <Adafruit_NeoPixel.h>

int p=25;
volatile bool estado = false ; 
volatile uint8_t cont=0;
volatile uint8_t ti=0;
volatile uint8_t tf=0;
volatile uint8_t total=0;
volatile uint8_t totali=0;
const char *ssid = "Conexion_ETB";
const char* password = "Colombia2012";
//const char* password = "";    uncomment this line if you 
                                        //use access point mode and you 
                                        //want a password for it 

WebServer server(80);
//ESP8266WebServer server(80);
Adafruit_NeoPixel pixels(7 , 12, NEO_GRB + NEO_KHZ800);

void IRAM_ATTR isr(){
  if(estado==false)
  {estado=true;  
  tf=millis()/1000;
  Serial.println(tf);

 
  }else{
  estado=false;  
  ti=millis()/1000;
  Serial.println(ti);

    }
  
  
  }
void setup() {
  
  pixels.begin();
  pinMode(p, INPUT);
  attachInterrupt(digitalPinToInterrupt(p),isr,CHANGE);
 //attachInterrupt(digitalPinToInterrupt(p),isr2,FALLING);
  
  pinMode(2, OUTPUT);
  pinMode(14, OUTPUT);
  
  Serial.begin(115200);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); 
 /* 

    UNCOMMENT THIS if using AP
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
*/
  server.on("/", handleConnectionRoot);
  server.on("/on", ON);
  server.on("/off", OFF);
  server.on("/Reset", Reset);
  server.onNotFound(handle_NotFound);
  server.begin();
  pixels.setBrightness(10);

}

void loop() {
  
  server.handleClient();
 // Serial.println(millis()/1000);
  //Serial.println("---------------------------------");
  //Serial.print((total)+"TOTAL");
  //Serial.print("---------------------------------");
  //Serial.println((totali)+"TOTALIIII");
}


String device = "";
String answer = "";
String answer2 = "";

void handleConnectionRoot() {
  setAnswer();
  server.send(200, "text/html", answer);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}


void ON() {
  setAnswer();
  
  digitalWrite(2, HIGH);
  digitalWrite(14, HIGH);
    
  server.send(200, "text/html", answer);
  pixels.fill(pixels.Color(20, 200, 100));
  pixels.show();
}
void OFF() {
  setAnswer();
  if(digitalRead(2)==HIGH){
    cont=cont+1;
    }
  
  digitalWrite(2, LOW);
  digitalWrite(14, LOW);
  server.send(200, "text/html", answer);
  pixels.fill(pixels.Color(0, 0, 0));
  pixels.show();
}

void Reset() {
  cont=0;
  setAnswer();
  server.send(200, "text/html", answer);
}
void setAnswer() {
  answer = " <!DOCTYPE html>\
            <html>\
            <head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\
            <title>LED Control</title>\
            <style>\
            html { height:100% ;font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\
            body { background-image: linear-gradient(rgb(10,10,89),#FF2506); font-family: Arial, Helvetica, Sans-Serif;} \
            h1 {color: #EE0000;margin: 50px auto 30px;}\
            h2 {color: rgb(255,255,255);margin: 50px auto 30px; font-size:20px}\
            h3 {color: rgb(255,255,255);margin-bottom: 50px;}\
            p {font-size: 14px;color: #888;margin-bottom: 10px;}\
            </style>\
            </head>\
            <body>\
            <h1>SENSORES TERRAZA</h1>\
            <h2>you can turn ON with the following button </h2>\
            <p><a href='/on'><button style='height:50px;width:100px;border-top-left-radius: 50px;border-top-right-radius: 50px;border-bottom-right-radius: 50px;border-bottom-left-radius:50px'>ON</button></a></p>\
            <h2>Avec cete vous pouvez turn it OFF </h2>\
            <p><a href='/off'><button style='height:50px;width:100px;border-top-left-radius: 50px;border-top-right-radius: 50px;border-bottom-right-radius: 50px;border-bottom-left-radius:50px'>OFF</button></a></p>\  
            <h2>veces pulsado oN</h2>\
            <p><a href='/Reset'><button style='height:100px;width:100px;border-top-left-radius: 10px;border-top-right-radius: 10px;border-bottom-right-radius: 10px;border-bottom-left-radius:10px'>RESET</button></a></p>\
            <h3>\"" + (String)cont + "\" %</h3>\
            <h2>Tiempo Total de encendido</h2>\
            <h3>\"" + (String)total + "\" %</h3>\
            </body>\
            </html>";
}
