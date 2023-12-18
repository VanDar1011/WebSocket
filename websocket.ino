#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h> // khai báo các thư viện
//-----------------------------------------------
const char* ssid = "P.3819";
const char* password = "meomeo..3"; // khai bao wifi
//-----------------------------------------------
#define LED1 12 // dinh nghia chan led
#define LED2 13
//---------
#define RXp2 16
#define TXp2 17
//-----------------------------------------------
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
//-----------------------------------------------
String readUno;
String stringLedON = "LED_ON";
String stringLedOFF = "LED_OFF";
//----
boolean LED1onoff = false;
boolean LED2onoff = false;
const int pinTouchLedGround = 14;
String JSONtxt;
//-----------------------------------------------
#include "html_page.h"
#include "functions.h"
//====================================================================
void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  //-----------------------------------------------
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  //-----------------------------------------------
  server.on("/", webpage);
  //-----------------------------------------------
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}
//====================================================================
void loop()
{
  webSocket.loop();
  server.handleClient(); //xu li http request gui den
  //----------------------------------------------- them button
  String readUno = Serial2.readStringUntil('\n');
  readUno = trimString(readUno);
  Serial.println(readUno);
  if (digitalRead(pinTouchLedGround) == HIGH) {
    LED1onoff = !LED1onoff;
  }
//if (isSubstring(stringLedON, readUno)) {
//    if(LED1onoff == false){
//      LED1onoff = true;
//      }
//      
//  } else if (isSubstring(stringLedOFF, readUno)) {
//    if(LED1onoff == true){
//      LED1onoff = false;
//    }
//    
//  }
  //-----
  if (LED1onoff == false)
    digitalWrite(LED1, LOW);
  else digitalWrite(LED1, HIGH);
  if (LED2onoff == false)
    digitalWrite(LED2, LOW);
  else digitalWrite(LED2, HIGH);
  //-----------------------------------------------
  String LED1status = "OFF";
  String LED2status = "OFF";
  if (LED1onoff == true) LED1status = "ON";
  if (LED2onoff == true) LED2status = "ON";
  JSONtxt = "{\"LED1onoff\": \"" + LED1status + "\",\"LED2onoff\": \"" + LED2status + "\"}";
  // JSONtxt = "{\"LED1onoff\":\"" + LED1status + "\"}";
  webSocket.broadcastTXT(JSONtxt);
}
bool isSubstring(String sub, String full) {
  return full.indexOf(sub) != -1;
}
String trimString(String input) {
  // Loại bỏ ký tự trắng ở đầu
  int startIndex = 0;
  while (input[startIndex] == ' ' || input[startIndex] == '\t' || input[startIndex] == '\n') {
    startIndex++;
  }

  // Loại bỏ ký tự trắng ở cuối
  int endIndex = input.length() - 1;
  while (endIndex > startIndex && (input[endIndex] == ' ' || input[endIndex] == '\t' || input[endIndex] == '\n')) {
    endIndex--;
  }

  // Trích xuất chuỗi đã được loại bỏ ký tự trắng
  return input.substring(startIndex, endIndex + 1);
}
