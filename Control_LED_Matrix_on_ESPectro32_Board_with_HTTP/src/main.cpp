#include <Arduino.h>
#include <ESPectro32_Board.h>
#include <WiFi.h>

Adafruit_IS31FL3731 matrix;
const char* ssid     = "SSID";
const char* password = "password";
int LED[7][7];

WiFiServer server(80);

void LED_Matrix_Condition(int coorX, int coorY ){
  if(LED[coorX][coorY]==0) {
    LED[coorX][coorY] = 200;            
  }
  else {
    LED[coorX][coorY] = 0;           
  }
}

void LED_Matrix(){
  for (int x = 0; x<=6; x++){
    for (int y = 0; y<=6; y++){
      matrix.drawPixel(x, y, LED[x][y]);
    }
  }
}

void setup()
{
    Serial.begin(115200);

    //LED Matrix initialitation
    Serial.println("ISSI manual animation test");
    if (! matrix.begin()) {
        Serial.println("IS31 not found");
        while (1);
    }
    Serial.println("IS31 Found!");
    delay(10);

    // We start by connecting to a WiFi network

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
    
    server.begin();
}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<a href=\"/0\">0</a> <a href=\"/1\">0</a> <a href=\"/2\">0</a> <a href=\"/3\">0</a> <a href=\"/4\">0</a> <a href=\"/5\">0</a> <a href=\"/6\">0</a><br>");
            client.print("<a href=\"/7\">0</a> <a href=\"/8\">0</a> <a href=\"/9\">0</a> <a href=\"/10\">0</a> <a href=\"/11\">0</a> <a href=\"/12\">0</a> <a href=\"/13\">0</a><br>");
            client.print("<a href=\"/14\">0</a> <a href=\"/15\">0</a> <a href=\"/16\">0</a> <a href=\"/17\">0</a> <a href=\"/18\">0</a> <a href=\"/19\">0</a> <a href=\"/20\">0</a><br>");
            client.print("<a href=\"/21\">0</a> <a href=\"/22\">0</a> <a href=\"/23\">0</a> <a href=\"/24\">0</a> <a href=\"/25\">0</a> <a href=\"/26\">0</a> <a href=\"/27\">0</a><br>");
            client.print("<a href=\"/28\">0</a> <a href=\"/29\">0</a> <a href=\"/30\">0</a> <a href=\"/31\">0</a> <a href=\"/32\">0</a> <a href=\"/33\">0</a> <a href=\"/34\">0</a><br>");
            client.print("<a href=\"/35\">0</a> <a href=\"/36\">0</a> <a href=\"/37\">0</a> <a href=\"/37\">0</a> <a href=\"/38\">0</a> <a href=\"/39\">0</a> <a href=\"/40\">0</a><br>");
            client.print("<a href=\"/42\">0</a> <a href=\"/43\">0</a> <a href=\"/44\">0</a> <a href=\"/45\">0</a> <a href=\"/46\">0</a> <a href=\"/47\">0</a> <a href=\"/48\">0</a><br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /0")) {
          LED_Matrix_Condition(0,0);
        }
        else if (currentLine.endsWith("GET /1")) {
          LED_Matrix_Condition(0,1);
        }
         else if (currentLine.endsWith("GET /2")) {
          LED_Matrix_Condition(0,2);
        }
         else if (currentLine.endsWith("GET /3")) {
          LED_Matrix_Condition(0,3);
        }
         else if (currentLine.endsWith("GET /4")) {
          LED_Matrix_Condition(0,4);
        }
         else if (currentLine.endsWith("GET /5")) {
          LED_Matrix_Condition(0,5);
        }
         else if (currentLine.endsWith("GET /6")) {
          LED_Matrix_Condition(0,6);
        }
         else if (currentLine.endsWith("GET /7")) {
          LED_Matrix_Condition(1,0);
        }
         else if (currentLine.endsWith("GET /8")) {
          LED_Matrix_Condition(1,1);
        }
         else if (currentLine.endsWith("GET /9")) {
          LED_Matrix_Condition(1,2);
        }
         else if (currentLine.endsWith("GET /10")) {
          LED_Matrix_Condition(1,3);
        }
         else if (currentLine.endsWith("GET /11")) {
          LED_Matrix_Condition(1,4);
        }
         else if (currentLine.endsWith("GET /12")) {
          LED_Matrix_Condition(1,5);
        }
         else if (currentLine.endsWith("GET /13")) {
          LED_Matrix_Condition(1,6);
        }
         else if (currentLine.endsWith("GET /14")) {
          LED_Matrix_Condition(2,0);
        }
         else if (currentLine.endsWith("GET /15")) {
          LED_Matrix_Condition(2,1);
        }
         else if (currentLine.endsWith("GET /16")) {
          LED_Matrix_Condition(2,2);
        }
         else if (currentLine.endsWith("GET /17")) {
          LED_Matrix_Condition(2,3);
        }
         else if (currentLine.endsWith("GET /18")) {
          LED_Matrix_Condition(2,4);
        }
         else if (currentLine.endsWith("GET /19")) {
          LED_Matrix_Condition(2,5);
        }
         else if (currentLine.endsWith("GET /20")) {
          LED_Matrix_Condition(2,6);
        }
         else if (currentLine.endsWith("GET /21")) {
          LED_Matrix_Condition(3,0);
        }
         else if (currentLine.endsWith("GET /22")) {
          LED_Matrix_Condition(3,1);
        }
         else if (currentLine.endsWith("GET /23")) {
          LED_Matrix_Condition(3,2);
        }
         else if (currentLine.endsWith("GET /24")) {
          LED_Matrix_Condition(3,3);
        }
         else if (currentLine.endsWith("GET /25")) {
          LED_Matrix_Condition(3,4);
        }
         else if (currentLine.endsWith("GET /26")) {
          LED_Matrix_Condition(3,5);
        }
         else if (currentLine.endsWith("GET /27")) {
          LED_Matrix_Condition(3,6);
        }
         else if (currentLine.endsWith("GET /28")) {
          LED_Matrix_Condition(4,0);
        }
         else if (currentLine.endsWith("GET /29")) {
          LED_Matrix_Condition(4,1);
        }
         else if (currentLine.endsWith("GET /30")) {
          LED_Matrix_Condition(4,2);
        }
         else if (currentLine.endsWith("GET /31")) {
          LED_Matrix_Condition(4,3);
        }
         else if (currentLine.endsWith("GET /32")) {
          LED_Matrix_Condition(4,4);
        }
         else if (currentLine.endsWith("GET /33")) {
          LED_Matrix_Condition(4,5);
        }
         else if (currentLine.endsWith("GET /34")) {
          LED_Matrix_Condition(4,6);
        }
         else if (currentLine.endsWith("GET /35")) {
          LED_Matrix_Condition(5,0);
        }
         else if (currentLine.endsWith("GET /36")) {
          LED_Matrix_Condition(5,1);
        }
         else if (currentLine.endsWith("GET /37")) {
          LED_Matrix_Condition(5,2);
        }
         else if (currentLine.endsWith("GET /38")) {
          LED_Matrix_Condition(5,3);
        }
         else if (currentLine.endsWith("GET /39")) {
          LED_Matrix_Condition(5,4);
        }
         else if (currentLine.endsWith("GET /40")) {
          LED_Matrix_Condition(5,5);
        }
         else if (currentLine.endsWith("GET /41")) {
          LED_Matrix_Condition(5,6);
        }
         else if (currentLine.endsWith("GET /42")) {
          LED_Matrix_Condition(6,0);
        }
         else if (currentLine.endsWith("GET /43")) {
          LED_Matrix_Condition(6,1);
        }
         else if (currentLine.endsWith("GET /44")) {
          LED_Matrix_Condition(6,2);
        }
         else if (currentLine.endsWith("GET /45")) {
          LED_Matrix_Condition(6,3);
        }
         else if (currentLine.endsWith("GET /46")) {
          LED_Matrix_Condition(6,4);
        }
         else if (currentLine.endsWith("GET /47")) {
          LED_Matrix_Condition(6,5);
        }
         else if (currentLine.endsWith("GET /48")) {
          LED_Matrix_Condition(6,6);
        }            
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
    Serial.print(LED[0][1]);
  }
  LED_Matrix();
}

