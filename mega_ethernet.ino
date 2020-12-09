#include <Ethernet2.h> //thư viện ethernet2 dùng cho w5500
#include <PubSubClient.h>
#include <Base64.h>
#include "EEPROM.h"
#include <ArduinoJson.h>
#include <avr/pgmspace.h>

//---------------   WEB CONFIG   ---------------
const static char web_config[] PROGMEM = \
    "<HTML lang='en-US'>"\
    "<head><meta charset= 'UTF-8'/>"\
    "<script type='text/javascript'>"\
    // Change the type of input to password or text
    "function Toggle() {"\
    "var temp = document.getElementById('mqttpass');"\
    "if (temp.type === 'password') {"\
    "temp.type = 'text';"\
    "}"\
    "else {"\
    "temp.type = 'password';"\
    "}"\
    "}"\
    //
    "function Confirm() {"\
    "if (document.getElementById('deviceID').value ==0 &&"\
    "document.getElementById('ip').value ==0 &&"\
    "document.getElementById('subnet').value ==0 &&"\
    "document.getElementById('gateway').value ==0 &&"\
    "document.getElementById('myDns').value ==0 &&"\
    "document.getElementById('MAC').value ==0 &&"\
    "document.getElementById('server').value ==0 &&"\
    "document.getElementById('mqttuser').value ==0 &&"\
    "document.getElementById('mqttpass').value ==0"\
    "){"\
    "baohieu.textContent=\"Nothing to save!\";"\
    "return false;"\
    "}"\
    "return true;"\
    "}"\
    "</script>"\
    "</head>"
    "<body>"\
    "<center>"\
    "<form method=\"post\" id='form2' action='/pass'>"\
    "</form>"\
    ////////
    "<form method=\"post\" id='form3' action='/' >"\
    "</form>"\
    ////////
    "<form method=\"post\" id='form1' action='/' onsubmit ='return Confirm()'>"\
    "<table border='5' >"\
    "<caption align='TOP'><H3>SETTING</H3></caption>"\
    "<tr>"\
    "<th colspan='2'>DEVICE</th>"\
    "</tr>"\
    "<tr>"\
    "<th><label for=\"deviceID\">DeviceID</label></th>"\
    "<td><input type=\"text\" name=\"deviceID\" id=\"deviceID\" placeholder ='~' maxlength='20' pattern='[A-Za-z0-9 _-]{0,20}' title=\"Doesn't contain special characters and no more 20 characters\"/></td> "\
    "</tr>"\
    //
    "<tr>"\
    "<th><label for=\"ip\">IP Address</label></th>"\
    "<td><input type='text' name='ip' id='ip' placeholder ='~' maxlength='15' pattern=\"(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\"  title=\"ip's format: xxx.xxx.xxx.xxx\" /></td>"\
    "</tr>"\
    "<tr>"\
    "<th><label for=\"subnet\">Subnet Mask</label></th>"\
    "<td><input type=\"text\" name=\"subnet\" id=\"subnet\" placeholder ='~' maxlength='15' pattern=\"(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\"  title=\"ip's format: xxx.xxx.xxx.xxx\" /></td>"\
    "</tr>"\
    "<tr>"\
    "<th><label for=\"gateway\">Gateway</label></th>"\
    "<td><input type=\"text\" name=\"gateway\" id=\"gateway\" placeholder ='~' maxlength='15' pattern=\"(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\"  title=\"ip's format: xxx.xxx.xxx.xxx\" /></td>"\
    "</tr>"\
    "<tr>"\
    "<th><label for=\"myDns\">DNS Server</label></th>"\
    "<td><input type=\"text\" name=\"myDns\" id=\"myDns\" placeholder ='~' maxlength='15' pattern=\"(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\"  title=\"ip's format: xxx.xxx.xxx.xxx\" /></td>"\
    "</tr>"\
    "<tr>"\
    "<th><label for=\"MAC\">MAC Address</label></th>"\
    "<td><input type=\"text\" name=\"MAC\"  id=\"MAC\" placeholder ='~' maxlength='20' pattern='^([0-9a-fA-F][0-9a-fA-F]-){5}([0-9a-fA-F][0-9a-fA-F])$' title= \"MAC's format: xx-xx-xx-xx-xx-xx\" /></td>"\
    "</tr>"\
    //
    "<tr>"\
    "<th colspan='2'>MQTT</th>"\
    "</tr>"\
    "<tr>"\
    "<th><label for=\"server\">Server's IP</label></th>"\
    "<td><input type=\"text\" name=\"server\" id=\"server\" placeholder ='~' maxlength='15' pattern=\"(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\"  title=\"ip's format: xxx.xxx.xxx.xxx\" /></td>"\
    "</tr>"\
    "<tr>"\
    "<th><label for=\"mqttuser\">Username</label></th>"\
    "<td><input type=\"text\" name=\"mqttuser\" id=\"mqttuser\" placeholder ='~' maxlength='20' pattern='[A-Za-z0-9]{0,20}' title=\"Doesn't contain special characters and no more 20 characters\" /></td>"\
    "</tr>"\
    "<tr>"\
    "<th><label for=\"mqttpass\">Password</label></th>"\
    "<td><input type=\"password\" name=\"mqttpass\" id=\"mqttpass\" placeholder ='********' maxlength='20' pattern='[A-Za-z0-9@]{0,20}' title=\"Doesn't contain special characters and no more 20 characters\" />"\
    "<input type='checkbox' onclick='Toggle()' disable title='Show password'/>"\
    "</td>"\
    "</tr>"\
    "</table>"\
    "</form>"\
    "<center><div id = 'baohieu' style= 'color:#FF00FF';></div></center>"\
    "</br>"\
    "<table>"\
    "<tr>"\
    "<td colspan='2'><INPUT TYPE='submit' NAME=\"savevalue\" VALUE=\"Save\" form='form1' style=\"line-height:1.2rem;font-size:1.0rem;\"></td>"\
    "<td colspan='2'><INPUT TYPE='submit' NAME=\"submid\" VALUE=\"Change Password\" form='form2' style=\"line-height:1.2rem;font-size:1.0rem;\"></td>"\
    "<td colspan='2'><INPUT TYPE='submit' NAME='logout' VALUE='Logout' form='form3' style=\"line-height:1.2rem;font-size:1.0rem;\"></td>"\
    "</tr>"\
    "</table>"\
    "</center>"\
    "<marquee width='100%' behavior='scroll' bgcolor='PaleTurquoise' onmouseover='this.stop()' onmouseout='this.start()'>Nha trong nam thong minh - Unitech.vn</marquee>"\
    "</body>"\
    "</html>"\
    ;

