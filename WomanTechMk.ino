/*  @nuria_imeq Nov-2019
 *  Como todos mis proyectos puedes usarlos siempre y cuando me tengas presente, no
 *  solo en tus pensamientos, sino en tu codigo. NO OLVIDES NOMBRAR de donde partio
 *  el proyecto.
 *  Si vas a usar este proyecto para hacer negocio espero que no me entere.
 *  Monitorizacion de los hashtag de un evento en twitter. Se monitoriza con
 *  nodeRed y se envían los datos a servidor MQTT y ThingSpeak.
 *  En un primer momento el proyecto consistia en encender los leds de una camiseta
 *  en el momento de producirse ese hashtag, mediante iteracion con protocolo MQTT.
 *  Lo dejamos pendiente para la proxima version. Por ahora solo usaremos 
 *  un anillo de NeoPixel y solo la plataforma de IoT ThingSpeak.
 *  Primero controlo que los hashtag son diferente para iluminar el neopixel dependiendo
 *  de la cantidad de ellos. Si se produce un hashtag se ilumina el neopixel con los
 *  colores de Woman Tech Maker y dependiendo del numero de hashtag terminaremos
 *  iluminando el anillo.
 *  Por ahora es así, tengo pendiente en contralar si el evento es nuevo o si ya tenemos 
 *  datos en ThingSpeak, es decir, hasta ahora detectar si se ha producido un cambio
 *  lo hago un la variable hashTag y hashTagThingSpeak. Debería leer primero si hay 
 *  Definicion de la libreria Adafruit Neopixel sacado de la documentacion.
 *  
 *  Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
 *    Argument 1 = Number of pixels in NeoPixel strip
 *    Argument 2 = Arduino pin number (most are valid)
 *    Argument 3 = Pixel type flags, add together as needed:
 *    NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
 *    NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
 *    NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
 *    NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
 *    NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
 */


//#include <NeoPixelAnimator.h>
//#include <NeoPixelBus.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

#define PIN D4
#define NUMPIXELS 16

/* const */

int hashTag;
int hashTagThingSpeak;

/* APIs y configuracion wifi */
// WiFI
char ssid[] = "PON_SSID";     // SSID 
char pass[] = "PON_CLAVE_WiFi";
int status = WL_IDLE_STATUS;
// thingspeak
const char* server = "api.thingspeak.com";
const char * myReadAPIKey = "API KEY";
unsigned long myChannelNumber = AQUI_CANAL;


/* definicion objetos */
// Adafruit
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// WiFi
WiFiClient  client;


void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid,pass);
  ThingSpeak.begin(client);
  //hashTag = 0;
  //hashTag = ThingSpeak.readFloatField(myChannelNumber, 1, myReadAPIKey);
  pixels.begin(); // inicializacion de neopixel
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Conectando a SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // WPA/WPA2 
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConectado");
  }

  hashTagThingSpeak = ThingSpeak.readFloatField(myChannelNumber, 1, myReadAPIKey);
  Serial.print("Numero de hashtag ThingSpeak: ");
  Serial.println(hashTagThingSpeak);
  
  // se produce un evento,tweet con hashtag pintamos los "colores" de WomanTechMaker
  if( hashTag < hashTagThingSpeak){
    pintaColor(pixels.Color(0,255,255), 200);  // azul WTM
    pintaColor(pixels.Color(0, 0, 0), 10); 
    pintaColor(pixels.Color(0,255,100), 200);  // verde WTM
    pintaColor(pixels.Color(0, 0, 0), 10);
    pintaColor(pixels.Color(0, 0, 0), 10);
    hashTag = hashTagThingSpeak;
  }

} // fin principal

/* definicion funciones propias */

/*
 * Por cada pixel del anillo voy pintandolo, pongo el color en la ram.
 * Actualizo la tira de pixel para que coincidan
 * Se pausa
 */
void pintaColor(uint32_t color, int pausa){
  for(int i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(pausa);
  }
}
