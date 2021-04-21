/*Smart Kontrol Ruang Lab Elektronika
Pin yang digunakan

  OUTPUT
  -D1
  -D2
  -D3
  -D4

  INPUT Dengan perangkat Saklar
  -Hubungan Serial (RX/TX)

Menggunakan Telegram sebagai perantara antara perangkat android dengan perangkat laboratorium

*/ 

//List libraries
#include <ESP8266WiFi.h>  /https://github.com/esp8266/Arduino

//Mengatur Wifi lewat webserver/AP
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

//telegram
#include "CTBot.h"

//Inisialisasi Pin NodeMCU
//Pin Output / Relay
#define S0 5 //D1
#define S1 4 //D2
#define S2 14 //D3
#define S3 13 //D4

//Pin Input / Saklar convensional
#define T1 12
#define T2 15

// Variabel untuk menyatakan status dari perangkat 
String a = "";
String b = "";
String c = "";
String d = "";
String AlamatIP = "";
// Variabel penghubung / sinkronisasi antara input dari perangkat manual dengan android

int w=0;
int x=0;
int y=0;
int z=0;

int i=0;

// Variabel penghubung / sinkronisasi antara saklar konvensional dengan software

int saklar1 = 0;
int saklar2 = 0;

// Variabel Saklar Touchless

boolean currentState1 = LOW;
boolean lastState1 = LOW;

boolean currentState2 = LOW;
boolean lastState2 = LOW;

//Token API dari TelegramBot

String token = "TokenBot"; 


// Set web server port number to 80
WiFiServer server(80);


CTBot myBot;

void setup(){
 
  Serial.begin(9600); // Baut rate yang digunakan Serial
  
  //Inisialisasi fungsi pin pada NodeMcu
  pinMode(S0, OUTPUT); 
  pinMode(S1, OUTPUT); 
  pinMode(S2, OUTPUT); 
  pinMode(S3, OUTPUT);

  
  pinMode(T1, INPUT); 
  pinMode(T2, INPUT);
  
  //Inisialisasi Keadaan awal dari Pin yang digunakan
  digitalWrite(S0, HIGH); 
  digitalWrite(S1, HIGH); 
  digitalWrite(S2, HIGH); 
  digitalWrite(S3, HIGH); 

  Serial.println("Starting Setup...");


  // WiFiManager
  WiFiManager WifiSetup;

  // Hilangkan tanda (//) untuk menghapus data wifi yang tersimpan
  //WifiSetup.resetSettings();

  // set custom ip for portal
  //WifiSetup.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  WifiSetup.autoConnect("SmartKontroler Lab");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
    
  // Notifikasi jika terhubung
  Serial.println("OK, Perangkat Terhubung . . .");
  delay (1000);

  // Menautkan Software dengan Bot dari Telegram, jika gagal
  myBot.setTelegramToken(token);
  Serial.println("Menghubungkan dengan Bot");
  if (myBot.testConnection()) {
    Serial.println("Terhubung dengan Bot");
  } else {
    Serial.println("Gagal Menghubungi Bot");
  }
  delay(500);
  Serial.println("Cek IP : ");
  Serial.println(WiFi.localIP());
  AlamatIP = WiFi.localIP().toString();
  delay(1000);
  
}


