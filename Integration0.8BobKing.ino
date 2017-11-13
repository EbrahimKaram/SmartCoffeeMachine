
/*
  INTEGRATIONG CODE BY Ebrahim
  VERSION 0.6
  Add image that Firas created
  THE PURPORSE OF RUN 1 IS ULTRASONIC SENSOR CALLIBRATION
  THE PURPOSE  OF RUN 2 IS TO INTEGRATE THE POTENTIOMETER
  THE PURPOSE  OF RUN 3  IS  TO INTEGRATE THE  LCD DISPLAY TO THE FINAL PRODUCT*/



#include "TM1637.h"  // 4 digits display include
#include "Ultrasonic.h"  // ultrasonic sensro libraries 
#include "math.h"
#include "string"

#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include "SPI.h"
#endif
#include "WiFi.h"


/*MACRO DEFINITIONS:
   digital IOs at first and  then analog IOs
  Digital pins  inputs and  outputs*/
#define RELAY_PIN 40  // connecting the realy that will control the coffeE machine as digital output . 
#define CLK 39        // 4-digit clock pin 
#define DIO 38        // 4-DIGIT display data pin 

/*ANALOG pins we have 5  analog  pins going from 23 till 27 included  */
#define KNOB_PIN 24           // Knob pin used for the potentiometer to be used as an analog inpt 
#define ULTRASONIC_PIN 27     // ultrasonic sensor  (measuring water height) used as analog  pin at pin 24 
#define BUZZER_PIN 25         // Buzzer Pin

/* Global Variables */
TM1637 tm1637(CLK, DIO);                  /* 4-Digit Display object */
Ultrasonic ultrasonic(ULTRASONIC_PIN);    /* Ultrasonic Ranger object */
int distance = 0;                         /* variable to store the distance to obstacles in front */
int waterLevel = 0;
int8_t bits[4] = {0};                     /* array to store the single bits of the value */
int cupsLeft=0;

int cupsDemanded = 0 ;
int cupgoal = 0;
bool order_being_processed = false;

/* Setting Up the WifFi server */
// your network name also called SSID
char ssid[] = "Bobby";


// your network password
char password[] = "12345678";
// your network key Index number (needed only for WEP)
int keyIndex = 0;

server(80);
/* Setting Up the WifFi server */

