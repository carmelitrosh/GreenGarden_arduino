#include <ESP32_Servo.h>
#include <analogWrite.h>
#include <FirebaseESP32.h>
#include <FirebaseESP32HTTPClient.h>
#include <FirebaseJson.h>
#include <jsmn.h>
#include <DHT.h>

// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 4
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11

// Definimos la librerias par la conexion a nuestro servidor
#include "FirebaseESP32.h"

// Definimos la direccion host de nuestro servidor
#define FIREBASE_HOST "greengardern-2ddc5.firebaseio.com"
// Definimos el token de nuestro servidor
#define FIREBASE_AUTH "YXzMUFMCVXom6v7MI2WWOXG5DWuumtC5Jm8Lmhww"

// Definimos la conexion a punto de acceso wifi
#define WIFI_SSID "INFINITUM7385_2.4"
#define WIFI_PASSWORD "Xz6CtRYtL8"

WiFiClient client;
FirebaseData firebaseLed;
FirebaseData firebaseServo;
FirebaseData firebaseRiego;
FirebaseData firebaseVentilador;
FirebaseData firebaseDataTemperature;
FirebaseData firebaseDataHumidity;

// Inicializamos el sensor
DHT dht(DHTPIN, DHTTYPE);

//definimos el pin para el foco
const int relay = 2;

//definimos el pin para el servo
Servo servoMotor;

//definimos el pin para el riego
const int relayriego = 5;

//definimos el pin para el ventilador
//#define ventilador 18



void setup() {
  Serial.begin(9600);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
  }

  Serial.println ("");
  Serial.println ("Se conectó al wifi!");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //Salida del pin de foco
  pinMode(relay, OUTPUT);

  //Salida del servo
  servoMotor.attach(19);

  //Salida del pin riego
  pinMode(relayriego, OUTPUT);

  //Salida del pin ventilador
  //pinMode(ventilador, OUTPUT);

  //Salida del sensor
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  
  dht.begin();
}

void loop() {

  
  //Acciones del pin foco
  Firebase.getInt(firebaseLed,"/led");
  int datoled=firebaseLed.intData();  
  digitalWrite(relay, datoled);
  Serial.println(datoled);

  //Acciones del servo
  Firebase.getInt(firebaseServo,"/puerta");
  int datoservo=firebaseServo.intData();
  servoMotor.write(datoservo);
  Serial.println(datoservo);

  
  //Acciones del pin riego
  Firebase.getInt(firebaseRiego,"/riego");
  int datoriego=firebaseRiego.intData();  
  digitalWrite(relayriego, datoriego);
  Serial.println(datoriego);

  /*
  //Acciones del pin ventilador
  Firebase.getInt(firebaseVentilador,"/ventilador");
  int datoventilador=firebaseVentilador.intData();
  analogWrite(ventilador, datoventilador);
  Serial.println(datoventilador);*/

  //Acciones del sensor 
  float t = dht.readTemperature();
  int h = dht.readHumidity();
  Firebase.setFloat(firebaseDataTemperature,"temperature", t);  
  Firebase.setInt(firebaseDataHumidity,"humidity", h);
}
