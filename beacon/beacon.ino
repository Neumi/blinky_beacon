/*
   thanks to the author! https://thearduinoandme.wordpress.com/tutorials/esp8266-send-receive-binary-data/
  - ip adress has to be set manually
  - network name and password has to be set correctly
  
*/

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

// wifi connection variables
const char* ssid = "MartinRouterKing";
const char* password = "i_have_a_stream";

boolean wifiConnected = false;

// UDP variables
unsigned int localPort = 8888;
WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back

IPAddress ip(192, 168, 1, 152);            // IP address of the server
IPAddress gateway(192, 168, 1, 1);        // gateway of your network
IPAddress subnet(255, 255, 255, 0);       // subnet mask of your network

void setup() {
  // Initialise Serial connection
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  // only proceed if wifi connection successful
  if (wifiConnected) {
    udpConnected = connectUDP();
    if (udpConnected) {
      // initialise pins
      pinMode(16, OUTPUT);
    }
  }
}

void loop() {
  // check if the WiFi and UDP connections were successful
  if (wifiConnected) {
    if (udpConnected) {

      // if there’s data available, read a packet
      int packetSize = UDP.parsePacket();
      if (packetSize)
      {
        Serial.println("");
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remote = UDP.remoteIP();
        for (int i = 0; i < 4; i++)
        {
          Serial.print(remote[i], DEC);
          if (i < 3)
          {
            Serial.print(".");
          }
        }
        Serial.print(", port ");
        Serial.println(UDP.remotePort());

        // read the packet into packetBufffer
        UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
        Serial.println("Contents: ");
        int value = packetBuffer[0] * 256 + packetBuffer[1];
        Serial.println(value);

        UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
        UDP.write(ReplyBuffer);
        UDP.endPacket();

        digitalWrite(16, value);
      }
      delay(10);

    }
  }
}

boolean connectUDP() {
  boolean state = false;

  Serial.println("");
  Serial.println("Connecting to UDP");

  if (UDP.begin(localPort) == 1) {
    Serial.println("Connection successful");
    state = true;
  }
  else {
    Serial.println("Connection failed");
  }

  return state;
}


boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to: ");
    Serial.println(ssid);
    Serial.print("IP address:");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("connection failed");
  }
  return state;
}
