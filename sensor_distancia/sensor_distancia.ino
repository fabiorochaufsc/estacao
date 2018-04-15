#include <ESP8266WiFi.h>

#define TRIGGER 5
#define ECHO    4
#define PORTA 10000
const char* ssid     = "xxxx";
const char* password = "xxxx";
const char* servidor = "10.1.1.19";

char buffer[100];
float dist;

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
  
  dtostrf(dados, 3, 2, floatComoStr);
  sprintf(resul,
  "{\"modulo\":\"%s\",\"valor\":\"%s\"}",
  tipo, floatComoStr);
}
float capturaDistancia (void)
{
  long duration;
  float distance;
  noInterrupts();
    digitalWrite(TRIGGER, LOW);  
    delayMicroseconds(2); 
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10); 
    digitalWrite(TRIGGER, LOW);
  interrupts();
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
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
   pinMode(TRIGGER, OUTPUT);
   pinMode(ECHO, INPUT);

   dist = capturaDistancia();
   criaJSON("distancia",dist,buffer);
   conecta();
   enviaDado(buffer);
   desconecta();
   ESP.deepSleep(50e6); // 50 segundos
}
void loop (void)
{
  
}
