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
void handle_root() {
  server.send(200, "text/html", HTML);
  if(server.arg(0)[0]=='1'){
    Serial.println("A");
  }
  if(server.arg(0)[1]=='1'){
    Serial.println("B");
  }
  if(server.arg(0)[2]=='1'){
    Serial.println("C");
  }
  if(server.arg(0)[3]=='1'){
    Serial.println("D");
  }
}
