#include <Wire.h>
#include <ESP8266WiFi.h>
#include <HTTPSRedirect.h>
#include <Keypad.h>
#include <Keypad_I2C.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C (0x25, 16, 4);

#define I2CADDR 0x20
#define WaterLevel D8
#define relay1 D6
#define relay2 D7
#define buzzer D2

// Enter network credentials:
const char* ssid     = "";
const char* password = "";

const char *GScriptId = "AKfycbxkhO3xSSmJduwIi4Z6O323FzgC47_JwebBV9ys-S_QDJRry5HUa7unOrL967T5IU03";
const char *cellAddress1 = "B2";
const char *cellAddress2 = "B3";
const char *cellAddress3 = "B4";
const char *cellAddress4 = "B5";

String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";

String url1 = String("/macros/s/") + GScriptId + "/exec?read=" + cellAddress1;
String url2 = String("/macros/s/") + GScriptId + "/exec?read=" + cellAddress2;
String url3 = String("/macros/s/") + GScriptId + "/exec?read=" + cellAddress3;
String url4 = String("/macros/s/") + GScriptId + "/exec?read=" + cellAddress4;

String payload = "";
String payload1 = "";
String payload2 = "";
String payload3 = "";
String payload4 = "";

const char* host = "script.google.com";
const int httpsPort = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;

int value0 = 0;
int value1 = 0;
int value2 = 0;

bool data_published = false;
int error_count = 0;

const byte ROWS = 4;
const byte COLS = 4;
 
char keys[COLS][ROWS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
 
byte colPins[COLS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte rowPins[ROWS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad
 
Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );

void setup() {
  lcd.begin();
  Wire.begin( );
  kpd.begin( makeKeymap(keys) );
  Serial.begin(115200);        
  delay(10);
  Serial.println('\n');
  //lcd.init();
  lcd.clear();
        lcd.setCursor(3, 0);
  lcd.print("Hidroponik");
  Serial.println( "start" );
  pinMode(WaterLevel, INPUT);
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, LOW);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, LOW);
  pinMode(buzzer, OUTPUT);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);             
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("Connecting to");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  
  Serial.print("Connecting to ");
  Serial.println(host);

  // Try to connect for a maximum of 5 times
  bool flag = false;
  for (int i=0; i<5; i++){ 
    int retval = client->connect(host, httpsPort);
    if (retval == 1){
       flag = true;
       Serial.println("Connected");
       lcd.clear();
       lcd.setCursor(0, 2);
       lcd.print("Connected");
       break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    return;
  }
  delete client;    
  client = nullptr; 
}