void loop(){

TBMessage msg;

for (i = 15; i >= 0; i--){
  
  Serial.println("Menunggu Input . . . . ");
  currentState1 = digitalRead(T1);
  currentState2 = digitalRead(T2);    
    if (currentState1 == HIGH && lastState1 == LOW){
      Serial.println("Input dari Saklar1");
      x = x+1;
      }
     
    if (currentState2 == HIGH && lastState2 == LOW){
      Serial.println("Input dari Saklar2");
      y = y+1;
      }
    lastState1 = currentState1;
    lastState2 = currentState2;

  // Looping Status Terkini
  
  /*if (w == 2)
      {w = 0;}
  */   
    
    if ( x == 2)
      {x = 0;}

    if ( y == 2)
      {y = 0;}
    
    if ( z == 2)
      {z = 0;}
    
    Serial.println("Status Terkini : ");  
    if ( w == 0){
      digitalWrite(S3, HIGH);
      Serial.println("Relay Ready");
      }  
    if ( w == 1){
      Serial.println("Relay Triggered");
      w = 0;
      }                           
    if ( x == 0){
      digitalWrite(S0, HIGH);
      Serial.println("Lampu 1 Mati");
      }    
    if ( x == 1){
      digitalWrite(S0, LOW);
      Serial.println("Lampu 1 Menyala"); 
      } 
    if ( y == 0){
      digitalWrite(S1, HIGH);
      Serial.println("Lampu 2 Mati");
      }  
    if ( y == 1){
      digitalWrite(S1, LOW);
      Serial.println("Lampu 2 Menyala");
      }                         
    if ( z == 0){
      digitalWrite(S2, HIGH);
      Serial.println("AC LAB STANDBY");  
      }                            
    if ( z == 1){
      digitalWrite(S2, LOW);
      Serial.println("AC Locked ");
    }
       
  delay(500);
}

if (true){
    
  Serial.println ("Mengupdate Pesan/Perintah Masuk dari Bot");
  
      if (msg.text.equalsIgnoreCase("S1on")) {            
        digitalWrite(S0, LOW);
        Serial.println("Lampu 1 Menyala");
        x = 1;                               
        myBot.sendMessage(msg.sender.id, "Menyalakan Lampu 1 Lab Elka");  
      }
    
      else if (msg.text.equalsIgnoreCase("S1off")) {      
        digitalWrite(S0, HIGH);                              
        Serial.println("Lampu 1 Mati");
        x = 0;
        myBot.sendMessage(msg.sender.id, "Mematikan Lampu 1 Lab Elka"); 
      }
    
      else if (msg.text.equalsIgnoreCase("S2on")) {            
        digitalWrite(S1, LOW);
        Serial.println("Lampu 2 Menyala");                               
        y = 1;
        myBot.sendMessage(msg.sender.id, "Menyalakan Lampu 2 Lab Elka");  
      }
    
      else if (msg.text.equalsIgnoreCase("S2off")) {      
        digitalWrite(S1, HIGH); 
        Serial.println("Lampu 2 Mati");                             
        y = 0;
        myBot.sendMessage(msg.sender.id, "Mematikan Lampu 2 Lab Elka"); 
      }
    
      else if (msg.text.equalsIgnoreCase("ACon")) {            
        digitalWrite(S2, HIGH);
        Serial.println("AC Lab Unloced");                               
        z = 0;
        myBot.sendMessage(msg.sender.id, "Membuka kunci AC Lab Elka");  
      }
              
      else if (msg.text.equalsIgnoreCase("ACoff")) {      
        myBot.sendMessage(msg.sender.id, "Mematikan AC Lab Elka"); 
        Serial.println("Mematikan AC");
        digitalWrite(S2, LOW);
        delay(5000);
        digitalWrite(S2, HIGH);
        Serial.println("AC LAB STANDBY");  
        z = 0;
        myBot.sendMessage(msg.sender.id, "AC Lab Elka Standby");
    
      }

      else if (msg.text.equalsIgnoreCase("AClock")) {            
        digitalWrite(S2, LOW);
        Serial.println("AC Lab Locked - dapat dinyalakan kembali setelah unlock");                               
        z = 1;
        myBot.sendMessage(msg.sender.id, "AC Lab Locked - dapat dinyalakan kembali setelah unlock");  
      }

      else if (msg.text.equalsIgnoreCase("S3on")) {            
        digitalWrite(S3, LOW);
        delay(1000);
        digitalWrite(S3, HIGH);
        Serial.println("Relay on - Remote Triggered");                               
        w = 1;
        myBot.sendMessage(msg.sender.id, "Relay on - Remote Triggered");  
      }
      
      else if (msg.text.equalsIgnoreCase("S3off")) {      
        digitalWrite(S3, LOW);
        delay(1000);
        digitalWrite(S3, HIGH);
        Serial.println("Relay on - Remote Triggered");                               
        w = 1;
        myBot.sendMessage(msg.sender.id, "Relay on - Remote Triggered");
      }
    
      else if (msg.text.equalsIgnoreCase("Cekstatus")) {
      //Membaca Real Time State            
          if (x == 1){ a = "Menyala";}                              
          if (x == 0){ a = "Mati";}
          if (y == 1){ b = "Menyala";}
          if (y == 0){ b = "Mati";}
          if (z == 1){ c = "Locked";}
          if (z == 0){ c = "Standby / ON";}
          if (w == 1){ d = "Active";}
          if (w == 0){ d = "Ready";}
      
          myBot.sendMessage(msg.sender.id, "Status Perangkat");
          myBot.sendMessage(msg.sender.id, "Lampu 1 : " + a); 
          myBot.sendMessage(msg.sender.id, "Lampu 2 : " + b); 
          myBot.sendMessage(msg.sender.id, "AC Ruang : " + c); 
          myBot.sendMessage(msg.sender.id, "Relay : " + d); 
          }
          
        else {                                
        String reply;
        reply = (String)"Hai " + msg.sender.username + (String)". Perintah yang anda masukan tidak terdaftar pada Lab Elka.";
        myBot.sendMessage(msg.sender.id, reply);             // and send it
      }
    }
  return;
  }  
}
