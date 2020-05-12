// TUTORIAL REGAR PLANTAS DESDE INTERNET CON ESP8266, FIREBASE Y APPINVENTOR.
// http://suarezdefigueroa.es/meteohuerto/
// https://youtu.be/Jw1j8ZlIV2I 
// Creado a partir de: https://www.youtube.com/watch?v=jW1j-l2Jr10&t
// Autor: J. Enrique Agudo

#include "DHTesp.h"

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

#include <ESP8266WiFi.h>

#include <FirebaseArduino.h>



// Definir Firebase e internet..


#define FIREBASE_HOST "pon tu direccion del proyecto" // Poner la direccion del proyecto Firevase Sin "http://" y sin "/" final.

#define FIREBASE_AUTH "codigo del proyacto" // buscar en la página de Firebase. Configuracion del proyecto Cuentas de servicio Secretos base de datos- mostrar- copiar y pegar aqui con las comillas.

#define WIFI_SSID "SSID" // nombre del wifi

#define WIFI_PASSWORD "PASSWORD" // contraseña del wifi






//Las variables a utilizar.
DHTesp dht;
int  humedad;



String  interruptor;




void ICACHE_RAM_ATTR ISRoutine ();

void setup() {

  Serial.begin(9600);



 



pinMode(A0, INPUT); //Aqui conectaremos el pin de control del sensor de humedad.

pinMode(12, OUTPUT); // conectamos al relé.(13 =D7)

  
dht.setup(4, DHTesp::DHT11); //GPIO4 - D2


  // conectamos al wifi.

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {

    Serial.print(".");

    delay(500);

  }

  Serial.println();

  Serial.print("connected: ");

  Serial.println(WiFi.localIP());

  

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}







void loop() {

  

  humedad = (100 - map(analogRead(A0),0,1023,0,100)); // Con esto conseguimos el porcentage de humedad en tantos por cien (el 1.45 puede variar segun el sensor de humedad).





  // Monitorizar los parametros.

  

  Serial.print(humedad);Serial.println (" %");
  Serial.print("humedad ambiental: "); Serial.println(dht.getHumidity());
  Serial.print("temperatura: "); Serial.println(dht.getTemperature());
 

  //Esto crea una etiqueta en firebase y le asigna el valor

  

  Firebase.setInt("humedadSuelo", humedad); // entre parentesis primero se pone la etiqueta entre comillas, despues una "," y despues el valor que se le asigna a la etiqueta.
  Firebase.setFloat("temperatura",dht.getTemperature());
  Firebase.setFloat("humedadAire",dht.getHumidity());

  // handle error

  if (Firebase.failed()) {

      Serial.print("setting /number failed:");

      Serial.println(Firebase.error());  

      return;

  }

  //delay(1000);





  

//Coger datos



 interruptor = Firebase.getString("regar"); // Para encender o apagar el motor.


 

 

// Acciones



     //Cuando pulsamos boton On se encenderá el motor.

    if ((interruptor.toInt() == 1)) {

      digitalWrite(12, HIGH);

      }

     //Cuando pulsamos boton Off se apagará el motor.

    if ((interruptor.toInt() == 0)) {

      digitalWrite(12, LOW);}



 } 
