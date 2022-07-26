#include "DHT.h"
#include "config.h"
#include <Adafruit_Sensor.h>
#define DHT11PIN 16
#define inDHT11PIN 19

DHT dht(DHT11PIN, DHT11);
DHT indht(inDHT11PIN, DHT11);


AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *humidity = io.feed("humidity");

AdafruitIO_Feed *intemperature = io.feed("intemperature");
AdafruitIO_Feed *inhumidity = io.feed("inhumidity");

void setup()
{
  
  Serial.begin(115200);
/* Start the DHT11 Sensor */
  while(! Serial);

  // initialize dht22
  dht.begin();
  indht.begin();
  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  Serial.println();
  Serial.println(io.statusText());
}
}

void loop()
{
  io.run();



  
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  float inhumi = indht.readHumidity();
  float intemp = indht.readTemperature();

   float celsius = temp;
  float fahrenheit = (celsius * 1.8) + 32;
  
    float incelsius = temp;
  float infahrenheit = (incelsius * 1.8) + 32;

  Serial.print("celsius: ");
  Serial.print(celsius);
  Serial.println("C");

  Serial.print("fahrenheit: ");
  Serial.print(fahrenheit);
  Serial.println("F");

  inhumidity->save(inhumi);
  intemperature->save(infahrenheit);

  Serial.print("humidity: ");
  Serial.print(humi);
  Serial.println("%");
  
  humidity->save(humi);
  delay(5000);
}