// the setup routine runs once when you press reset:
void setup() {

  pinMode(RELAY_PIN, OUTPUT); // initialize the digital pin as an output.
  //Setting Up the serial for debug purposes
  Serial.begin(9600) ;
  /* Initialize 4-Digit Display */
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);

 /* Connecting to WiFi and creating WebServer */
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid);
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  
  while ( WiFi.status() != WL_CONNECTED) 
  {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
    }


  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  
  // you're connected now, so print out the status  
  printWifiStatus();

  Serial.println("Starting webserver on port 80");
  server.begin();                           // start the web server on port 80
  Serial.println("Webserver started!");

  
  }

  // the loop routine runs over and over again forever:
  void loop() {

/////////////////////////////////////////The Server Side/////////////////////////////////////////
 int i = 0;
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    char buffer[150] = {0};                 // make a buffer to hold incoming data
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (strlen(buffer) == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<!DOCTYPE html>");
            client.println("<html><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
            client.println("<head>");
            client.println("<style>");
            client.println("@media screen and (max-width:480px){");
            client.println("h1 {    font-size: 32px;    max-width: 400px;}");
            client.println("h2 { font-size: 44px;    max-width: 400px;}");
            client.println("p {   font-size: 20px;    max-width: 400px;}");
            client.println(".container{    max-width: 400px;  }}");
            client.println("h2 {   text-align: center;    padding-bottom: 30px;    margin-top: 0px;    font-family: Tahoma;}");
            client.println("h1 {    text-align: center;    padding-bottom: 60px;    font-family: Tahoma;}");
            client.println("p {  text-align: center;    font-size: 24px;    font-weight: 12;    font-family: Helvetica;}");
            client.println(".container {   text-align: center;   margin: 10px;   padding: 10px;}");
            client.println(".buttons{  color:#fff;  background-color: #793700;  border-color: #fff;  font-size: 14pt;");
            client.println("text-align: center;  margin: 10px;  padding-right: 25px;  padding-left: 25px;  height: 50px;  border: 1px solid; border-radius: 5px;  font-family: Helvetica;}");
            client.println(".buttons:hover{  color:#793700;  background-color:#F6F6DF; }");
            client.println("</style></head>");
            client.println("<body style=' background-color: #F6F6DF; color:#793700; padding-bottom:100px; margin: 0px;'>");
            client.println("<img src=\"http://192.168.43.1:12345/CoffeMachine%2Fchocolate_chip.png\" style='min-height:310px; padding:0px; width:100%;' border=\"0\" alt=\"Null\">");//need to change image source
            client.println("<section style='padding-top:100px; padding-bottom:200px;'>");
            
            client.println("<h2>Order Coffee:</h2>");
            client.println("<div class=\"container\">");
            client.println("<button class=\"buttons\" onclick=\"location.href='/On'\" >On</button> </div>");
            client.println("<div class=\"container\">");
            client.println("<button class=\"buttons\"  onclick=\"location.href='/Off'\"  >Off</button> </div> </div>");
//            client.println("");
//            client.println("");           
//            client.println("<h1 align=center><font color=\"red\">EEECE 425: Smart Coffee Machine</font></h1>");
//            client.print("Order Coffee <button onclick=\"location.href='/On'\">Turn on</button>");
//            client.println(" <button onclick=\"location.href='/Off'\">Turn Off</button><br>");
            
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear the buffer:
            memset(buffer, 0, 150);
            i = 0;
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          buffer[i++] = c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (endsWith(buffer, "GET /On ")) {
        
          
          digitalWrite(RELAY_PIN, HIGH);
          Serial.println();
          Serial.println("Relay On");
          Serial.println();
        }
        if (endsWith(buffer, "GET /Off ")) {
         
          digitalWrite(RELAY_PIN, LOW);
          Serial.println();
          Serial.println("Relay Off");
        Serial.println();
        }
          if(endsWithNumber(buffer,"GET /?quantity=")){ //if you press quantity directly
     
      Serial.println(numberOfEnding(buffer,"GET /?quantity=")); 
      cupsDemanded=numberOfEnding(buffer,"GET /?quantity=");
      order_being_processed=false;
      Serial.println();
      Serial.println("Number of Cups wanted");
      
      Serial.println(cupsDemanded);
      Serial.println();
      //Number of Cups wanted 
          }


if(endsWithNumber(buffer,"GET /On?quantity=")){ //if you press then quantity
     
      Serial.println(numberOfEnding(buffer,"GET /On?quantity=")); 
      cupsDemanded=numberOfEnding(buffer,"GET /On?quantity=");
      order_being_processed=false;
      Serial.println();
      Serial.println("Number of Cups wanted");
      
      Serial.println(cupsDemanded);
      Serial.println();
     
      //Number of Cups wanted 
          }

          
          
      
      }
    }
    distance = ultrasonic.MeasureInCentimeters();
    waterLevel = 10 - distance; //How much water is left
    if (waterLevel < 0)
    { waterLevel = 0;
    }
    
 
    cupsLeft = CupsLeft(waterLevel); // displays the cups left to use
    
    char cups_left_disp[500] = "";
    char cups_left_str[4] = "";
    sprintf(cups_left_str, "%d", cupsLeft);
    strcpy (cups_left_disp,"<div class=\"container\"> <p> We can currently make ");
    //strcpy (cups_left_disp,"<div class=\"container\"> <form action='' method='get'> We can currently make ");
    strcat (cups_left_disp, cups_left_str);
    strcat (cups_left_disp, " cups of coffee. How many would you like? </p> ");
  
    //<select><option value='1'>1</option><option value='1'>1</option></select> type='submit' onclick='submitted_how_many();'/> </form>");
    strcat (cups_left_disp, "<form action='' method='get'>");
    strcat (cups_left_disp, "<input class=\"container\" name=\"quantity\" type=\"number\" min=\"1\" max=\"");
    
    strcat(cups_left_disp, cups_left_str) ; 
    strcat(cups_left_disp,"\" step=\"1\" value=\"1\" required style=\"border:1px solid; border-radius:5px; border-color:#793700; width:100px; height:15px;\"><br></form>");
    client.println(cups_left_disp);
    client.println("</div> </section> </body> </html>");
    Serial.println(buffer) ; 

   // client.println("<script>function() { var url = ''; url. }</script>");
//    client.println("<script>setTimeout(function(){ location.reload(); }, 3000);</script>");
    // close the connection:
//  if (client.available() > 0) {
//      // read the bytes incoming from the client:
//    
//      char thisChar = client.read();
//     
//      
//    };
    
      client.stop();
  }


