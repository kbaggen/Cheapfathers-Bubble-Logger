# Cheapfathers-Bubble-logger!
A ferment Logger/controller, measuring "Temperature and Sound detection of CO2 bubbles" for NodeMCU (ESP8266). The software give an indicative SG estrimate based on S-airlock is used with a precise amount of water (4 ml)! 


### Introduction
This project measure: 
1. the Sound-bubbles as CO2 escape the fermenter by a digital Sound Sensor Detecting Module LM393
2. Temperature by ds18b20 and offset correction!
3. SG is calculated from complex model taking pressure and temperatur into account.
4. One channel Relay to control a heat source. 
(I used the cold sheed or cold utility floor as cooling, and hence, only need a heat source) 

Hence, we simply make use of the "Big Blops" coming from airlock during fermentation. Ever tried to put you ear near to the airlock? You be surprised how loud the sound is and this is what we measure!

As frontend "Ubidots for Education" or "Ubidots STERM" is used!

### Purpose
Knowing the yeast activity though CO2 bubble measurements over time including the start of decline, we can better foresee when the SG is close to FG, and better make decision on dry-hopping, temperature changes, etc.! 

Brooklyn Lager Clone:
 <p align="center">
  <img width="755" height="400" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/brroklyn.png">
</p>

"Yoga Pils":
<p align="center">
  <img width="755" height="400" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/yoga2.png">
</p>
(other pictures of brews in the end)

### Project outcome 
Hence, the goal is to have a temperature logger/controller, one tiny device to take care of measurement of fermentation rate, e.g. showing Acivity as CO2 "Sound-bubbles pr Minute" (SBM) and a same time control the temperature at one setpoint! Secondly, to give an indicative SG estrimate with a offset of +/- 2 SG units. The idea is to get one device instead of having both a temperature controller and fermentation-rate logger at a very low price, e.g. 10$ or EUR! Secondly, the idea is also to remove a lot of soldering (e.g. Ispindel) and to get away from placing anything in the tank (e.g. Ispindel/Tilt). In some sense this is a DIY Plaato.  

### Important!
* Airtight - For CO2 to be measured the fermenter needs to be airtight! Hence, FORGET you buckets! You need sealed AIRTHIGHT tanks to do this!
* Sound detecting is prone to sound, hence, best placed in box/fridge even the calibration is set to allow rather much noise nearby. 
* Sound sensor is prone to moisture, needs a "condom" (small water balloon, se below)!
* Krausen-yeast-particles or wort shooting out onto the sound detector will set-off the measurements! hence, ..... 
  - A airtight Blow-off system is needed if you use low headspace, 
  - Or,  a high headspace of 33% together with a low temperature and hence a slow fermentation can also overcome the issue!
* All calculations is based on precise amounts of wort, hence, keep eye on you amounts in Liters.
* The logger post to Ubidots every second minte, and as missing SBM post will influrence on precision of SG estrimation, you need good/steedy WiFi. 
* This equipment do make use of Arduino IDE, and hence, you need to be familiar with Arduino IDE or willing to learn!!!


 <p align="center">
  <img width="446" height="555" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/tank2.JPG">
</p> Here is my 30L tank, wtih logger onside. Sensor in s-airlock. Sometimes I use a airtight blow-off system seen next to tank! 

### Equipment - Wiring
Besides a NodeMCU and some double female til female wires to connect it all:
 - Temp probe (ds18b20 + resistor (4,7K ohm)) is on D4/pin2. 
 - The Sound Sensor Detecting Module LM393 is on Pin D5/Pin14.
 - BMP280 pressure module is on pin 4/D2 for SDA and SCL is on pin5/D1. You need to solder the header onto the BMP280, and **this is the only place any soldering is needed in this project.**
 - Optinal - A single side Relay (D7/Pin 13). 
<p align="center">
  <img width="556" height="646" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/diagram2.png">
</p>
(Remember to make the wires sufficiently long for sound senor and temp probe, e.g. to be able to have the logger outside fridge and sensor inside if your fridge is of metal or otherwise bad conectivity. The BMP280 pressure sensor is just placed together with NodeMCU in some kind of box you may have floating around).
 
## Theory behind this project
What we meassure is as said the Sound Blops pr. Min comming when CO2 is released, e.g. SBM, and if we look  at the chemistry behind the metabolisms of fermentation of sugar by yeast cells, we see one part Alcohol generate two part CO2. Hence, CO2 is a direct measument of the alcohol production. The key issue is to measument this gas accuarate and precise. Hence, the need of sealed airtight tanks.

                   C6H12O6    ====>   2(CH3CH2OH)      +        2(CO2)         (+Energy)
                    Sugar     ====>    Alcohol         +    Carbon dioxide gas (+Energy)

