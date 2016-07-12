
/* Author: Devvrat arya */


/* The following is the program for minor project of sixth semester*/
/*It uses an esp8266 nodemcu board to control a brightness of an 42 LED panel and a position of servo*/
/*After connecting to nodemcu wifi access point with the password and ssid given below */
/*One can can give command in url of the browser as 192.168.4.1/l<brightness>s<angle>   */
/*For example put 192.168.4.1/l75s90 will set brightness to 75% and servo angle at 90 */

// Range
// Brightness: 0-100
// Angle: 0-180

//Connections for Node mcu
// Servo : D5- GPI012
// LED: D6- GPIO14


#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Servo.h>

/* Credentials are here */
const char *ssid = "LEDspot";
const char *password = "controltheled";

ESP8266WebServer server(80);

int led=12;
int servo_con=14;
char command_type;
int value=100;

Servo servo;


/* 
 *  This fuction is to write LED brightness level
 *  Input: perentage of birghtness 
 *  Output: LED brightness setup to that point
*/
void LED_brightness (int level)
{
      analogWrite(led,level*10);
}



/*
 * To adjust the Angle of servo 
 * Input : servo angle from 1 to 180 degrees
 * Output: Servo takes that angle
*/
void servo_angle ( int angle)
  {
      servo.write(angle); 
  }

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  server.send(200, "text/html", "<h1>You can control the LED now.</h1> <BR>  <h1>Enter the commmand in url as <h2> <br> LED: 192.168.4.1/l50 <br> Rotation: 192.168.4.1/r90 </h2>");
}

String command;
void handleNotFound()
   { 
    command = server.uri();
    Serial.println(command);
    command_type=command.charAt(1);       // Extract the first word after '/' mark
    value=(command.substring(2)).toInt(); // Extract the 2 digit number after that

       if(command_type==108 &&  (value<101 && value>=0)) // ascii value of l
       { 
         LED_brightness(value);
         server.send(200, "text/html", "<h1> Light intensity at  specified value </h1> ");
         
        } 
       
       else if(command_type== 114 && ((value<181 && value>=0))) // ascii value of s
       {
            servo_angle(value);
             server.send(200, "text/html", "<h1> Setting servo at specified value </h1>");
        
       }
       
       else   //Wrong input goes here
           {  server.send(200, "text/html", "<h1> Wrong command type.<br> LED: 192.168.4.1/l50 <br> Rotation: 192.168.4.1/r90  </h1> ");
           }
      
   }

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  pinMode(led,INPUT);
  servo.attach(servo_con);
}


void loop() {
  server.handleClient();
  }


