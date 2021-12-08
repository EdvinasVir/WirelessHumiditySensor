#ifndef HumiditySensorNode_h
#define HumiditySensorNode_h

#include <Arduino.h>
#include <stdint.h>

class HumiditySensorNode
{
public:
    HumiditySensorNode();   // constructor
    ~HumiditySensorNode();  // destructor

    void initialise(uint8_t id, uint16_t inputMin, uint16_t inputMax);
    void updateHumidity(uint16_t humidityAnalog);
    uint16_t getHumidity();
    boolean getNodeStatus();
    void setNodeStatus(boolean nodeStatus);
private:

    uint16_t m_humidity; // normalised humidity value
    uint16_t m_inputMax; // maximum expected input value  
    uint16_t m_inputMin; // minimum expected input value
    boolean m_nodeStatus;
};
#endif