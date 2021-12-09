# WirelessHumiditySensor

**Device overview**

The device described is a wireless plant-soil humidity sensor, it consists of a single 'slave' device and up to 10 'controller' devices. A 'controller' device captures the humidity level using a sensor and sends it to the 'slave' device that normalises the data and displays it via the OLED screen. A single button was used to simplify the user interface. The ESP NOW protocol is used to connect the devices over a wireless network. Data can be sent from as far as 480 meters which makes this device suitable for plant-soil monitoring in greenhouses. Along with the captured humidity data, the 'slave' device also tracks the connection status of the 'controller' devices. The device must be calibrated before use to accurately reflect the relative humidity amount. 

![IMG_20211209_203323](https://user-images.githubusercontent.com/68562161/145469727-ef667d45-2756-4d98-a8f7-de8990bd72a1.jpg)


The 'controller' devices are designed to be powered using batteries. To save energy, the device is put into 'deep sleep' mode for 5 minutes immediately after capturing and sending the humidity sensor data. If no data is received for 15 minutes, the slave device automatically considers the connection status of the 'controller' device as pending.

**External dependencies**

Adafruit SSD1306, GFX and BusIO libraries are required to control the OLED screen. Simply place the libraries inside the _library_ folder.

**Installation process**

To set up a 'slave' device, set the number of sensors used by re-defining _NUM_SENSORS_  to reflect the number of 'controller' devices and install the code provided alongside the external dependencies. 

To set up a 'controller' device, one must re-define _MY_ID_ with a number from 0-9, change the _receiverAddress_ to the MAC address of the 'slave' device and install the code.

**Code overview**

**'Slave' device:** 
The humidity data, connection status and identification number for each 'controller' device is maintained in a _HumiditySensorNode_ class. An interrupt is generated when data is received from a 'controller' device. Inside the callback function, the humidity data and connection status is updated, and the pending status counter is restarted. 

An interrupt is generated when a button press is detected to avoid polling, debouncing was implemented inside the software.

Functions related to screen content drawing are placed inside the main function. A flag is raised when the screen needs to be updated, this is to avoid triggering WDT restart due to interrupt functions interfering with the background processes. 

**'Controller' device:**
The processor never reaches the main loop, all functions are executed inside the setup() function. The device measures the analog voltage at the output of the sensor, initialises ESP NOW connection and sends the data packet. At the end of the setup() function, the device is put into 'deep sleep' mode. 

![IMG_20211209_203919](https://user-images.githubusercontent.com/68562161/145469962-c4acc326-0168-4965-b2c7-6f554707a749.jpg)

![IMG_20211209_203414](https://user-images.githubusercontent.com/68562161/145469967-2330cb6c-0a90-4940-9447-eb81f9309ff1.jpg)

![IMG_20211209_205402](https://user-images.githubusercontent.com/68562161/145469973-08cf2054-3496-4d10-916e-d36f52067968.jpg)
