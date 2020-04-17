# NodeMCU - COVID - statistics

This project pretends to be a useful gadget to give trusty information about the current COVID statistics all around the world. 
At the same time, the interested developers on this project could found a good option to spend their lockdown due to the quarantine by improving themself with interesting projects.

## Connections diagram:

![connections](/images/circuit.jpg)

## Release notes:

### Version 1.0.0beta
* Consumes a REST API with https protocol that gives COVID statistics
* Current version of "novelcovid" API is: V2 (https://corona.lmao.ninja/v2/countries/)
* Extracts specific data from the JSON response
* Many wifi SSID and passwords can be saved on it thanks to ESP8266WiFiMulti.h
* Uses OLED SSD1306 as the display (I2C)
* After displaying the data for each country display a logo again.

## Development notes:
## This project is based on:
* BasicHTTPSClient.ino from Examples of ESP8266HTTPClient.h library 

### Displaying custom images on OLED:
* https://www.hackster.io/najad/interfacing-and-displaying-images-on-oled-59344a

### Converting images to cpp code:
* https://diyusthad.com/image2cpp

### How to get the fingerprint of a https rest api:
* https://www.ssl2buy.com/wiki/how-to-view-ssl-certificate-details-on-chrome-56

![fingerprint](/images/fingerprint.jpg)

## Demo:
Taken on April 16th 2020:

![animated demo](/images/demo.gif)

### Thanks to:
https://github.com/novelcovid/api