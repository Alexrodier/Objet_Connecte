#include <WiFi.h>
#include <WebServer.h>

// SSID & Password
const char* ssid = "zpdeiPhone";  // Enter your SSID here
const char* password = "zhang199841";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

void setup() {
  Serial.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_root);

  server.begin();
  Serial.println("HTTP server started");
  delay(100); 
}

void loop() {
  server.handleClient();
}

// HTML & CSS contents which display on web server
String HTML = "<!DOCTYPE html>\
<html lang='en'>\
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
void handle_root() {
  server.send(200, "text/html", HTML);
  if(server.arg(0)[0]=='1'){
    Serial.println("A");
  }
  if(server.arg(0)[4]=='1'){
    Serial.println("B");
  }
  if(server.arg(0)[8]=='1'){
    Serial.println("C");
  }
  if(server.arg(0)[12]=='1'){
    Serial.println("D");
  }
}
