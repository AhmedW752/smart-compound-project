#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "Crypto.h"
#include "PRP_CASES.h"
//
#define mac_size 4
#define n       12

//prototypes
//pinMode (D8,INPUT);

//AP
const char* ssid = "TESTING";
const char* password = "12345678";
IPAddress AP_IP(192, 168, 1, 1);
//STA
IPAddress staticIP(192, 168, 1, 24);
IPAddress gateway(192, 168, 1, 9);
IPAddress subnet(255, 255, 255, 0);

WiFiUDP Udp;
unsigned int localUdpPort = 4210; // local port to listen on
void setup(void)
{
  // start I2C communication
  Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
  // start the baud rate for the serial UART communication
  Serial.begin(9600);
  
  Serial.println();
  delay(500);
  Serial.printf("Connecting to %s\n", ssid);
  
  // connnecting to the server
  WiFi.begin(ssid, password);
  WiFi.config(staticIP, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  // start the UDP communication on a port
  Udp.begin(localUdpPort);
  
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}
void loop ()
{

  //recieving data from AVR
  char avr_data[n]="";
  char wifi_message[n]="nice";
  //char message[n] = "hi";
  char secret_key = 27;
  char encrypted_message[n] ;
  // variables for the encryptions 
  
  //variables to organize the code
  //bool ready_Encryption = false;
  //bool ready_Send = false; 
  String local_message;
//---------------------------------------------------------------//
// UART
char Rx_char=0;
int index=0;
 if ( Serial.available() > 0 )
 {
   while (Serial.available() > 0 && index <48 )
    {
    delay(50);
    Rx_char=Serial.read();
    avr_data[index]=Rx_char;
      if (Rx_char == '*')
     {
      avr_data[index]='\0';
      Serial.println(avr_data);
      Encrypt (encrypted_message,avr_data , secret_key);
      UDP_SEND(encrypted_message, AP_IP , localUdpPort);
      break;
     }
     index++; 
     }
  }

  //I2C
  char c;
  String prp_data_string;
  char prp_data[n];
    Wire.requestFrom(8, 1); /* request & read data of size 13 from slave */
    if(Wire.available())
    {
      
      c = Wire.read();
      //local_state = c;
      if(c != default_case)
      {
       //prp_data_string = print_state(c);
      //Serial.println(prp_data_string);
      /*
      for (int i = 0; i <= n; i++)
    {
      if (i < prp_data_string.length())
        wifi_message[i] = prp_data_string[i];
      else
        wifi_message[i] = 0;
    }*/
      print_state(wifi_message,c);
      Encrypt (encrypted_message,wifi_message , secret_key);
      UDP_SEND(encrypted_message, AP_IP , localUdpPort); 
      }
      
    }
    
delay(2000);
}

void Encrypt (char*encrypted_message,char* wifi_message ,char secret_key)
{
    char Eindex = 0;
    char Iindex = 0;
    //char encrypted_message[n] = {0};
    char key_AES[16];
    char key_MAC[16];
    // encryption (for loop 4 times)
    //              input 48 bytes => encrypt 12 each time => 16
    //              16 * 4 = 64 bytes 
  for (int index = 0 ; index < n ; index += (16-mac_size) )
  {
    sequence_key_generator(key_AES, key_MAC, mac_size, secret_key);
    char state[16] ;
    char message_t[16];
    for (int i = 0; i < (16-mac_size); i++)
    {
      state[i] = wifi_message[Iindex];
      Iindex++;
    }
    // get the MAC for authintication
    char temp_mac_message[16];
    copyArray(state, temp_mac_message, (char)sizeof(state));
    MAC(temp_mac_message, key_MAC, (char)sizeof(state));

    //-----------------------------------------------//
    //combine the state with the mac
    //-----------------------------------------------//
    combine_message(message_t, state, temp_mac_message, (char)sizeof(state) - mac_size, mac_size);
    //----------------------------------------------//
    //AES for the message for confidientiallyty
    //----------------------------------------------//
    aes_enc_dec(message_t, key_AES, 0);
    for (char i = 0; i < 16; i++)
    {
      encrypted_message[Eindex] = message_t[i];
      Eindex++;
    }
  }  
}
void UDP_SEND(char* mess , IPAddress IP, unsigned int Port )
{
  Udp.beginPacket(IP, Port);
  Udp.write(mess);
  Udp.endPacket();
}
