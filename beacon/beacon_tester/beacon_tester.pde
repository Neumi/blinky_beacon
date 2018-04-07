import hypermedia.net.*;

UDP udp;
String ip = "192.168.1.152"; // remote beacon IP address
int port = 8888; // destination port

long previousMillis = 0;
int light = 0;
long interval = 500;
int flicker = 0;
int held = 0;
byte[] message = new byte[2];

void setup() {
  udp = new UDP( this, 8888 );
  udp.listen( true );
}

void draw()
{
  int delTime = 30;
  delay(delTime);
  message[1] = 0;
  udp.send(message, ip, port);
  delay(delTime);
  message[1] = 1;
  udp.send(message, ip, port);
}

void receive( byte[] data ) {
  for (int i=0; i < data.length; i++)
    print(char(data[i]));
  println();
}