
/* 
This header allow you to make:
- sent "AT" command that need tho know if is all wright connected---> void sendATcommand(),
- communicate as a normal Serial with a SoftwareSerial ports after included SoftwareSerial.h to ESP8266-->void writeAndRead(),
- send "CWLAP" command that return the list of wifi availables-->sendCWLAPcommand(),
- connect to Wifi with a name of wifi and password --> int connectToWifi(String ssid, String password)=>return 1 if the connection is stable,
- connect to website/webserver with domain-->int HTTPconnectToDomain(String domain)=>return 1 if the connection is stable,
- get the connection status that is 5: search online connection status ESP8266 -> char connectionStatus()=> return the char status of the connection 1 to 5 
- send http GET request that must start with a "/address/ad...", name of the host for example 192.168.1.1 and a user agent that you want, for
example I utilized "wget/1.12" and it works  -> void sendHTTPGETrequest(String GetRequest, String Host, String UserAgent)

NB. First to utilize "sendHTTPGETrequest" you must onnected on wifi, and after on domain.

Example:

//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SoftwareSerial.h"
#include "ESP8266HTTPGet_Library.h"

SoftwareSerial ESP8266(2,4);

void setup()
{
  Serial.begin(9600);
  Serial.println("Start WIFI");
  ESP8266.begin(9600);
  Serial.println("Sensor Start");
  pinMode(8, INPUT); 
  
  if(connectToWifi("name","password") == 1)
  {
    Serial.println("Connection to Wifi");    
   }

   char statusWifi = connectionStatus();
   Serial.println(statusWifi);
}

void loop()
{ 
  int connectedDomain = HTTPconnectToDomain("192.168.1.1");  //Insert your domain

  if (connectedDomain ==1)
  {
    Serial.println("Connected");
    sendHTTPGETrequest("/address/page.php","192.168.0.111","wget/1.12"); //Insert your values
    delay(1000);  
  } 

 }
//////////////////////////////////////////////////////////////////////////////////////////////////////
*/

extern SoftwareSerial ESP8266;



void sendATcommand(){
  String inData;
  ESP8266.print("AT\r\n");
  delay(300);
  while (ESP8266.available()){
     inData = ESP8266.readStringUntil('\n');
  } 
  Serial.println("Got reponse from ESP8266: " + inData);
}

void sendCWLAPcommand(){
  String inData;
  ESP8266.print("AT+CWLAP\r\n");
  delay(300);
  while (ESP8266.available()){
     inData = ESP8266.readStringUntil("OK");
  } 
  Serial.println("Got reponse from ESP8266: " + inData);
  }

void writeAndRead(){
  if (ESP8266.available()) {
    Serial.write(ESP8266.read());
  }
  if (Serial.available()) {
    ESP8266.write(Serial.read());
  }
 }

 int connectToWifi(String ssid, String password){
  String inData;
  int connection;
  ESP8266.print("AT+CWJAP_CUR=\"" + ssid + "\"," + "\"" + password + "\"\r\n");
  delay(300);
  int control = 0;
  
  while (control == 0){
  
  inData = ESP8266.readStringUntil("K");
  if (inData == 'K'){
    Serial.println("Connected");
    control = 1;
    connection = 1;
    break;
    } else {
     Serial.println("Connecting...");
     }
  } 
  return connection;
  
 }

 int HTTPconnectToDomain(String domain){
  String inData;
  ESP8266.print("AT+CIPSTART=\"TCP\",\"" + domain + "\"," + "80\r\n");
  delay(300);
  int control = 0;
  int connection = 0;
  
  while (control == 0){
  
  inData = ESP8266.readStringUntil("K");
  if (inData == 'K'){
    control = 1;
    connection = 1;
    break;
    } 
  } 
  return connection;
  }

 
 char connectionStatus(){
  String inData;
  ESP8266.print("AT+CIPSTATUS\r\n");
  delay(300);
  if(ESP8266.available()>0){
    inData = ESP8266.readStringUntil("\n");
   }
   return inData[22];
  }

 
 void sendHTTPGETrequest(String GetRequest, String Host, String UserAgent){
  String inData;
  int numberOfChar = GetRequest.length() + Host.length() + UserAgent.length() + 15 + 8 + 16;
  String numberOfCharRequest = String(numberOfChar);
  ESP8266.println("AT+CIPSEND=" + numberOfCharRequest);
  delay(2000);
  ESP8266.println("GET " +GetRequest + " HTTP/1.1");
  ESP8266.println("Host: "+Host);
  ESP8266.println("User-Agent: "+UserAgent);  
  ESP8266.println();

 int control = 0;
  
 while (control == 0){
  if (ESP8266.available()>0){
    inData = ESP8266.readString();
  } else {
    control = 1;
    }
  }
  Serial.print(inData);
 }

  
