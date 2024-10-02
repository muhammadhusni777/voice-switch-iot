#include <WiFi.h>
#include <PubSubClient.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "";
const char* password = "";


const char* mqtt_server = "192.168.1.14";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


float analog_value = 0;
float voltage_read;
float sensor_read;

// LED Pin
const int ledPin = 5;

void setup() {
  Serial.begin(115200);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT
  if (String(topic) == "lights") {
    Serial.print("Changing output to ");
    if(messageTemp == "ON"){
      Serial.println("ON");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "OFF"){
      Serial.println("OFF");
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32_husni")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("lights");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    
    
    analog_value = analogRead(A0);   
    voltage_read = map(analog_value, 0, 1023, 0, 3300); // dalam mili volt. 
    sensor_read = voltage_read/10; //
    
    
    char sensor_read_send[8];
    dtostrf(sensor_read, 1, 2, sensor_read_send);
    Serial.print("Temperature: ");
    Serial.println(sensor_read_send);
    client.publish("sensor", sensor_read_send);

  }
}
