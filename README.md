# KitchenSensor
A system designed to detect when someone is using their kitchen and alert a carer.
### Introduction
This project aims to create a smart home solution for carers of elderly people or people with dementia who are living independently. It was inspired by my grandmother, who often forgets to eat meals and cannot reliably report about how much she has eaten. This device monitors use of the kitchen and sends a daily report by wi-fi to the carer’s email address. It also tweets whenever motion is detected in the kitchen. The carer can use this information to monitor whether the elderly person has used the kitchen to make food, and then make decisions accordingly. In order to create this device, I hypothesised that food preparation in the kitchen space could be accurately measured and reported using this device, which uses motion detection and a temperature/humidity sensor. The research questions needed to test this hypothesis are:
* How many readings of “active” by the PIR motion sensor indicate food preparation?
* What temperature and humidity patterns indicate food preparation?
* How can this information be communicated accurately through ThingSpeak to the carer?

### Approach
Hardware Development

<img src="./images/fig1.jpg" width="400px">

The initial hardware setup is shown in Figure 1. This was used to gather data from the sensors in order to determine thresholds. The data was read from the sensors and stored as a CSV file on the SD card using the attached datalogging shield. However, the datalogging shield did not have heads and it was difficult to get a reliable connection with the sensors.

<img src="./images/fig2.jpg" width="400px">
Figure 2 shows the location of the hardware in the kitchen. The temperature and humidity sensor are placed close to the stovetop in order to detect when it is used. The PIR motion sensor is directed towards the rest of the kitchen in order to detect movement near the stove and sink.
 
<img src="./images/fig3.jpg" width="600px">
Figure 3 shows the second version of the setup. Initially, it was planned that the Arduino would have a datalogger attached which would be used to store, read and analyse the data. However, given the limited dynamic memory capacity it was determined that using an online service would be a simpler alternative. The datalogging shield was removed, and an ESP-01 Wi-Fi module. This was used to upload the sensor data to ThingSpeak. Adding in the Wi-Fi module necessitated the use of a breadboard.
 
<img src="./images/fig5.jpg" width="600px">
Figure 4 shows the final version of the device. Previously, the setup had problems with power. Due to the insufficient power, sometimes the sensors would stop retrieving data.
A power supply was used in order to ensure regular power supply, and a voltmeter was used to select an appropriate supply from the AC adapter (which has variable voltage).
<img src="./images/Schematic.png" width="1000px">
Figure 5 shows a circuit diagram of the final hardware setup. A voltmeter was used to test the 3V and 5V rail and determine if the setup had sufficient power.

### Software Development

#### Arduino software
For each of the hardware iterations, a different Arduino program was written. The various components are described below in the order the were written. Some were removed as the project evolved, indicated in italics.

1.	Read data from the PIR motion sensor and the temperature/humidity sensor simultaneously

2.	_Logging the data as a CSV file that could be retrieved and analysed_

3.	_Reading data from the CSV and analysing it in arrays_

4.	_Sending emails from the Arduino using an SMTP email service_

5.	Sending sensor data to ThingSpeak at 20 second intervals.
