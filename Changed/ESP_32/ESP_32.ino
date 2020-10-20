#include <Arduino.h>
#include <iostream>
#include <WiFi.h>
#include <WebServer.h>
#include "djikstra.h"
///////// RFID import
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 5
#define RST_PIN 0
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// SSID & Password
const char* ssid = "HUAWEI P30 ALEX";  // Enter your SSID here
const char* password = "AlexConnection";  //Enter your Password here
vector<char> parcours;
vector<vector<char>> ensemble_parcours;
WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

void setup() {
  Serial.begin(115200);
  //Serial.println("Try Connecting to ");
  //Serial.println(ssid);
    ////   RFID settings
    SPI.begin();            // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card
    /////////////
  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("WiFi connected successfully");
  //Serial.print("Got IP: ");
  //Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_root);

  server.begin();
  //Serial.println("HTTP server started");
  delay(100);
}

unsigned long getID(){
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return -1;
  }
  unsigned long hex_num;
  hex_num =  mfrc522.uid.uidByte[0] << 24;
  hex_num += mfrc522.uid.uidByte[1] << 16;
  hex_num += mfrc522.uid.uidByte[2] <<  8;
  hex_num += mfrc522.uid.uidByte[3];
  mfrc522.PICC_HaltA(); // Stop reading
  return hex_num;
}

void send_order(int nb_order){
  for(int k=0;k<nb_order;k++){  
    Serial.write(ordres[0]);
    ordres.erase(ordres.begin());
    delay(500<<);
  }
}

int is_at_start = 1;
char intersection = '0';
unsigned long checkUid;
char command = 'a';
char card = '0';
int is_lost = 0;

void loop() {
  server.handleClient();
  if(ensemble_parcours.size() && is_at_start == 1){
    get_order(ensemble_parcours[0]);
    ensemble_parcours.erase(ensemble_parcours.begin());
    send_order(1);
    is_at_start = 0;
  }
  if(mfrc522.PICC_IsNewCardPresent()){
    checkUid = getID();
    if(checkUid != -1){
      if(card != checkUid){
        card = checkUid;
        is_lost = 0;
        send_order(2);
      }else if(is_lost){
        Serial.write('Z');
        is_lost = 0;
      }
      //intersection = rechercher_Intersection(checkUid, liste_intersection);
    }
  }
  if(Serial.available()){
    command = Serial.read();
    switch(command){
      case 'W':
        is_lost = 1;
        break;
      case 'F':
        is_at_start = 1;
        break;
      break;
    }
  }
}

// HTML & CSS contents which display on web server
String HTML = "<!DOCTYPE html>\
<html>\
<body>\
<header>\
<h1>ROBOT</h1>\
</header>\
<form>\
<input id='a' type='checkbox' value='A'>A\
<input id='b' type='checkbox' value='B'>B\
<input id='c' type='checkbox' value='C'>C\
<input id='d' type='checkbox' value='D'>D\
<a  id='flag'><button type='button' onclick='myFunction()'>submit</button></a>\
</form>\
<p id='result'></p>\
<script>\
function myFunction(){\
  var x,text,ahref;\
  var checkboxs = document.getElementsByTagName('input');\
  let alink=document.getElementById('flag');\
  if(checkboxs[0].checked===true){\
  x=document.getElementById('a').value;\
  ahref='?a=1'\
  }\
  else{ahref='?a=0';}\
  if(checkboxs[1].checked===true){\
  x=x+' '+document.getElementById('b').value;\
  ahref=ahref+'1';\
  }\
  else{ahref=ahref+'0';}\
  if(checkboxs[2].checked===true){\
  x=x+' '+document.getElementById('c').value;\
  ahref=ahref+'1';\
  }\
  else{ahref=ahref+'0';}\
  if(checkboxs[3].checked===true){\
  x=x+' '+document.getElementById('d').value;\
  ahref=ahref+'1';\
  }\
  else{ahref=ahref+'0';}\
  alink.setAttribute('href', ahref);\
  text='Selection des destinations suivantes:'+x;\
  document.getElementById('result').innerHTML=text;\
}\
</script>\
</body>\
</html>";

// Handle root url (/)
void handle_root(){
  server.send(200, "text/html", HTML);
  
  if(server.arg(0)[0]=='1'){
    parcours.insert(parcours.end(), 'A');
  }
  if(server.arg(0)[1]=='1'){
    parcours.insert(parcours.end(), 'B');
  }
  if(server.arg(0)[2]=='1'){
    parcours.insert(parcours.end(), 'C');
  }
  if(server.arg(0)[3]=='1'){
    parcours.insert(parcours.end(), 'D');
  }
  ensemble_parcours.insert(ensemble_parcours.end(), parcours);
  parcours.clear();
}