Hence, by knowing the SBM and brew sieze (L) and other involving constants like pessure (p) and temperature (t) we can plot a curve of SBM/pt/L vs. the reduction in Gravity and generate a model or polynomial for the alcohol production based on our initial measument of Sound Blops pr. Min (SBM), se more below. 

### Atmospheric pressure + Temperature and SBM - Indirectly impacting SG
The Atmospheric pressure do impact on the amount of bubbles in the sense 50% more bubbles can be seen at very low pressure! My assuption is the bubbles is of lower size, and hence the release of CO2 is not higher, we just see more tiny bubbles so to speak, or the density of gasses in each bubbles is less. This can be handled by calculating if you know the pressure, hence the BDM280 sensor! What I do is I have set 1015 hPa as my baseline as this is the geomean pressure in Denmark, furthermore at the very high the pressure is 1040, and at  the very low 980 hPa, giving a range of 60 hPa. Hence, to account for the pressure-range over the year I use:

P-faktor => 1-((1015-x)/60)), where X is the pressure at the giving time!

This turns for instance at very low pressure of 995 hPa: 1-((1015-995)/60)=Y <=> Y= 0.666, and this factor is then used to re-calculate the SBM/L into SBM/p/L to compresate for the release of CO2 at low Atmospheric pressure.

The temperature also impacts on the acivity of gasses, and hence at lower temperature the gas is not moving as fast and therefore the bubbles rate is lower at Pilsner temperature of 10`c vs. an ale of 20´C. So to acount for less acivity at lower temeprature the following fomalar is used (20´C is used as baseline):

PT-faktor => 1-((1015-x)/60))*(TEMP/20), 
hence, we get SBM/pt/L from the intinial SBM measument by addition with the PT-faktor.

### SG estrimation by polynomial approach
This software give an indicative SG with an offset of  +/- 2 SG untis if used in a airtight tank and an S-shaped airlock with 4 ml water in it!

The SG is caclualted by we measure the SBM over time and this is re-cacluated in regards of SBM/pt/L by taken the pressure (p), temperature (t) and brew sieze into account (L), and hence this is used by the polynoimal to calcualte the SG though a second degree polynoimal.

 <img width="656" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/data2.png">

If you wish to dig into the data a bit more see below link where it can be seen the last 9 brews where I used my logger with a standard error of the mean of +/- 2 SG units:

 <img width="656" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/mean_data.png">

***So, if a keen eye on placement/alingment of sound probe, 4ml water amount in s-arilock and a slow/controlled fermentation in a airtight fermenter and the use of the same calibrated sensor, it is possible to use CO2 as a SG measument though a 2nd degree poly.*** 

You might have to re-calculate you own polynomial based on your sound sensor and its calibration. Hence, make 2-3 brews and take hydrometer measuments over the time compared to SBM/pt/L and update your polynomial. The same is likley needed if you change sound sensor.

All used data can be seen here: https://1drv.ms/x/s!An5QQQ1io7W7icIIRECfkWO0aTP8-Q?e=b4ph5j

## Setup
1. Get a Ubibots STEM account!
2. Install Arduino IDE 
   - install EXP8266 boards! Se "http://arduino.esp8266.com/Arduino/versions/2.0.0/doc/installing.html"
   - Download "dependencies.zip" (se above folders) and extract the content into Arduino libraries folder so all the various folders of zip-file is kept (e.g. varoius folders stored into C:\Users\xxxxxx\Documents\Arduino\libraries\xxxxxx).
 3. Download my .ino file, and type in your own data under settings area in regards of ssid, wifi, Ubidot token,  etc.!
   - **Please notice**, OG, start temp, brew size (in Liters), and brew name needs updated before each brewing!
4. Upload till NodeMCU though Arduino IDE

The settings you need to update is the following:
```
//............ Settings/SETUP - PLEASE CHANGE accordingly............................
#define TOKEN  "YOUR Ubidots token"                                    // Put here your Ubidots TOKEN  
#define WIFISSID "YOUR wifi"                                           // Put here your Wi-Fi SSID                     
#define PASSWORD "YOUR pass"                                           // Put here your Wi-Fi password               
char my_polynominal[100] = "(-0.0000004*sum^2)-(0.009*sum)";           // Inset your polnominal if calibrated, sum = SBM/pt/L
double TEMP_OFFSET = 0;                                                // Offset temperature of ds18b20 sensor
double PRESSURE_OFFSET = 5;                                            // Offset presure, check, you pressure on internet nearby!
//.............BELOW NEED TO BE CHANGE BEFORE EACH BREW..............................
#define DEVICE  "Tester"                                                // Name of brew, whatever
double START_TEMP = 20;                                                 // Start temperature
double brewsize = 20;                                                   // The sieze of you brew in liters
double OG = 1050;                                                       // Your start gravity, OG                     

