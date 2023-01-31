# EnergyMeter_WiFi_Com
This repository holds the code used by the WiFi module of my Wattmeter project. The *PIC24FJ256GA110* integrated on the *MCP3910 Evaluation Board* used to aquire data does not support wireless communication by itself and requires an external source to provide it. 
This design uses an ESP32 microcontroller that will read data sent by the PIC through an RS-232 wire and will transmits it via WiFi.

## Hardware design
| Component | Device | Link |
|-----------|--------| -----|
| Microcontroller board | WEMOS LOLIN32 Lite Development Board | [Link](https://www.amazon.es/AZDelivery-Lolin-ESP32-Parent/dp/B086W49HRH)
| RS-232 Interface | MIKROE RS232 Click | [Link](https://www.mikroe.com/rs232-click)

The RS-232 interface is powered by the *LOLIN32*. Cross connection of pins can be done either betweeen these two componets or in the wire connecting the *MCP3910 Evaluation Board* and this module.

## Software design
This module is only responsible for reading the data sent via RS-232 by the *MCP3910 Evaluation Board* and displaying it in real time graphs. It will connect the watt-meter to a WiFi
network predefined by the user in the code and launch an asynchronous web server, which
means that it will be able to handle different clients at the same time. An HTML file is stored
in the SPI Flash File System SPIFFS of the ESP32. This is system lets the user access the
flash memory of the device as if it was a normal file system like the one implemented on a
regular computer. The file contains the structure of the web application and the declaration
of the charts where the variables will be represented and updates the graphs periodically.

### System configuration
A serial port of the ESP32 needs to be configured in order to receive data from the other
board. In this case, pins 16 and 17 are used for the communication. Together they make
the Serial port 2 of the ESP32. The MCP3910 Evaluation Board uses a default baud rate of
921.6kbaud. When configuring the serial pins for the ESP32, the same transmission speed
is specified.

The file stored in the SPIFFS needs to be uploaded and built before executing the program. PlatformIO
provides its own tool to do this. When the configuration is running, the SPIFFS needs to be
started.

In order to connect to a Wifi network, the code uses the WiFi library provided by the
Arduino Framework for ESP32 boards. The user needs to specify the credentials before
running the software. After connecting to the network, the terminal outputs the local IP
address that has been assigned by the network provider to the WiFi module. This is the
address that the user types on a web browser to access the real time data charts.
The final part of the configuration launches an asynchronous server, which means that it
will always be running even if it is not executed continuously. The server is implemented
thanks to the external library ESPAsyncWebServer. Right before launching the server, the
request handlers are declared. When the ESP32 receives the first request on the root URL,
the HTML file saved in the SPIFFS is sent to the network. When receiving any other URL
specified in the handlers and the charts declarations, it calls the function that return the
requested parameter.

### Loop
The continuous loop will read the serial port periodically and obtain a char array containing
the differnt data separated by commas. A special function
checks the array’s integrity, finds the commas in it and breaks it into six
variables (VRMS, IRMS, P, Q, S, PF). These variables are the ones called by the request
handlers

### HTML File
The file uses HTML language to set up the charts and give the web page a structure. It consists of seven charts implemented using the library Highcharts. Its is included as JavaScrip
language in the code. Seven dynamic charts are created to represent the seven variables.
The charts, which act as the web client on the server, automatically produce seven HTML
requests using an URL pointing to the file stored in the SPIFFS that holds the host and path
for the data stored in the ESP32 memory every 500 milliseconds. The handlers stored in
the microcontroller send the respective variable value to the client as an HTML text that is
processed by the chart class and added to the plot. The new variables are represented against
time thanks to a time capturing function included in the chart declarations. The axis update
automatically when if a new value exceeds the previous limits.
