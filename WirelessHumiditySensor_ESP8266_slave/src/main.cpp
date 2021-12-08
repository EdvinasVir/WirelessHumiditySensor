#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <espnow.h>
#include <HumiditySensorNode.h>

#define MY_NAME   "SLAVE_NODE"
#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels
#define OLED_RESET -1           // OLED reset pin not used
#define NUM_SENSORS_MAX 10      // up to 10 controller devices can be connected to one slave using ESP NOW
#define NUM_SENSORS 1           // number of active controller devices
#define PENDING_LIMIT_MS 120000 // 2 minute before the node status is set to pending

// set the maximum number of sensors
#if NUM_SENSORS > NUM_SENSORS_MAX 
  #define NUM_SENSORS NUM_SENSORS_MAX
#endif

volatile boolean displayFlag = false; // a flag is set to update screen contents
volatile uint8_t selectSensor = 0;    // select the sensor node to display

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // OLED display object
HumiditySensorNode sensorArr[NUM_SENSORS] = { HumiditySensorNode() };      // sensor node objects
Ticker sensorStatusTicker[NUM_SENSORS_MAX] = { Ticker() };   // initialise a max number of tickers required to safeguard against undefined behaviour if wrong ticker obj is called
volatile int8_t pendingNode = -1; // if there are no pending nodes, the value is set to -1;

// because input arguments are not allowed, individual interrupt service routine must be created for each ticker object
void ticker0ISR()
{
  pendingNode = 0;
  if(pendingNode > NUM_SENSORS)
  {
    pendingNode = -1;
    return;
  }

  else
  {
    sensorArr[pendingNode].setNodeStatus(false);
    if(selectSensor == pendingNode)
    {
      displayFlag = true;
    }
    pendingNode = -1;
  }
}
void ticker1ISR()
{
  pendingNode = 1;
  if(pendingNode > NUM_SENSORS)
  {
    pendingNode = -1;
    return;
  }

  else
  {
    sensorArr[pendingNode].setNodeStatus(false);
    if(selectSensor == pendingNode)
    {
      displayFlag = true;
    }
    pendingNode = -1;
  }
}
void ticker2ISR()
{
  pendingNode = 2;
  if(pendingNode > NUM_SENSORS)
  {
    pendingNode = -1;
    return;
  }

  else
  {
    sensorArr[pendingNode].setNodeStatus(false);
    if(selectSensor == pendingNode)
    {
      displayFlag = true;
    }
    pendingNode = -1;
  }
}
void ticker3ISR()
{
  pendingNode = 3;
  if(pendingNode > NUM_SENSORS)
  {
    pendingNode = -1;
    return;
  }

  else
  {
    sensorArr[pendingNode].setNodeStatus(false);
    if(selectSensor == pendingNode)
    {
      displayFlag = true;
    }
    pendingNode = -1;
  }
}
void ticker4ISR()
{
  pendingNode = 4;
  if(pendingNode > NUM_SENSORS)
  {
    pendingNode = -1;
    return;
  }

  else
  {
    sensorArr[pendingNode].setNodeStatus(false);
    if(selectSensor == pendingNode)
    {
      displayFlag = true;
    }
    pendingNode = -1;
  }
}
void ticker5ISR()
{
  pendingNode = 5;
  if(pendingNode > NUM_SENSORS)
  {
    pendingNode = -1;
    return;
  }

  else
  {
    sensorArr[pendingNode].setNodeStatus(false);
    if(selectSensor == pendingNode)
    {
      displayFlag = true;
    }
    pendingNode = -1;
  }
}
void ticker6ISR()
{
  pendingNode = 6;
  if(pendingNode > NUM_SENSORS)
  {
    pendingNode = -1;
    return;
  }

  else
  {
    sensorArr[pendingNode].setNodeStatus(false);
    if(selectSensor == pendingNode)
    {
      displayFlag = true;
    }
    pendingNode = -1;
  }
}
void ticker7ISR()
{
  pendingNode = 7;
  if(pendingNode > NUM_SENSORS)
  {
    pendingNode = -1;
    return;
  }

  else
  {
    sensorArr[pendingNode].setNodeStatus(false);
    if(selectSensor == pendingNode)
    {
      displayFlag = true;
    }
    pendingNode = -1;
  }
}
void ticker8ISR()
{ 
  pendingNode = 8;
  if(pendingNode > NUM_SENSORS)
  {
    pendingNode = -1;
    return;
  }

  else
  {
    sensorArr[pendingNode].setNodeStatus(false);
    if(selectSensor == pendingNode)
    {
      displayFlag = true;
    }
    pendingNode = -1;
  }
}
void ticker9ISR()
{
  pendingNode = 9;
  if(pendingNode > NUM_SENSORS)
  {
    pendingNode = -1;
    return;
  }

  else
  {
    sensorArr[pendingNode].setNodeStatus(false);
    if(selectSensor == pendingNode)
    {
      displayFlag = true;
    }
    pendingNode = -1;
  }
}

