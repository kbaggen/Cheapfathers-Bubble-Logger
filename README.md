# Cheapfathers-Bubble-logger!
A ferment Logger/controller, measuring "Temperature and Sound detection of CO2 bubbles" for NodeMCU (EXP8266). The sound detection is turned into Sound-Bubbles-Min (SBM) pr. Liter brew (SBM/L) there can be correlated with gravity (SG) though hydrometer measurements and finally give an indication/estimation of SG.
**WORK in Progress - The use of polynomial to predicts the SG needs verification in detail. Hence, to be tested over the next months by 2-3 brews**

Facebook group:
https://www.facebook.com/groups/2176394599141882/

### Introduction
This project measure: 
1. the Sound-bubbles as CO2 escape the fermenter by a digital Sound Sensor Detecting Module LM393
2. Temperature by ds18b20 and offset correction!
3. One channel Relay to control a heat source! 
4. A polonium is used to calculated an estimated/indicative SG (+/- 3 SG is the aim).

Hence, we simply make use of the "Big Blops" coming from airlock during fermentation. Ever tried to put you ear near to the airlock? You be surprised how loud the sound is and this is what we measure!

This project is very much inspired by  the commercial PLAATO product, but use different approach and very basic crude code to obtain the same functions. 

As frontend "Ubidots for Education" is used!

### Purpose
Knowing the activity though CO2 bubble measurements over time including the start of decline, we can better foresee when the SG is close to FG, and better make decision on dry-hopping, temperature changes, etc.! 

 <p align="center">
  <img width="855" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/into.png">
</p>
See full brew here --> https://app.ubidots.com/ubi/public/getdashboard/page/34PfeVSlCYvi_2V9wT3byvlC1h0

Other brew: https://app.ubidots.com/ubi/public/getdashboard/page/oryUSQPFTyZBZYJ23le4xzNlOtk

### Project outcome 
Hence, the goal is to have a temperature logger/controller, one tiny device to take care of measurement of fermentation rate, e.g. showing Acivity/Gravity and a same time control the temperature at one setpoint! The idea is to get one device instead of having both a temperature controller and fermentation-rate logger at a very low price, e.g. 10$ or EUR! Secondly, the idea is also to remove a lot of soldering (e.g. Ispindel) and to get away from placing anything in the tank (e.g. Ispindel/Tilt).  

### Important!
* Airtight - For CO2 to be measured the fermenter needs to be airtight! Hence, FORGET you buckets! You need sealed AIRTHIGHT tanks to do this!
* Sound detecting is prone to sound, hence, best placed in fridge especially if no interference is seen from your fridge! *Please Notice, the use in fridge is not fully tested yet (Interference)!*
* Sound sensor is prone to moisture, needs a "condom" (small water balloon)!
* A airtight Blow-off system is needed as else krausen-yeast-particles from blow-off will set-off the measurements! Or, you might get away with a high headspace of 33% together with a low temperature and hence a slow fermentation.
* This equipment do make use of Arduino IDE, and hence, you need to be familiar with Arduino IDE or willing to learn!!!
* **To make use of the inbuild polonium and hence SG estimation precise 4ml water should be added to the "S" airlock**!
* **You must calibrate the Sound sensor as below!**


 <p align="center">
  <img width="446" height="555" src="http://posebryg.bagge-nielsen.dk/wp-content/uploads/2019/05/59489491_10162219591400179_5222123827512737792_n.jpg">
</p>


### Equipment
1. Sound Sensor Detecting Module LM393
2. ds18b20  + resistor (4,7K ohm)!
3. NodeMCU (others likely also fine)!
4. Wires!
5. One small single-channel relay
6. "Activity-LED" to show a bubble is detected by NodeMCU (the sensor got one but is needed to really see if or not a bubble is detected/logged by NodeMCU)!


### Wiring
Temp probe (ds18b20) is on D4/pin2, the Sound Sensor Detecting Module LM393 is on Pin D5/Pin14 and Relay on D7/Pin 13. "activity-LED" is on D6/pin 12. I have isolated the Sound sensor 3.3v + GRD from he other parts to lower any interference, not sure if needed, but please consider it! Info, the long leg of LED is normally the positive (some recommend to have a resistor included to prolong life of LED, hence, please consider it). ***Note!*** Remember to make the wires sufficiently long to, e.g. to be able to have the logger outside fridge and sensor inside!
 
 <p align="center">
  <img width="556" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/diagram.png">
</p>
 
## Setup
1. Get a Ubibots for education account!
2. Install Arduino IDE 
   - install EXP8266 bards! 
   - Download "tinyexpry.h" and "ubidotsesp8266.h" (se above folders) and place in libraries folder in it own directory.
3. Download my .ino file, and type in your own data under settings area in regards of ssid, wifi, Ubidot token,  etc.!
   - **Please notice**, OG, start temp, brew size (in Liters) and brew name needs updated before each run!
4. Upload till NodeMCU though Arduino IDE

