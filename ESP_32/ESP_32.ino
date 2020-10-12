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
//////////////////////////////



// SSID & Password
const char* ssid = "HUAWEI P30 ALEX";  // Enter your SSID here
const char* password = "AlexConnection";  //Enter your Password here
vector<char> parcours;
WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

void setup() {
  Serial.begin(9600);
  pinMode(1, OUTPUT);
  pinMode(3, INPUT);
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

char intersection = '0';
unsigned long checkUid;
int start = 0;

void send_order(int nb_order){
  for(int k=0;k<nb_order;k++){  
    Serial.write(ordres[0]);
    ordres.erase(ordres.begin());
  }
}

char command = 'a';
char card = '0';

void loop() {
  server.handleClient();
  if(parcours.size()){
    get_order(parcours);
    parcours.clear();
  }
  if(start == 0 && ordres.size()){
    send_order(1);
    start = 1;
  }
  if(mfrc522.PICC_IsNewCardPresent()){
    checkUid = getID();
    if(checkUid != -1 && checkUid != card){
      card = checkUid;
      intersection = rechercher_Intersection(checkUid, liste_intersection);
      send_order(2);
    }
  }
  if(Serial.available()){
    command = Serial.read();
    if(command == 'W'){
      send_order(2);
    }
  }

  /*if(parcours.size()){
    get_order(parcours);
    parcours.clear();
    intersection = rechercher_Intersection(getID(), liste_Intersection);
    if
    for(char ordre : ordres){
      Serial.write(ordre);
    }
    ordres.clear();
  }*/
  
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
    //Serial.println("A");
    parcours.insert(parcours.end(), 'A');
  }
  if(server.arg(0)[1]=='1'){
    //Serial.println("B");
    parcours.insert(parcours.end(), 'B');
  }
  if(server.arg(0)[2]=='1'){
    //Serial.println("C");
    parcours.insert(parcours.end(), 'C');
  }
  if(server.arg(0)[3]=='1'){
    //Serial.println("D");
    parcours.insert(parcours.end(), 'D');
  }
}