//------------    WEB_SIGN_IN    ---------------
const static char web_sign_in[] PROGMEM = \
    "HTTP/1.1 401 Unauthorized\r\n"\
    "HTTP/1.1 401 Authorization Required\r\n"\
    "WWW-Authenticate: Basic realm=\"Secure\"\r\n"\
    "Content-Type: text/html\r\n"\
    "\r\n"\
    "<html>Text to send if user hits Cancel button</html>\r\n"\
    ;

//-------------   WEB_CHANGE-PASSWORD    -------------
const static char web_change_password[] PROGMEM = \
    "<HTML>"\
    "<head><meta charset= 'UTF-8'/>"\
    "<script type='text/javascript'>"\
    // Change the type of input to password or text
    "function Confirm() {"\
    "var temp1 = document.getElementById('password').value;"\
    "var confirm = document.getElementById('confirm').value;"\
    "if (confirm ==0 && temp1==0){"\
    "baohieu.textContent=\"Enter password please!\";"\
    "return false;"\
    "}"\
    "if (confirm === temp1) {"\
    "return true;"\
    "}"\
    "else {"\
    "baohieu.textContent=\"Password don't match\";"\
    "return false;"\
    "}"\
    "}"\
    ////
    "function Toggle() {"\
    "var temp = document.getElementById('password');"\
    "if (temp.type === 'password') {"\
    "temp.type = 'text';"\
    "document.getElementById('confirm').type='text';"\
    "}"\
    "else {"\
    "temp.type = 'password';"\
    "document.getElementById('confirm').type='password';"\
    "}"\
    "}"\
    "</script>"\
    "</head>"
    "<body>"\
    "<center>"\
    "<form method=\"post\" id='form2' action='/'>"\
    "</form>"\
    ////////
    "<form method=\"post\" id='form3' action='/'>"\
    "</form>"\
    ////////
    "<form method=\"post\" id='form1' action='/' onsubmit='return Confirm()'>"\
    "<table  border='1'>"\
    "<caption align='TOP'><H3>ACCOUNT</H3></caption>"\
    "<tr>"\
    "<th><label for=\"username\">Username</label></th>"\
    "<td><input type=\"text\" name=\"username\" id=\"username\" VALUE='admin' readonly maxlength='20' pattern='[A-Za-z0-9]{0,20}' title=\"Doesn't contain special characters and no more 20 characters\"/></td>"\
    "</tr>"\
    "<tr>"\
    "<th><label for=\"password\">Password</label></th>"\
    "<td><input type=\"password\" name=\"password\" id=\"password\" placeholder ='New Password' maxlength='20' pattern='[A-Za-z0-9]{0,20}' title=\"Doesn't contain special characters and no more 20 characters\" /></td>"\
    "</tr>"\
    "<tr>"\
    "<th><label for=\"confirm\">Passwork</label></th>"\
    "<td><input type=\"password\" name=\"password\" id=\"confirm\" disable placeholder ='Confirm New Password' maxlength='20' />"\
    "<input type='checkbox' onclick='Toggle()' disable title='Show password'/>"\
    "</td>"\
    "</tr>"\
    "</table>"\
    "</form>"\
    "<center><div id = 'baohieu' style= 'color:#ff0000';></div></center>"\
    "</br>"\
    "<table>"\
    "<tr>"\
    "<td><INPUT TYPE='submit' NAME=\"savevalue\" VALUE=\"Save\" form='form1' style=\"line-height:1.2rem;font-size:1.0rem;\"></td>"\
    "<td><INPUT TYPE='submit' NAME=\"submit\" VALUE=\"Back\" form='form2' style=\"line-height:1.2rem;font-size:1.0rem;\"></td>"\
    "<td><INPUT TYPE='submit' NAME=\"logout\" VALUE='Logout' form='form3' style=\"line-height:1.2rem;font-size:1.0rem;\"></td>"\
    "</tr>"\
    "</table>"\
    "<center>"\
    "</body>"\
    "</html>"\
    ;

