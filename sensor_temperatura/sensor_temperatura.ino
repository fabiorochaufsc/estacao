#include "ds18b20.h"
#include <ESP8266WiFi.h>

#define PORTA 10000
const char* ssid     = "xxxx";
const char* password = "xxxx";
const char* servidor = "10.1.1.19";
char buffer[100];
float temp;

void conecta (void)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}
void criaJSON(char *tipo,float dados,char *resul)
{
  char floatComoStr[20];
  
  dtostrf(dados, 4, 3, floatComoStr);
  sprintf(resul,
  "{\"modulo\":\"%s\",\"valor\":\"%s\"}",
  tipo, floatComoStr);
}
void enviaDado (char *dado)
{
  WiFiClient client;
  for (int x=0;x<5;x++)
  {
    if (client.connect(servidor, PORTA)) {
      client.println("POST /posts HTTP/1.1");
      client.println(String("Host: ")+
      String(servidor));
      client.println("Cache-Control: no-cache");
      client.println("Content-Type: \
      application/json");
      client.print("Content-Length: ");
      client.println(strlen(dado));
      client.println();
      client.println(dado);
      client.println("Connection: close");
      client.println();
      client.stop();
      return;
    }
    delay(500); 
  }
}
void desconecta()
{
  WiFi.disconnect();
}
void setup(void)
{
  conecta();
  temp = DS18B20_Captura_temperatura();
  criaJSON("temperatura",temp,buffer);
  enviaDado(buffer);
  desconecta();
  ESP.deepSleep(50e6); // 50 segundos
}
void loop (void)
{
 
}
