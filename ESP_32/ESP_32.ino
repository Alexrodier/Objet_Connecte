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
<html lang='fr'>\
    <head>\
        <meta charset='utf-8'>\
        <title>ESP32 ROBOT</title>\
        <meta name='robots' content='index, follow'>\
        <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
    </head>\
  <style>\
        header{text-align:center;\
    font-size:60px;\
      color:#abddda;\
    font-weight:bold;}\
    form{\
      text-aligin:center;\
      }\
    button{\
      zoom:140%;\
      border-radius:10%;\
      color:white;\
      background-color:#F7B05E;\
      margin-left:20px;\
      }\
    .submit {\
      zoom:150%;\
      border-radius:15%;\
      cursor: pointer;\
        color:white;\
      padding:4px 10px 4px 10px;\
      background-color:#21B4AB;\
      text-decoration:none;\
      margin-left:20px;\
      margin-bottom:20px;\
    }\
    .submit_1 {\
      margin-left:20px;\
      margin-bottom:20px;\
      zoom:200%;\
      border-radius:15%;\
        color:white;\
      padding:4px 10px 4px 10px;\
      background-color:#F36C62;\
      border-top:1px solid #eee;\
      border-left:1px solid #eee;\
      border-bottom:1px solid #eee;\
      border-right:1px solid #eee;\
    }\
    body\
    {\
       background-image:url('http://www.yjpoo.com/images/201901/goods_img/19854_p_1546755950368.jpg');\
    }\
  </style>\
    <body onload='loads();'>\
        <header>\
            <h1>ROBOT</h1>\
        </header>\
    <form style='text-align:center;'>\
    <input type='button' name='' id='A' class='submit' value='A' onclick='myClicka()'/>\
    <input type='button' name='' id='B' class='submit' value='B' onclick='myClickb()'/>\
    <input type='button' name='' id='C' class='submit' value='C' onclick='myClickc()'/>\
    <input type='button' name='' id='D' class='submit' value='D' onclick='myClickd()'/></br>\
    <a  id='flag'><button type='button' onclick='myFunction()'>submit</button></a>\
    <a  id='flag2'><button type='button' onclick='myFunction2()'>clear</button></a>\
    </form>\
    <p id='result'></p>\
        <script>\
    function loads(){\
      A=window.location.href.split('?')[1];\
            A1=A.split('=')[1];\
      if(A1==1){\
        document.getElementById('A').className='submit_1';\
        }\
      B=window.location.href.split('?')[2];\
      B1=B.split('=')[1];\
      if(B1==1){\
        document.getElementById('B').className='submit_1';\
        }\
      C=window.location.href.split('?')[3];\
      C1=C.split('=')[1];\
      if(C1==1){\
        document.getElementById('C').className='submit_1';\
        }\
      D=window.location.href.split('?')[4];\
      D1=D.split('=')[1];\
      if(D1==1){\
        document.getElementById('D').className='submit_1';\
        }\
      }\
    function myClicka(){\
      var _submit = document.getElementById('A');\
      if(_submit.className=='submit_1'){\
        document.getElementById('A').className='submit';\
        }\
      else{\
      document.getElementById('A').className='submit_1';\
      }\
    }\
    function myClickb(){\
      var _submit = document.getElementById('B');\
      if(_submit.className=='submit_1'){\
        document.getElementById('B').className='submit';\
        }\
      else{\
      document.getElementById('B').className='submit_1';\
      }\
    }\
    function myClickc(){\
      var _submit = document.getElementById('C');\
      if(_submit.className=='submit_1'){\
        document.getElementById('C').className='submit';\
        }\
      else{\
      document.getElementById('C').className='submit_1';\
      }\
    }\
    function myClickd(){\
      var _submit = document.getElementById('D');\
      if(_submit.className=='submit_1'){\
        document.getElementById('D').className='submit';\
        }\
      else{\
      document.getElementById('D').className='submit_1';\
      }\
    }\
    function myFunction() {\
        var x, text,ahref;\
      var checkboxs = document.getElementsByTagName('input');\
      let alink = document.getElementById('flag');\
      if (document.getElementById('A').className=='submit_1') {\
            ahref='?a=1';\
      }\
      else{ahref='?a=0';}\
      if (document.getElementById('B').className=='submit_1') {\
            ahref=ahref+'?b=1';\
      }\
      else{ahref=ahref+'?b=0';}\
      if (document.getElementById('C').className=='submit_1') {\
        ahref=ahref+'?c=1';\  
      }\
      else{ahref=ahref+'?c=0';}\
      if (document.getElementById('D').className=='submit_1') {\
       ahref=ahref+'?d=1';\
      }\
      else{ahref=ahref+'?d=0';}\
      alink.setAttribute('href', ahref);\
        document.getElementById('result').innerHTML = text;\
    }\
    function myFunction2(){\
      var ahref;\
      let alink = document.getElementById('flag2');\
      ahref='?a=0?b=0?c=0?d=0';\
      alink.setAttribute('href', ahref);\
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
  if(server.arg(0)[4]=='1'){
    //Serial.println("B");
    parcours.insert(parcours.end(), 'B');
  }
  if(server.arg(0)[8]=='1'){
    //Serial.println("C");
    parcours.insert(parcours.end(), 'C');
  }
  if(server.arg(0)[12]=='1'){
    //Serial.println("D");
    parcours.insert(parcours.end(), 'D');
  }
}
