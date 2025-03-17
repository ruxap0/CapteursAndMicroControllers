#include "WiFi.h"
#include "esp_now.h"

#define GREEN_LED 35
#define RED_LED 34

typedef struct
{
  float temp;
  uint8_t time;
} DATA_RCV;

DATA_RCV myData;

//uint8_t peer_addMAC[] {0x5c, 0x01, 0x3b, 0x67, 0xb4, 0xc0};
// Pas besoin de l'adresse MAC de l'envoyeur car on ne doit pas lui réenvoyer une donnée en retour
//esp_now_peer_info_t peerInfo;

void OnDataRecv(const esp_now_recv_info_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&myData, incomingData, sizeof(myData));

  digitalWrite(GREEN_LED, HIGH);

  Serial.print(myData.temp);
  Serial.print("°C ---- ");
  Serial.print(myData.time);
  Serial.println("sec");

  delay(500);
  
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);

}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Erreur ESP-NOW");
    while(1); // pour éviter d'aller plus loin en attendant un reset
  }

  esp_now_register_recv_cb(OnDataRecv);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
}

void loop()
{
  // Tout sera traîté dans le Callback OnDataRecv(...)
}