//----------------   VALUE_DEFAULT   ----------------------------
const static byte ip_default[] PROGMEM = {192, 168, 2, 220};
const static byte gateway_default[] PROGMEM = {192, 168, 2, 11};
const static byte subnet_default[] PROGMEM = {255, 255, 255, 0};
const static byte DNS_default[] PROGMEM = {192, 168, 2, 12};
const static byte server_default[] PROGMEM = {192, 168, 2, 24};
const static byte mac_default[] PROGMEM = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
const static char deviceID_default[] PROGMEM = "ROOM1";
const static char mqttuser_default[] PROGMEM = "phucnh";
const static char mqttpass_default[] PROGMEM = "Unitech@";
const static char username_default[] PROGMEM = "admin";
const static char password_default[] PROGMEM = "admin";
const static byte* const table_byte[] PROGMEM = {ip_default, gateway_default, subnet_default, mac_default, server_default, DNS_default};
const static char* const table_char[] PROGMEM = {deviceID_default, mqttuser_default, mqttpass_default, username_default, password_default};

//---------------------------------------------------------

#define BOM_SUONG           23
#define BOM_GIANMUA         24
#define GIANHIET            25
#define DEN                 26
#define QUAT_GIO            28
#define QUAT_DAY            29  

#define SW_BOM_SUONG        A1 
#define SW_BOM_GIANMUA      A3
#define SW_GIANHIET         A5
#define SW_DEN              A11
#define SW_QUATGIO          A7
#define SW_QUATDAY          A9 


#define SW_MODE             13  //nút nhấn chọn mode
#define LED_mode            38

String check_mode =" "; //cờ báo chế độ
//--------------------------------------------------------
byte mac[6];
IPAddress ip;
IPAddress myDns;
IPAddress gateway;
IPAddress subnet;
IPAddress server_ip;

//------------------------------------------------------------
//const char str[] PROGMEM = "P.";
//const char room[] PROGMEM = "005";
char publish_topic[20] = "";  // topic publish data
char subscribe_topic[20] = "";  //topic subscribe data
///
char deviceID[20] = "";
char mqttuser[20] = "";
char mqttpass[20] = "";
char mode_eeprom[20] = "";
char mode_eeprom1[20] = "";
///
char header[200];
int bufferSize = 0;
int flag = 1;
String chuoi=" ";
String CHEDO =" ";

//////
void callback(char* topic, byte* payload, unsigned int length);
/////
EthernetServer server(80);               // BIEN TAO SERVER
EthernetClient server_client;            // BIEN HOAT DONG TREN SERVER
EthernetClient ethClient;                // BIEN DI ETHENET
EthernetClient mqttClient;               // BIEN DI MQTT
//PubSubClient pubsub_client(server_ip, 1883, callback, mqttClient);
PubSubClient pubsub_client;

void setup() {
  Serial.begin(115200);
  //Serial2.begin(38400);   // Serial1 fo thermal print
  if ( char(EEPROM.read(0)) != '0') {
    reset_config();  //reset value default
    Serial.println('a');
  }
  Serial.println("Setting IP address......");
  confing_value();   //load value EEPROM
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  //Serial.print("local_ip: ");
  //Serial.println(Ethernet.localIP());
  if(pubsub_client.connect(deviceID, mqttuser, mqttpass)){
    Serial.println("MQTT Subscribe..");
    pubsub_client.subscribe(subscribe_topic); 
    //pubsub_client.setCallback(callback);   
  }
  pinMode(SW_MODE, INPUT);
  pinMode(LED_mode, OUTPUT);
  digitalWrite(LED_mode, HIGH);
  //pinMode(SW_BOM_SUONG, INPUT);
  DDRA = 0B11011110;
  DDRF = 0B00000000;
  DDRK = 0B00000000;
}