void loop() {
     String value2 = "";
    if (digitalRead(WaterLevel)) {
     value2 = "Nutrisi Kosong";
     Serial.println("Nutrisi Kosong");
     lcd.setCursor(0, 3);
     lcd.print("Nutrisi Kosong");
     digitalWrite(buzzer, HIGH);
     digitalWrite(relay1, HIGH);
    }
    else {
     value2 = "Nutrisi Penuh";
     Serial.println("Nutrisi Penuh");
     lcd.setCursor(0, 3);
     lcd.print("Nutrisi Penuh ");
     digitalWrite(relay1, LOW);
     digitalWrite(buzzer, LOW);
    }
    
  char key = kpd.getKey();
  if (key)  {

    data_published = false;
    error_count = 0;

    value0 ++;
    String value1 = "";
    if (key == '2') {
    lcd.setCursor(3, 0);
    value0 = 2;
    value1 = "Kangkung"; 
    lcd.clear();
    lcd.print("Hidroponik");
    Serial.println("Kangkung");
    lcd.setCursor(0, 2);
    lcd.print("Kangkung");
    //Serial.println("Delay Kangkung = 1 detik");
    }
  
    if (key == '3') {
    value0 = 3;  
    value1 = "Bayam"; 
    lcd.clear();
    lcd.print("Hidroponik");
    Serial.println("Bayam");
    lcd.setCursor(0, 2);
    lcd.print("Bayam");
    //Serial.println("Delay Bayam = 2 detik");
    }
  
    if (key == '5') {
    value0 = 5;
    value1 = "Tauge"; 
    Serial.println("Tauge");
    lcd.clear();
    lcd.print("Hidroponik");
    Serial.println("Tauge");
    lcd.setCursor(0, 2);
    lcd.print("Tauge");
    //Serial.println("Delay Tauge = 3 detik");
    }
  
    if (key == '6') {
    value0 = 6;
    value1 = "Saicim"; 
    Serial.println("Saicim");
    lcd.clear();
    lcd.print("Hidroponik");
    Serial.println("Saicim");
    lcd.setCursor(0, 2);
    lcd.print("Saicim");
    //Serial.println("Delay Saicim = 4 detik");
    }

    if (key == 'A') {
    value0 = 'A';
    value1 = "Kangkung";
    value2 = "Pompa Menyala"; 
    Serial.println("Kangkung");
    lcd.clear();
    lcd.print("Hidroponik");
    Serial.println("Kangkung");
    lcd.setCursor(0, 2);
    lcd.print("Kangkung");
    lcd.setCursor(0, 3);
    lcd.print("Pompa Menyala");
    digitalWrite(relay2, HIGH);
    //digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(relay2, LOW);
    //digitalWrite(buzzer, LOW);
    }

    if (key == 'B') {
    value0 = 'B';
    value1 = "Bayam";
    value2 = "Pompa Menyala"; 
    Serial.println("Bayam");
    lcd.clear();
    lcd.print("Hidroponik");
    Serial.println("Bayam");
    lcd.setCursor(0, 2);
    lcd.print("Bayam");
    lcd.setCursor(0, 3);
    lcd.print("Pompa Menyala");
    digitalWrite(relay2, HIGH);
    //digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(relay2, LOW);
    //digitalWrite(buzzer, LOW);
    }

    if (key == 'C') {
    value0 = 'C';
    value1 = "Tauge";
    value2 = "Pompa Menyala"; 
    Serial.println("Tauge");
    lcd.clear();
    lcd.print("Hidroponik");
    Serial.println("Tauge");
    lcd.setCursor(0, 2);
    lcd.print("Tauge");
    lcd.setCursor(0, 3);
    lcd.print("Pompa Menyala");
    digitalWrite(relay2, HIGH);
    //digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(relay2, LOW);
    //digitalWrite(buzzer, LOW);
    }

    if (key == 'D') {
    value0 = 'D';
    value1 = "Saicim";
    value2 = "Pompa Menyala"; 
    Serial.println("Saicim");
    lcd.clear();
    lcd.print("Hidroponik");
    Serial.println("Saicim");
    lcd.setCursor(0, 2);
    lcd.print("Saicim");
    lcd.setCursor(0, 3);
    lcd.print("Pompa Menyala");
    digitalWrite(relay2, HIGH);
    //digitalWrite(buzzer, HIGH);
    delay(3000);
    digitalWrite(relay2, LOW);
    //digitalWrite(buzzer, LOW);
    }
    
    static int connect_count = 0;
    const unsigned int MAX_CONNECT = 20;
    static bool flag = false;
    if (!flag){
     client = new HTTPSRedirect(httpsPort);
     client->setInsecure();
     flag = true;
     client->setPrintResponseBody(true);
     client->setContentTypeHeader("application/json");
    }
    if (client != nullptr){
     if (!client->connected()){
        client->connect(host, httpsPort);
     }
    }
    else{
     Serial.println("Error creating client object!");
    }

    if (connect_count > MAX_CONNECT){
      connect_count = 0;
      flag = false;
      delete client;
      return;
    }

    if (key == '1') {
    //digitalWrite(relay1, HIGH);
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == '2') {
    //digitalWrite(relay1, LOW);
    if (client->GET(url2, host)){
    //get the value of the cell
      payload2 = client->getResponseBody();
      payload2.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == '3') {
    //digitalWrite(relay1, HIGH);
    if (client->GET(url3, host)){
    //get the value of the cell
      payload3 = client->getResponseBody();
      payload3.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == '4') {
    //digitalWrite(relay1, LOW); 
    if (client->GET(url4, host)){
    //get the value of the cell
      payload4 = client->getResponseBody();
      payload4.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == '5') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == '6') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }
    
    if (key == '7') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == '8') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == '9') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == '0') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }
    
    if (key == 'A') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == 'B') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == 'C') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == 'D') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == '*') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }

    if (key == '#') {
    if (client->GET(url1, host)){
    //get the value of the cell
      payload1 = client->getResponseBody();
      payload1.trim();
      payload = client->getResponseBody();
      payload.trim();
      }
    }
    
    payload = payload_base + "\"" + value0 + "," + value1 + "," + value2 + "\"}";
    Serial.println("Publishing data...");
    if(client->POST(url, host, payload)){ 
     data_published = true;
     Serial.println();
    }
    else{
     Serial.println("Error while connecting");
     error_count++;
     delay(200);
    }
    yield();
  }
}
