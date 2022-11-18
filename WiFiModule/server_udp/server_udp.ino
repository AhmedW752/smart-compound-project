#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "Crypto.h"
#include "compound_macros.h"
// define the server ip
IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 0, 80);
IPAddress subnet(255, 255, 255, 0);
// server name and password
const char* ssid = "TESTING";
const char* password = "12345678";
WiFiUDP Udp;

unsigned int localUdpPort = 4210; // local port to listen on
char incomingPacket[255]; // buffer for incoming packets
//char replyPacekt[] = "Message received"; // a reply string to send back
int len = 0;
/*
  given the cipher nymber num in te crypto.h (a => num = 7)
  key_shared = Ya => a^(Xa)
  hey_cipher = Xa
*/
#define mac_size 4
#define n  16 // size of the message

//char key_cipher =0;// DH(q, num); //Xa
//char key_shared =0;// pow(num, key_cipher); //Ya
//char key_seq = 27; // pow(Yb , Xa);

//// encryption standered data


void setup() {
  Serial.begin(9600);
  //Serial.println();
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "AP Configured" : "Configuration Failed!");
  Serial.print("Setting Soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "AP Ready" : "Failed!");
  Serial.print("Soft-AP IP address: ");
  Serial.println(WiFi.softAPIP());
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

}
int packetSize = 0;
void loop() {
  char key_seq = 27; // pow(Yb , Xa);
  char REC_key_AES[16];
  char REC_key_MAC[16];
  // encryption standered data
  char Oindex = 0;
  char Iindex = 0;
  char output[n-mac_size] = {0};
  // share the key

  // wait for packet recieving
  //    while(!packetSize){
  //     packetSize = Udp.parsePacket();
  //     }
  //     // read the packet
  //     len = Udp.read(incomingPacket, 255);
  //    if (len > 0)
  //    {
  //      incomingPacket[len] = 0;
  //    }
  //    Serial.printf("UDP packet contents: %s\n", incomingPacket);//get the key
  //     // generate the key
  //     key_cipher = DH(q, num); //Xa
  //     key_shared = pow(num, key_cipher); //Ya
  //     // send key_shared
  //     Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  //     Udp.write(key_shared);
  //     Udp.endPacket();
  //     // generate the key
  //    char key_seq = pow((char) *incomingPacket, key_cipher);
  //recieve the message
  
  packetSize = 0;
  while (!packetSize) {
    packetSize = Udp.parsePacket();
  }
  // read the packet
  
  len = Udp.read(incomingPacket, 255);
  if (len > 0)
  {
    incomingPacket[len] = 0;
  }
  //Serial.println(incomingPacket);
  
  //Serial.printf("UDP packet contents: %s\n", incomingPacket);//get the key
  Serial.print(incomingPacket);
  Serial.print(",");
  //Serial.print(" the message after decryption : ");
  //start decrryption
 // for (int index = 0 ; index < n ; index += 16 )
  //{
    sequence_key_generator(REC_key_AES, REC_key_MAC, mac_size, key_seq);
    //char state[16] = { 0 };
    char message_t[n];
    for (int i = 0; i < 16; i++)
    {
      message_t[i] = incomingPacket[Iindex];
      Iindex++;
    }
    aes_enc_dec(message_t, REC_key_AES, 1);
    //decryption testing
    char data[n];
    char temp_data[n];
    char mac[mac_size];
    // separate the mac from the message
    separete_mac(message_t, data, mac, (char)sizeof(message_t), mac_size);

    //generate the mac from the message again
    copyArray(data, temp_data, (char)sizeof(data));

    // compare the mac
    if (macCompare(temp_data, mac, REC_key_MAC, (char)sizeof(data), mac_size))
    {
      //Serial.println("message recieved successfully");
    }
    for (char i = 0; i < n-mac_size; i++)
    {
      output[Oindex] = data[i];
      Oindex++;
    }
     Serial.println(display_message(output));//serial print macr => message
 // }
  //char macro[] ={property_1 , problem , uitily,gas_problem,0 ,0,0,0,0,0,0,0};
  
  //Serial.println(output);
  //space_lines();
  delay(2000);
}
void space_lines()
{
  Serial.println("");
  Serial.println("//----------------------------------------------------------------------------//");
  Serial.println("");
  Serial.println("//----------------------------------------------------------------------------//");
  Serial.println("");
}
