#include <HumiditySensorNode.h>

HumiditySensorNode::HumiditySensorNode() 
{

}
HumiditySensorNode::~HumiditySensorNode()
{

}

void HumiditySensorNode::initialise(uint8_t id, uint16_t inputMin, uint16_t inputMax) 
{
    m_inputMin = inputMin;   // maximum expected humidity analog value
    m_inputMax = inputMax;   // minimum expected humidity analog value
    m_humidity = 0;          // initial humidity value
    m_nodeStatus = false;    // node is considered active only when it was recently updated
}

void HumiditySensorNode::updateHumidity(uint16_t humidityAnalog)
{
    m_humidity = map(humidityAnalog, m_inputMax, m_inputMin, 0, 100); //map an analog value to a range between 0 and 100
}

uint16_t HumiditySensorNode::getHumidity()
{
    return m_humidity;
}

boolean HumiditySensorNode::getNodeStatus()
{
    return m_nodeStatus;
}

void HumiditySensorNode::setNodeStatus(boolean nodeStatus)
{
    m_nodeStatus = nodeStatus;
}