////////////////Order my cups of coffee////////////////
Serial.println();
Serial.println("Before If condition cupsDemanded");
Serial.println(cupsDemanded);
Serial.println();
if(cupsDemanded!=0)
{
  Serial.println("What is order_being_processed?");
  Serial.println(order_being_processed);
  
    if (!order_being_processed)
    {
      Serial.println("It BLOODY WORKS HALELUJAH");
      cupgoal = cupsLeft - cupsDemanded;
      order_being_processed = true;
    }

Serial.println("What is the value cupgoal?");
Serial.println(cupgoal);
Serial.println("What is the value cupsLeft?");
Serial.println(cupsLeft);
    if (cupsLeft <= cupgoal)
    {
      digitalWrite(RELAY_PIN, LOW);
      order_being_processed = false;
      cupgoal=0; //reset value
      cupsDemanded=0; //reset value
      Serial.println("Cups are done!!");
      delay(10000);
      
      
    }

  }
  
////////////////Order my cups of coffee////////////////
  


    distance = ultrasonic.MeasureInCentimeters();
    waterLevel = 10 - distance; //How much water is left
    if (waterLevel < 0)
    { waterLevel = 0;
    }

    if ((waterLevel <= 0)&&(digitalRead(RELAY_PIN))) //Buzzer only turns on if there are no cups left and the coffee machine is on
    {
      analogWrite(BUZZER_PIN, GetVoltage());
      //Serial.println("Buzzer On");
      //Serial.println(waterLevel);
    }
    else
    {
      analogWrite(BUZZER_PIN, 0);
      //Serial.println("Buzzer Off");
      
    }
  

    memset(bits, 0, 4);                             /* reset array before we use it */

    cupsLeft = CupsLeft(waterLevel); // displays the cups left to use.
    int cupsLeft_Display=cupsLeft;
    //client.println("number of cups: <span id='nbcups'>N</span>");
    for (int i = 3; i >= 0; i--)
    {
      /* Convert the value to individual decimal digits for display */
      bits[i] = cupsLeft_Display % 10;
      cupsLeft_Display = cupsLeft_Display / 10;
      tm1637.display(i, bits[i]);                 /* display on 4-Digit Display */
    }
    Serial.println(distance);
    delay(500);   // wait for half a second

}


  int GetVoltage ()
  {
    int knobValue = analogRead(KNOB_PIN);
    int voltage = knobValue * (240.0 / 4095.0); //has to be a value be value between 0 and 240
    //Serial.print("knobValue: ");
    //Serial.print(knobValue);
    //Serial.print("\n");
    return voltage;

  }

  int CupsLeft(int waterLeft) // /Value Ranges betweeen 1 to 6;
  {
    int cupsLeft;

    if (waterLeft <= 0) {
      cupsLeft = 0;

    }
    else
    {
      cupsLeft = round((waterLeft + 2.0) / 2.0);
    }
    return cupsLeft;
  }

boolean endsWith(char* inString, char* compString) {
  int compLength = strlen(compString);
  int strLength = strlen(inString);
  
  //compare the last "compLength" values of the inString
  int i;
  for (i = 0; i < compLength; i++) {
    char a = inString[(strLength - 1) - i];
    char b = compString[(compLength - 1) - i];
    if (a != b) {
      return false;
    }
  }
  return true;
}


boolean endsWithNumber(char* inString, char* compString) {
  int compLength = strlen(compString);
  int strLength = strlen(inString);
  
  //compare the last "compLength" values of the inString
  int i;
  for (i = 0; i < compLength; i++) {
    char a = inString[(strLength - 2) - i];
    char b = compString[(compLength - 1) - i];
    if (a != b) {
      return false;
    }
  }
  return true;
}

int numberOfEnding(char* inString, char* compString){
  int strLength = strlen(inString);
  if (endsWithNumber( inString, compString ))
  { 
    int value  = static_cast<int>(inString[strLength-1]) - static_cast<int>('0');
    return value ; 
    }
    else return -1 ; 
  }


///////////////Detects a change in the number of cups///////////////////
bool detectChangeinCups(int cupsLeft)
{
  if(cupsLeft==CupsLeft(waterLevel))
    return false;

  else 
    return true;  
}
///////////////Detects a change in the number of cups///////////////////

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
   
}