void loop() {
  if (Ethernet.localIP() != ip) {
    Serial.println("Attempting Ethernet connection...");
    Ethernet.begin(mac, ip, myDns, gateway, subnet);
    delay(2000);
    // server.begin();
  }
  else create_server();
  if (!pubsub_client.connected()) {
    Serial.println("disconect");
    reconnect();
  }
  pubsub_client.loop();
  if(digitalRead(SW_MODE) == LOW){
    delay(5);
    while(digitalRead(SW_MODE) == LOW);
    if(check_mode == "tudong"){
      check_mode = "thucong";
      CHEDO = "thucong";
      digitalWrite(LED_mode, LOW);
    }else if(check_mode != "tudong"){
      check_mode = "tudong";
      CHEDO = "tudong";
      digitalWrite(LED_mode, HIGH); 
    }
  }
  if(CHEDO == "thucong"){
    check_mode = "thucong";
    digitalWrite(LED_mode, LOW);
    Switch();
    delay(10);
  }
  if(CHEDO == "tudong"){
    check_mode = "tudong";
    digitalWrite(LED_mode, HIGH);
  }
  Serial.print("check_mode: ");
  Serial.println(check_mode);
  Serial.print("CHEDO: ");
  Serial.println(CHEDO);
  if(CHEDO != " ") pub_data();
}

//------------   RESET VALUE TO DEFAULT VALUE   --------------------
void reset_config(){
  char data[21] = "" ;
  //-----IP-------
  StrClear(data, sizeof(data));
  strcpy_P(data, (char *)pgm_read_word(&(table_byte[0])));
  nhapeep(data, 1, 4, 4);
  //----gateway----
  StrClear(data, sizeof(data));
  strcpy_P(data, (char *)pgm_read_word(&(table_byte[1])));
  nhapeep(data, 5, 8, 4);
  //----subnet----
  StrClear(data, sizeof(data));
  strcpy_P(data, (char *)pgm_read_word(&(table_byte[2])));
  nhapeep(data, 9, 12, 4);
  //-------mac-----
  StrClear(data, sizeof(data));
  strcpy_P(data, (char *)pgm_read_word(&(table_byte[3])));
  nhapeep(data, 13, 18, 6);
  //-------DNS----
  StrClear(data, sizeof(data));
  strcpy_P(data, (char *)pgm_read_word(&(table_byte[5])));
  nhapeep(data, 23, 26, 4);
  //-----deviceID-----
  StrClear(data, sizeof(data));
  strcpy_P(data, (char *)pgm_read_word(&(table_char[0])));
  nhapeep(data, 31, 50, strlen(data));
  //-----mqttuser-----
  StrClear(data, sizeof(data));
  strcpy_P(data, (char *)pgm_read_word(&(table_char[1])));
  nhapeep(data, 51, 70, strlen(data));
  //-----mqttpass-----
  StrClear(data, sizeof(data));
  strcpy_P(data, (char *)pgm_read_word(&(table_char[2])));
  nhapeep(data, 71, 90, strlen(data));
  //----server's IP----
  StrClear(data, sizeof(data));
  strcpy_P(data, (char *)pgm_read_word(&(table_byte[4])));
  nhapeep(data, 19, 22, 4);
  //-----username-----
  StrClear(data, sizeof(data));
  strcpy_P(data, (char *)pgm_read_word(&(table_char[3])));
  nhapeep(data, 111, 130, strlen(data));
  //------password-----
  StrClear(data, sizeof(data));
  strcpy_P(data, (char *)pgm_read_word(&(table_char[4])));
  nhapeep(data, 131, 150, strlen(data));
  //------chedo---------
//  StrClear(data, sizeof(data));
//  strcpy_P(data, (char *)pgm_read_word(&(table_char[5])));
//  nhapeep(data, 171, 190, strlen(data));
  //
  EEPROM.write(0, '0');
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  delay(1000);
  asm volatile ( "jmp 0");
}

