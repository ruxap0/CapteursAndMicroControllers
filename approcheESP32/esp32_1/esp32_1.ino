#include "DHT22.h"
#include "WiFi.h"
#include "esp_now.h"

#define GREEN_LED 22
#define RED_LED 17
#define PIN_SENSOR 4

uint8_t peer_addMAC[] {0x3c, 0x8a, 0x1f, 0xb9, 0xdb, 0x78};

uint8_t time_passed = -2;

typedef struct
{
  float temp;
  uint8_t time;
} DATA_SENT;

DATA_SENT myData;
DHT22 sensor(PIN_SENSOR);
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *peer_addMAC, esp_now_send_status_t status)
{
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Succès" : "Échec");
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
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, peer_addMAC, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
      Serial.println("Échec d'ajout du peer");
      while(1); // pour éviter d'aller plus loin en attendant un reset
  }

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void loop()
{
  time_passed = time_passed + 2;
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  myData.temp = sensor.getTemperature(); // True pour avoir les degrés en Celsius -> fait par défaut
  myData.time = time_passed + 2;

  esp_err_t result = esp_now_send(peer_addMAC, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK)
  {
    Serial.println("Données envoyées !");
    digitalWrite(GREEN_LED, HIGH);
  }
  else
  {
    Serial.println("Échec d'envoi");
    digitalWrite(RED_LED, HIGH);
  }

  delay(2000);
}