```                          

Please ntoice the Punctuation Marks (") and where they are not, as this is important!


### Using Ubidot
The Bubble-Logger script make a device at Ubidots of "YOUR name" and push the following till Ubidots "SG", "temp", "settemp", "SBM", and the current "SBM_sum_pt_L" of SBM. The script push data every second minute.

 <p align="center">
  <img width="655" height="445" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/you%20brew.png">
</p>

Based on the data you can create a Dashboard and graphs whit your data as the various Dashboards pictures I show ant the tope and end of this page (the one from above is from Ubidtos STEM and below is from Ubidots EDUCATION).

To have Temperature control, you need to add to Dashboard (where you most likely also wish to add the SBM and SG as graphs), a "Control/Slider/Settemp" widget of the range of your chosen temperature range (10-30C)! It should be noted the script do not support values below 1´C and turn over to a backup system if values below 1 is set. This is to secure the temperature system don’t go down if connection till "Ubidots" is lost (if no connection a value of 0 or below is read and posted by the logger/Ubidtos).

## Calibration 
### Calibration of Sound Sensor Detecting Module LM393
The digital Sound Sensor Detecting Module LM393 needs to be calibrated to a degree where it is responsive, but where we also can "work" besides make some noise. What I do as a base calibration: 

1. Put on the "condom" (small water balloon) on the LM393, se picture!
2. To calibrate the LM393 connect it to A0 (analog pin) (and ofcouse 3.3v/GRD) and run below script in Arduino IDE:
```
/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  */

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);
  delay(20);        // delay in between reads for stability
}
```
3. Adjust the potentiometer of LM393 until it just turns (and only just) from rubish numbers of a mix of "58, 64 or xx" and shows a steady "1024" on the serial monitor(9600)!

 <p align="center">
  <img width="556" height="276" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/Calibration.png">
</p>

4. DONE! This give you a calibrated LM393, whereoff, mine look like this (I propose you set yours accordingly as closed as you can of below picture):


<p align="center">
  <img width="256" height="400" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/sound_sensor.png">
</p>

5. Remember to put LM393 back on D5/pin 14


This give a high resolution sensor there miss a few and also post some double bubbles. This calibration should give you between 50 and up till 100-150 SBM at high krausen depending on temperature/yeast/brew size (I brew in 14-25L amounts), etc! This setting is prone to high sounds,  but light talking, music, drier and washing machine is ok to have nearby! 

***To be able to compare from brew to brew of SBM and hence make use of polynoium you should try to hold as many variable the same, e.g. same sensor from brew to brew and foremost have same amount of water in airlock (+ same kind of S-airlock). I use 4 ml. Secondly, take a picture of your LM393 potentiometer setting and the amount of water in airlock from brew to brew***

A picture of the amount of water under pressure:
<p align="center">
  <img width="190" height="265" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/water_airlock.jpg">
</p>

### Fitting the Condom - Water Balloon on the LM393 - fitting in Airlock
The LM393 need a mosture protection, and this is done by a small water balloon, and it should be rather tight around the noose, but still loose as above pictures shows. It  needs to sit tight in the airlock making an seal to resticict any water from vaporiation. This restict the CO2 in getting out so to let the CO2 out please drill a small hole in the airlock! Align it so the mirc is place over the direct hole in the airlock, so the sesor get the direct sound "blup". 
 <p align="center">
  <img width="446" height="256" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/sensor_3pic.png">
 </p>
As the sensor got some shapes edges there will flence the ballon and secondly as the mirc rather easily can break off, try steady the sensor by some tape as first picture shows!

### Calibration of ds18b20 temperature probe
It should be noted that he cheap ds18b20 probes even rated at +/- 0.5`C, is no anywhere near that, so check you probe in boiling water and also ice bath (e.g. the crushed ice and water amount should be around 1:1), and inset the correction at TEMP_OFFSET in the setting in the Arduino.ino file!  

### Calibration of BDM280 pressure sensor
Check the pressure in a serial monitor of Arduino IDE and if it is as mine was 5 hPA off (to the one you compare with on internet as nearby to you). The altitude of you location vs. the one you compare with do impact, hence, you might need to use a bit commen sense here. Change PREssure_OFFSET accordingly in settings.


## Facebook group (if any questions)
https://www.facebook.com/groups/2176394599141882/

## Brews with SG
<p align="center">
  <img width="855" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/RawWeiss.png">
</p>

<p align="center">
  <img width="855" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/ReUse.png">
</p>

<p align="center">
  <img width="855" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/raw_neipa.png">
</p>

<p align="center">
  <img width="855" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/SW_clone.png">
</p>

## Other brews
<p align="center">
  <img width="855" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/intro.png">
</p>
<p align="center">
  <img width="855" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/sort.png">
</p>

<p align="center">
  <img width="855" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/oldboys.png">
</p>

<p align="center">
  <img width="855" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/rhubarbwheat.png">
</p>