//--------------    LOAD VAULE FROM EPPROM    ----------------------
void confing_value() {
  byte temp[4];
  //IP
  doceep(temp, 1, 4, 4);
  ip = temp;
  StrClear(temp, 4);
  //GATEWAY
  doceep(temp, 5, 8, 4);
  gateway = temp;
  StrClear(temp, 4);
  //SUBNET
  doceep(temp, 9, 12, 4);
  subnet = temp;
  StrClear(temp, 4);
  //IP's Server
  doceep(temp, 23, 26, 4);
  myDns = temp;
  StrClear(temp, 4);
  //IP's Server
  doceep(temp, 19, 22, 4);
  server_ip = temp;
  StrClear(temp, 4);
  //chedo
  //doceep(mode_eeprom, 171, 190, sizeof(mode_eeprom)); 
  //MAC
  doceep(mac, 13, 18, sizeof(mac));
  //deivceID
  doceep(deviceID, 31, 50, sizeof(deviceID));
  //mqttuser
  doceep(mqttuser, 51, 70, sizeof(mqttuser));
  //mqttpass
  doceep(mqttpass, 71, 90, sizeof(mqttpass));
  //subscribe_topic
  //doceep(subscribe_topic, 91, 110, sizeof(subscribe_topic));
  copy_to(deviceID, publish_topic, '\0');
  copy_to(deviceID, subscribe_topic, '\0');
  copy_to("-FEEDBACK", &publish_topic[strlen(publish_topic)], '\0');
  copy_to("-CONTROL", &subscribe_topic[strlen(subscribe_topic)], '\0');
  Serial.println(deviceID);
  Serial.println(mqttuser);
  Serial.println(mqttpass);
  Serial.println(subscribe_topic);
  Serial.println(publish_topic);
  //start the Ethernet connection and the server:
  Serial.println(ip);
  Serial.println(gateway);
  Serial.println(subnet);
  Serial.println(myDns);
  Serial.println(server_ip);

  //debug
  for(int i = 0; i < 6; i++) {
    Serial.print(mac[i], HEX);
    Serial.print("-");
  }
  ///////
  //  Ethernet.begin(mac);
   // w5500.swReset();
  delay(3);
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  Serial.print("server is at ");
  pubsub_client.setServer(server_ip, 1883);
  pubsub_client.setCallback(callback);
  pubsub_client.setClient(mqttClient);
  server.begin();
  Serial.println(Ethernet.localIP());
  sign_in_server();
}