The settings you need to update is the following:
```
//............ Settings/SETUP - PLEASE CHANGE accordingly............................
#define TOKEN  "YOUR Ubidots token"                                         // Put here your Ubidots TOKEN  
#define WIFISSID "YOUR wifi"                                                        // Put here your Wi-Fi SSID                     
#define PASSWORD "YOUR pass"                                                                // Put here your Wi-Fi password                            
char my_polynominal[100] = "(0.0000000000004*sum^3)-(0.0000001*sum^2)+(0.0049*sum)+0.33";  // Inset your polynomial if calibrated, sum = SBM/L
double TEMP_OFFSET = 0;                                                                     // Offset temperature of ds18b20 sensor
//.............BELOW NEED TO BE CHANGE BEFORE EACH BREW..............................
#define DEVICE  "YOUR name"                                                                    // Name of brew, whatever
double START_TEMP = YOUR temp;                                                                     // Start temperature
double brewsize = YOUR bresize;                                                                       // The sieze of you brew in liters
double OG = YOUR OG;                                                                           // Your start gravity, OG                                               
//...................................................................................
```
Please ntoice the Punctuation Marks (") and where they are not, as this is important!


### Using Ubidot
The Bubble-Logger script make a device at Ubidots of "YOUR name" and push the following till Ubidots "temp", "settemp", "SBM", and the current "SBM_sum" of SBM. Further it push a calculated "SG" estimated by the initial "OG" and "brew size" you stated under settings, and it is utterly important it is entered correct for the script to work!

 <p align="center">
  <img width="655" height="445" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/you%20brew.png">
</p>

Based on the data you can create a Dashboard and graphs where you have max. 500 datapoints, hence, you needs to under device create "Rolling Windows" where you make an average calculation every 60 min (the 500 datapoints equalls 18-20 days). To do so, select "Rolling Windows/YOUR Brew/SG or SBM/Average " type in 60 mins and select your "Start date+time".

 <p align="center">
  <img width="655" height="445" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/avg.png">
</p>

To have Temperature control, you need to add to Dashboard (where you most likely also wish to add the above average SBM and SG as a graph), a "Control/Slider/Settemp" widget of the range of your chosen temperature range (10-30C)! It should be noted the script do not support values below 1´C and turn over to a backup system if values below 1 is set. This is to secure the temperature system don’t go down if connection till "Ubidots Education" is lost (if no connection a value of 0 or below is read and posted by the logger/Ubidtos).

 <p align="center">
  <img width="655" height="445" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/settemp_control.png">
</p>

## Calibration 
### Calibration of Sound Sensor Detecting Module LM393
The digital Sound Sensor Detecting Module LM393 needs to be calibrated to a degree where it is responsive, but where we also can "work" besides make some noise. What I do is: 

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

4. DONE! This give you a calibrated LM393 (and mine look like this):


<p align="center">
  <img width="556" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/sensors.png">
</p>
5. REmember to put LM393 back on D5/pin 14


This give a high resolution sensor there miss a few and also post some double bubbles. This calibration should give you between 80 and up till 200 SBM at high krausen (maybe a bit more) depending on temperature/yeast/brew, etc! This setting is prone to sounds,  but light talking, music, drier and washing machine is ok to have nearby! 

***To be able to compare from brew to brew of SBM and hence make use of polynoium you should try to hold as many variable the same, and foremost have same amount of water in airlock (+ same kind of S-airlock). I use 4 ml for the inbould polynoium. Secondly, take a picture of your LM393 potentiometer setting and stick to it especially if you update/change the polynoium***

If you do not wish to make use of SG estimation and soly use it as activity logger you can ofcouse turn the potentiometer a bit more down to make it less sensitive! 

*Please notice, the use in fridge is currently not fully tested (interference), but seems to work with a few miss-reading here and there depending on the age of the fridge and hence it level of interference!*

### Fitting the Condom - Water Balloon on the LM393 - fitting in Airlock
The LM393 need a mosture protection, and this is done by a small water balloon, and it should be loose in the noose. If it dosent sit tight in airlock depending on size, make it fit (rubberband?), so it is fitted stable. It should not be pushed thight all the way down to bottom as this will make a kind of sealing, and hence, restict the CO2 in getting out and foremsot restict the bubble sound! So when you press on tank/blow-out you should hear a normal "blop"!

 <p align="center">
  <img width="446" height="556" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/condom_LM393.jpg">
</p>

### Calibration of ds18b20 temperature probe
It should be noted that he cheap ds18b20 probes even rated at +/- 0.5`C, is no anywhere near that, so check you probe in boiling water and also ice bath (e.g. the crushed ice and water amount should be around 1:1), and inset the correction at TEMP_OFFSET in the setting in the Arduino.ino file!  

### Calibration of the polynomial
**Please Notice, the below is still work in prrogress**
You might need to do your own SBM/L vs. hydrometer measurements to get good SG estimations as it is unclear to me how much tank seize, temperature, different LM393 and variations in airlocks all play together even taken the above calibration procedure into account and the amount of 4 ml in airlock! 

If you should wish to update the polynomial base on above unknown factors, you will need to model a 3rd degree (or 2nd) polynomial based on Sound-Bubbles-Min/Liter (SBM/L) towards the reduction in SG from your hydrometer reading during a brew! I use the following 3rd degree polynomial for the SG estimation!
 <p align="center">
  <img width="855" height="446" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/poly.png">
</p>
As it can be seen from the above picture showing data from 2 different settings of the LM393´s pontimeter, even a small difference from the calibration method of "1024", it do have a high impact of the needed SMB/L for the same reduction of SG. 

The used polynoium inbuild in the script is curreently the "Blue Line"! The blue line is also the least sensitive setting for the sensor, and hence easier to work with in regards of sideby "niose". 

The data I collected is at (excel) link: https://1drv.ms/x/s!An5QQQ1io7W7iPwHmv-1I0dIkRlGiw
