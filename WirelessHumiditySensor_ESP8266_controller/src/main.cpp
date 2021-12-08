#include<ESP8266WiFi.h>
#include<espnow.h>

#define MY_NAME         "CONTROLLER_NODE"
#define MY_ROLE         ESP_NOW_ROLE_CONTROLLER   // set the role of this device: CONTROLLER, SLAVE, COMBO
#define RECEIVER_ROLE   ESP_NOW_ROLE_SLAVE        // set the role of the receiver
#define WIFI_CHANNEL    14
#define MY_ID 0                                   // individual ID for each controller node
#define SLEEP_TIME_uS 30 * 1000000                // sleep for 30 seconds

uint8_t receiverAddress[] = {0xE8,0xDB,0x84,0xDB,0x61,0xA0};   // MAC address of the receiver

// ((packed)) attribute ensures data structure padding is disabled 
struct __attribute__((packed)) dataPacket {
  int id;
  int humidityAnalog;
}; 

void transmissionComplete(uint8_t *receiver_mac, uint8_t transmissionStatus) 
{
  // no need to have anything in the callback function
}

void setup() 
{
  dataPacket packet;  // transmission packet
  packet.id = MY_ID;  // set individual controller id number
  packet.humidityAnalog = analogRead(PIN_A0);  //read 10-bit analog voltage from pin A0
  
  WiFi.mode(WIFI_STA);      // wifi station mode
  WiFi.disconnect();        // we do not want to connect to a WiFi network

  esp_now_init();
  esp_now_set_self_role(MY_ROLE); // controller role
  esp_now_register_send_cb(transmissionComplete);   // this function will get called once all data is sent
  esp_now_add_peer(receiverAddress, RECEIVER_ROLE, WIFI_CHANNEL, NULL, 0);  // connect to a slave device 
  esp_now_send(receiverAddress, (uint8_t *) &packet, sizeof(packet)); // send the sensor data 
  
  ESP.deepSleep(SLEEP_TIME_uS); // begin deep sleep
}

void loop() 
{
  // esp is switched to deep sleep mode before reaching the main loop
}