void (*tickerISR[])(void) = { ticker0ISR, ticker1ISR, ticker2ISR, ticker3ISR, ticker4ISR, ticker5ISR,
                                ticker6ISR, ticker7ISR, ticker8ISR, ticker9ISR };

uint16_t  nodeMin[NUM_SENSORS] = { 300 }; // min and max values are acquired during controller node calibration process
uint16_t  nodeMax[NUM_SENSORS] = { 600 };

struct __attribute__((packed)) dataPacket { // data packet size is reduced by adding ((packed)) attribute (250 bytes max)
  int id;                                   // identify the sensor node using id number
  int humidityAnalog;                       // 10-bit humidity input value 
}; 

IRAM_ATTR void buttonISR()
{
  // timer based debouncing  
  uint16_t count = 0;            
  uint16_t oldv, newv;           
  oldv = digitalRead(D8); 
  // proceed only when counter reaches 2000
  // if the value is unstable, reset the counter      
  while(count < 2000)           
  {
    newv = digitalRead(D8);
    if(newv == oldv)
    {
      count++;                                   
    }
    else
    {
      count = 0;                           
      oldv = newv;
    }
  }
  selectSensor = selectSensor + 1;      // scroll through available sensor data
  if(selectSensor > NUM_SENSORS_MAX - 1)  
  {
    selectSensor = 0;   // once the sensor 10 has been reached, jump back to sensor 1
  }
  displayFlag = true;  // display the changes
}

void dataReceived(uint8_t *senderMac, uint8_t *data, uint8_t dataLength) 
{
  dataPacket packet;                                              // initialise an identical struct in a both controller and slave nodes 
  memcpy(&packet, data, sizeof(packet));                          // access the received data by copying it into a packet 
  sensorArr[packet.id].updateHumidity(packet.humidityAnalog);     // select a correct node and update the humidity data
  if(packet.id == selectSensor)                                     
  {                                                                 
    displayFlag = true;                                           // if the currrently selected sensor is updated, display the changes
    sensorArr[packet.id].setNodeStatus(true);                     // update the status of the node once the data is received
    {                                                              
      sensorStatusTicker[packet.id].attach_ms(PENDING_LIMIT_MS, tickerISR[packet.id]);  // restart ticker
    }                                                              
  }
}

void initialiseDisplay()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);  // using white OLED display
  display.setTextSize(1);
}

void initialiseWireless()
{
  WiFi.mode(WIFI_STA); // ESP NOW initialisation routine
  WiFi.disconnect();  
  esp_now_init(); 
  esp_now_register_recv_cb(dataReceived); // a callback function is called once the data is received
}

void updateDisplay()
{
  if(selectSensor > NUM_SENSORS - 1)  // if displaying disconnected sensor
  {
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.print("Sensor:");
    display.print(selectSensor + 1);
    display.setTextSize(1);
    display.setCursor(0, 54);
    display.print("Status:disconnected");
  }

  else  // if displaying connected sensor
  {
    display.setTextSize(2);
    display.setCursor(0, 5);
    display.print("Sensor:");
    display.print(selectSensor + 1);
    display.setTextSize(1);
    display.setCursor(0, 40);
    display.print("Humidity:");
    display.print(sensorArr[selectSensor].getHumidity());
    display.print("%");
    display.setCursor(0, 54);
    display.print("Status:");
    if(sensorArr[selectSensor].getNodeStatus() == true) // if the node has been updated recently
    {
      display.print("connected");
    }
    else  // if the connection is pending
    {
      display.print("pending");
      display.setTextSize(4);
      display.setCursor(100, 30);
      display.print("!");
      display.setTextSize(1);
    }
  }
}

void setup() 
{ 
  Serial.begin(9600);
  pinMode(D8, INPUT_PULLUP); // pullup resistor used instead of an external resistor
  attachInterrupt(digitalPinToInterrupt(D8), buttonISR, RISING);  
  
  for(int n = 0; n < NUM_SENSORS; n++) //initialise the sensor nodes with id, min and max expected values
  {
    sensorArr[n].initialise(n, nodeMin[n], nodeMax[n]);      
  }
  
  initialiseDisplay();  
  initialiseWireless(); 
  displayFlag = true; 
}

void loop() 
{
  /* a flag is used to notify the processor when to update the display
  ESP devices have a WDT which resets if an interrupt 
  routine takes too long to complete, as this is the case with SSD1306 library */
  while(displayFlag == true) 
  {
    display.clearDisplay();
    updateDisplay();      // place the data to display in ram
    display.display();    // display the data currently in ram
    displayFlag = false;  // reset the flag
  }
}
