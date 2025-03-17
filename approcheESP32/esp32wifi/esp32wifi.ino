#include <WiFi.h>

// Sensor connectics
#define pinEcho 14
#define pinTrig 12

//LEDs connectics
#define pinRed 35
#define pinGreen 34

// WiFi network details
const char* ssid = "ILoveCycling";
const char* password = "KiwiKiwiKlan";

// My computer's IP address and port (shuuuuuuut no one can know)
const char* serverIP = "192.168.32.147";
const int serverPort = 5000;

WiFiClient client;
unsigned long previousMillis = 0;
const long interval = 5000; // Send data every 5 seconds

void setup()
{
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);

  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2); // Pour attendre avant de capter
                        // -> Evite les ondes indésirables
  Serial.begin(115200);
  delay(1000);
  
  // Connect to WiFi network
  Serial.println("\n");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  digitalWrite(pinRed, LOW);
  digitalWrite(pinGreen, LOW);
}

void loop()
{
  unsigned long currentMillis = millis();

  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);

  long duration = pulseIn(pinEcho, HIGH) * 0.01717; // Pour l'avoir en cm

  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    Serial.println(duration);
    String dataString = String(currentMillis / 1000) + "," + String(duration);

    sendData(dataString);
  }
}

void sendData(String data)
{
  Serial.print("Connecting to ");
  Serial.println(serverIP);
  
  if (client.connect(serverIP, serverPort))
  {
    Serial.println("Connected to server");
    
    // Send the data
    client.println(data);
    client.stop();
    digitalWrite(pinGreen, HIGH);
    
    Serial.println("Data sent: " + data);
  } 
  else 
  {
    Serial.println("Connection failed");
    digitalWrite(pinRed, HIGH);
  }

  delay(500);
}