bool create_server() {
  bool value_logout = 0;
  server_client = server.available();             // KIEM TRA CO CLIENT YEU CAU HAY KO
  if (server_client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (server_client.connected()) {
      if (server_client.available()) {
        char c = server_client.read();
        if (bufferSize < 200) header[bufferSize++] = c;       // READ header
        if (c == '\n' && currentLineIsBlank) {
          
          //parse headers
          Serial.print("header: ");
          Serial.println(header);
          
          // CHECK PASSWORD
          if (check_password() != 0) {
            //
            char response [35];
            byte temp_ip[4];
            byte count = 0;
            //
            while (server_client.available())
            {
              
              //--------------- đọc response-------------------------
              char c = (char)server_client.read();           // DOC TUNG KI TU CHO DEN KHI GAP KI TU & HOAC DU 35 KI TU
              //Serial.println(response);
              if (c == '&' || bufferSize >= 35) {
                Serial.println(response);
                switch (count) {
                  case 1  :
                    if (strstr(response, "deviceID")) {             // NEU BIEN DEVICEID BI THAY DOI THI DOC VA LUU VAO EPPROM
                      if (strlen(strchr(response, '=')) > 1){   
                        nhapeep((strchr(response, '=')) + 1, 31, 50, strlen(strchr(response, '=')));
                      }
                    }
                    //
                    else if (strstr(response, "username")) {
                      if (strlen(strchr(response, '=')) > 1){   
                        nhapeep((strchr(response, '=')) + 1, 111, 130, strlen(strchr(response, '=')));
                      }
                    }
                    break;
                  //
                  case 8  :
                    if (strstr(response, "mqttuser")) {
                      if (strlen(strchr(response, '=')) > 1)   nhapeep((strchr(response, '=')) + 1, 51, 70, strlen(strchr(response, '=')));
                    }
                    break;
                  //
                  case 9  :
                    if (strstr(response, "mqttpass")) {
                      if (strlen(strchr(response, '=')) > 1)   nhapeep((strchr(response, '=')) + 1, 71, 90, strlen(strchr(response, '=')));
                    }
                    break;
                  //
                  case 7  :
                    if (strstr(response, "server")) {
                      if (strlen(strchr(response, '=')) > 1) {
                        tach_ip(temp_ip, strchr(response, '='));
                        nhapeep(temp_ip, 19, 22, 4);
                      }
                    }
                    break;
                  //
                  case 2  :
                    if (strstr(response, "ip")) {
                      if (strlen(strchr(response, '=')) > 1) {
                        tach_ip(temp_ip, strchr(response, '='));
                        nhapeep(temp_ip, 1, 4, 4);
                      }
                    }
                    else if (strstr(response, "password")) {
                      if (strlen(strchr(response, '=')) > 1)   nhapeep((strchr(response, '=')) + 1, 131, 150, strlen(strchr(response, '=')));
                    }
                    break;
                  //
                  case 5  :
                    if (strstr(response, "myDns")) {
                      if (strlen(strchr(response, '=')) > 1) {
                        tach_ip(temp_ip, strchr(response, '='));
                        nhapeep(temp_ip, 23, 26, 4);
                      }
                    }
                    break;
                  //
                  case 4  :
                    if (strstr(response, "gateway")) {
                      if (strlen(strchr(response, '=')) > 1) {
                        tach_ip(temp_ip, strchr(response, '='));
                        nhapeep(temp_ip, 5, 8, 4);
                      }
                    }
                    break;
                  case 3  :
                    if (strstr(response, "subnet")) {
                      if (strlen(strchr(response, '=')) > 1) {
                        tach_ip(temp_ip, strchr(response, '='));
                        nhapeep(temp_ip, 9, 12, 4);
                      }
                    }
                    break;
                  //
                  case 6  :
                    if (strstr(response, "MAC")) {
                      if (strlen(strchr(response, '=')) > 1) {
                        tach_mac(mac, strchr(response, '='));
                        nhapeep(mac, 13, 18, 6);
                      }
                    }
                    break;
                }
                //////////////////
                count++;
                StrClear(response, sizeof(response));
                bufferSize = 0;
              }
      //------------- ĐỌC QUY DOI CAC KÍ TU DAC BIET ,VD @=%40/////   // CHUYEN CAC KY TU DAC BIET TU MA HEX SANG CHAR
              if ( c == '%') {
                int j = 0;
                c = (char)server_client.read();
                if ( c > 47 && c < 58) j = ((c - 48) * 16);
                else j = ((c - 55) * 16);
                c = (char)server_client.read();
                if ( c > 47 && c < 58) j += ((c - 48));
                else j += ((c - 55));
                c = j;
              }
      //-------------- CHECK BUTTON ở trang config (html)-------------------------
              response[bufferSize++] = (char)c;
              if (strstr(response, "logout")) {
                server_client.println("HTTP/1.1 401 Unauthorized");
                value_logout = 1;
              }
              else if (strstr(response, "savevalue"))   // CHECK BUTTON SAVEVALUE
              {
                server_client.println("HTTP/1.1 401 Unauthorized");
                //get_reset_flat=1;
                server_client.stop();
                TCCR1A = 0;   // TAT HET NGOAI VI
                TCCR1B = 0;
                TIMSK1 = 0;
                delay(1000);
                asm volatile ("jmp 0");  // RESET
              }
            }

            //------------CHECK AND LOAD WEB-----------------------
            if (strstr(header, "/ HTTP/1.1")) {  // LOAD TRANG CONFIG
              confing_server();
            }
            else if (strstr(header, "/pass HTTP/1.1")) {   // LOAD TRANG PASSWORD CHANGE
              change_pass_server ();
            }
          }
          else {
            // wrong user/pass
            sign_in_server ();
          }
          server_client.stop();
          bufferSize = 0;
          StrClear(header, 500);
          // break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    Serial.println("client disconnected");
    if (value_logout == 1) {
      Serial.println("Aa");
      sign_in_server ();
      return 1;
    }
    else return 0;
  }
}

//-------------    CHECK PASSWORD SIGN IN    -------------------
bool check_password () {
  char pass_temp[41] = "";
  for (int i = 0 ; i <= 20; ++i)
  {
    if (EEPROM.read(111 + i) == '\0') {
      break;
    }
    pass_temp[i] = EEPROM.read(111 + i);
  }
  pass_temp[strlen(pass_temp)] += ':';
  for (int i = 0 ; i <= 41; ++i)
  {
    if (EEPROM.read(131 + i) == '\0') {
      break;
    }
    pass_temp[strlen(pass_temp)] = EEPROM.read(131 + i);
  }
  Serial.print("pass_temp:");
  Serial.println(pass_temp);
  int encodedLen = Base64.encodedLength(strlen(pass_temp));
  char encoded[encodedLen];
  Base64.encode(encoded, pass_temp, strlen(pass_temp));
  Serial.println(encoded);
  if (strstr(header, encoded) != NULL) return 1;
  else return 0;
}

////// CALCUTATOR IP FORMAT 0.0.0.0
void tach_ip(byte* ip, char* aray) {
  for (int i = 0; i < 4; ++i) {
    aray++;
    char temp [3] = "";
    byte j = 0;
    while ((*aray != '.') && (*aray != '\0') && j < 4) {
      temp[j] = *aray;
      j++;
      aray++;
    }
    *ip = atoi(temp);
    if (i < 3) ip++;
  }
}

////// CALCUTATOR MAC FORMAT FROM HEX TO DEC  12-DC-CF-32-14-DA
void tach_mac(byte* ip, char* aray) {
  for (int i = 0; i < 6; ++i) {
    aray++;
    char temp [2] = "";
    byte j = 0;
    while ((*aray != '-') && (*aray != '\0') && j < 2) {
      temp[j] = strupr(*aray);
      j++;
      aray++;
    }
    j = 0;
    if ( temp[0] > 47 && temp[0] < 58) j = ((temp[0] - 48) * 16);
    else j = ((temp[0] - 55) * 16);
    if ( temp[1] > 47 && temp[1] < 58) j += (temp[1] - 48);
    else j += (temp[1] - 55);
    *ip = j;
    if (i < 5) ip++;
  }
}

/////////////////////   WRITE EPPROM     ///////////////////
void nhapeep(char* ten, int dau, int cuoi, int sizechar) {
  Serial.println("clearing eeprom");
  for (int i = 0; i <= (cuoi - dau); ++i)
  {
    if (i < sizechar) {
      EEPROM.write(dau + i, ten[i]);
    }
    else EEPROM.write(dau + i, '\0');
  }
}

//-----------   LOAD WEB CONFIGN   ---------------
void confing_server() {
  int len = strlen_P(web_config);
  byte count_place = 0;
  for (int k = 0; k < len; k++)
  {
    char myChar =  pgm_read_byte_near(web_config + k);
    if (myChar == '~') {
      switch (count_place) {
        case 0:   server_client.print(deviceID);         break;        //  deviceID
        case 1:   server_client.print(ip);               break;        //  ip
        case 2:   server_client.print(subnet);           break;        //  subnet
        case 3:   server_client.print(gateway);          break;        //  gateway
        case 4:   server_client.print(myDns);            break;        //  myDns
        case 5:   for (int i = 0; i < 6; i++) {
            server_client.print(mac[i], HEX);
            if (i < 5) server_client.print('-');
          }
          break;
        case 6:   server_client.print(server_ip);        break;        //  server
        case 7:   server_client.print(mqttuser);         break;        //  mqttuser
        //        case 8:   client.print(mqttpass);         break;        //  mqttpass
        //        case 9:   client.print(username);         break;        //  username
        //        case 3:   client.print(subscribe_topic);  break;      //  subscribe_topic
        default:  Serial.println("da vao 5");      break;
      }
      count_place++;
    }
    else server_client.print(myChar);
  }
}

//----------------  LOAD WEB CHANGE PASSWORD   -----------------
void change_pass_server () {
  int len = strlen_P(web_change_password);
  for (int k = 0; k < len; k++)
  {
    char myChar =  pgm_read_byte_near(web_change_password + k);
    server_client.print(myChar);
  }
}

//--------------------  LOAD WEB SIGN IN   -------------------
void sign_in_server () {
  int len = strlen_P(web_sign_in);
  for (int k = 0; k < len; k++)
  {
    char myChar =  pgm_read_byte_near(web_sign_in + k);
    server_client.print(myChar);
  }
}

//----------- sets every element of str to 0 (clears array)-------------------
void StrClear(char *str, char size_buf)
{
  for (int i = 0; i < size_buf; i++) {
    str[i] = '\0';
  }
}

//----------------   READ EPPROM   ------------------
void doceep(char* ten, int dau, int cuoi, int sizechar) {
  for (int i = 0 ; i <= (cuoi - dau); ++i)
  {
    if (i < sizechar) {
      ten[i] = EEPROM.read(dau + i);
    }
    else break;
  }
}

//---------------   RECONNECT  ------------------
void reconnect() {
  while (!pubsub_client.connected()) {
    //    byte i;
    Ethernet.begin(mac, ip, myDns, gateway, subnet);
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (pubsub_client.connect(publish_topic, mqttuser, mqttpass)) {
      Serial.println("MQTT connected");
      pubsub_client.subscribe(subscribe_topic);
      return;
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(pubsub_client.state());
      Serial.println(" try again in 5 seconds");
    }
    if (Ethernet.localIP() != ip) {
      Serial.println("Attempting Ethernet connection...");
      server.begin();
      delay(2000);
      // server.begin();
    }
    else {
      for (int i = 0; i <= 10; i++) {
        create_server();
        delay(500);
      }
    }
  }
}

//--------------------   copy_to  ------------------
void copy_to(char *str_f, char *str_t, char c)
{
  char *p = str_f;
  while ((*p != c) & (*p != '\0')) {
    *str_t = *p;
    str_t++;
    p++;
    *str_t = '\0';
  }
}

//------------- callback   ------------------------
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  StaticJsonDocument<200> recieve;
  DeserializationError error = deserializeJson(recieve, payload);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  String chedo = recieve["chedo"];
  String bomsuong = recieve["bomsuong"];
  String bomgianmua = recieve["bomgianmua"];
  String gianhiet = recieve["gianhiet"];
  String den = recieve["den"];
  String quatgio = recieve["quatgio"];
  String quatday = recieve["quatday"];
  Serial.print("bomsuong: ");   
  Serial.println(bomsuong);
  Serial.print("che do: ");   
  Serial.println(chedo);
  CHEDO = chedo;
  if(bomsuong.indexOf("on") != -1) digitalWrite(BOM_SUONG, HIGH);
  if(bomsuong.indexOf("off") != -1) digitalWrite(BOM_SUONG, LOW);
  if(bomgianmua.indexOf("on") != -1) digitalWrite(BOM_GIANMUA, HIGH);
  if(bomgianmua.indexOf("off") != -1) digitalWrite(BOM_GIANMUA, LOW);
  if(gianhiet.indexOf("on") != -1) digitalWrite(GIANHIET, HIGH);
  if(gianhiet.indexOf("off") != -1) digitalWrite(GIANHIET, LOW);
  if(den.indexOf("on") != -1) digitalWrite(DEN, HIGH);
  if(den.indexOf("off") != -1) digitalWrite(DEN, LOW);
  if(quatgio.indexOf("on") != -1) digitalWrite(QUAT_GIO, HIGH);
  if(quatgio.indexOf("off") != -1) digitalWrite(QUAT_GIO, LOW);
  if(quatday.indexOf("on") != -1) digitalWrite(QUAT_DAY, HIGH);
  if(quatday.indexOf("off") != -1) digitalWrite(QUAT_DAY, LOW); 
}  

//--------------- Switch ------------------ nút nhấn điều khiển thiết bị
void Switch(){  
  //Serial.println("vao switch");
  if(digitalRead(SW_BOM_SUONG) == LOW){
    delay(10);
    while(digitalRead(SW_BOM_SUONG) == LOW);
    if(digitalRead(BOM_SUONG) == LOW){
      digitalWrite(BOM_SUONG, HIGH);
    }else digitalWrite(BOM_SUONG, LOW);
  }
  if(digitalRead(SW_BOM_GIANMUA) == LOW){
    delay(10);
    while(digitalRead(SW_BOM_GIANMUA) == LOW);
    if(digitalRead(BOM_GIANMUA) == LOW){
      digitalWrite(BOM_GIANMUA, HIGH);
    }else digitalWrite(BOM_GIANMUA, LOW);
  }
  if(digitalRead(SW_GIANHIET) == LOW){
    delay(10);
    while(digitalRead(SW_GIANHIET) == LOW);
    if(digitalRead(GIANHIET) == LOW){
      digitalWrite(GIANHIET, HIGH);
    }else digitalWrite(GIANHIET, LOW);
  }
  if(digitalRead(SW_DEN) == LOW){
    delay(10);
    while(digitalRead(SW_DEN) == LOW);
    if(digitalRead(DEN) == LOW){
      digitalWrite(DEN, HIGH);
    }else digitalWrite(DEN, LOW);
  }
  if(digitalRead(SW_QUATGIO) == LOW){
    delay(10);
    while(digitalRead(SW_QUATGIO) == LOW);
    if(digitalRead(QUAT_GIO) == LOW){
      digitalWrite(QUAT_GIO, HIGH);
    }else digitalWrite(QUAT_GIO, LOW);
  }
  Serial.print("debug bomsuong: ");
  Serial.println(digitalRead(BOM_SUONG));
  Serial.print("debug bomgianmua: ");
  Serial.println(digitalRead(BOM_GIANMUA));
  Serial.print("debug gianhiet: ");
  Serial.println(digitalRead(GIANHIET));
  Serial.print("debug den: ");
  Serial.println(digitalRead(DEN));
  Serial.print("debug quatgio: ");
  Serial.println(digitalRead(QUAT_GIO));
}

//------------- publish data ----------------------
void pub_data(){
  StaticJsonDocument<200> transmitJson;
  transmitJson["Mode"] = CHEDO;
  //transmitJson["Mode"] = check_mode;
  digitalRead(BOM_SUONG) == HIGH? transmitJson["Bomsuong"] = "on": transmitJson["Bomsuong"] = "off";
  digitalRead(BOM_GIANMUA) == HIGH? transmitJson["Bomgianmua"] = "on": transmitJson["Bomgianmua"] = "off";
  digitalRead(GIANHIET) == HIGH? transmitJson["Gianhiet"] = "on": transmitJson["Gianhiet"] = "off";
  digitalRead(DEN) == HIGH? transmitJson["Den"] = "on": transmitJson["Den"] = "off";
  digitalRead(QUAT_GIO) == HIGH? transmitJson["Quatgio"] = "on": transmitJson["Quatgio"] = "off";
  digitalRead(QUAT_DAY) == HIGH? transmitJson["Quatday"] = "on": transmitJson["Quatday"] = "off";
  char transmit[250]; 
  serializeJson(transmitJson, transmit);
  
  if((String)transmit != chuoi){
    pubsub_client.publish(publish_topic, transmit);
    Serial.print("MQTT published: ");
    Serial.println(transmit);
  }
  chuoi = transmit;
  delay(100);
}
