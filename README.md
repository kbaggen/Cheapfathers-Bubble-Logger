# UPDATE MARCH 2020 - I moved out of github, please go till www.bubble-logger.com
# Secondly I moved on till Optical inferred sensor!

# Cheapfathers-Bubble-logger!
A ferment Logger/controller, measuring "Temperature and Sound detection of CO2 bubbles" for NodeMCU (ESP8266). The software give an indicative SG estrimate based on S-airlock is used with a precise amount of water (4-4,5 ml)! 

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
  <img width="446" height="555" src="https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/tank3.png">
</p> Here is my 30L tank, wtih logger onside. Sensor in s-airlock. Sometimes I use a airtight blow-off system seen next to tank! My other 20L Braumeister Speidel tank standing on 30W heating mat: "https://github.com/kbaggen/Cheapfathers-Bubble-Logger/blob/master/pic/tank1.jpg


 
## Theory behind this project
What we meassure is as said the Sound Blops pr. Min comming when CO2 is released, e.g. SBM, and if we look  at the chemistry behind the metabolisms of fermentation of sugar by yeast cells, we see one part Alcohol generate two part CO2. Hence, CO2 is a direct measument of the alcohol production. The key issue is to measument this gas accuarate and precise. Hence, the need of sealed airtight tanks.

                   C6H12O6    ====>   2(CH3CH2OH)      +        2(CO2)         (+Energy)
                    Sugar     ====>    Alcohol         +    Carbon dioxide gas (+Energy)

Hence, by knowing the SBM and brew sieze (L) and other involving constants like pessure (p) and temperature (t) we can plot a curve of SBM/pt/L vs. the reduction in Gravity and generate a model or polynomial for the alcohol production based on our initial measument of Sound Blops pr. Min (SBM), se more below. 

### Atmospheric pressure + Temperature and SBM - Indirectly impacting SG
The Atmospheric pressure do impact on the amount of bubbles in the sense 50% more bubbles can be seen at very low pressure! My assuption is the bubbles is of lower size, and hence the release of CO2 is not higher, we just see more tiny bubbles so to speak, or the density of gasses in each bubbles is less. 


The temperature also impacts on the acivity of gasses, and hence at lower temperature the gas is not moving as fast and therefore the bubbles rate is lower at Pilsner temperature of 10`c vs. an ale of 20´C. So to acount for less acivity at lower temeprature the following fomalar is used (20´C is used as baseline):



### SG estrimation by polynomial approach
This software give an indicative SG with an offset of  +/- 2 SG untis if used in a airtight tank and an S-shaped airlock with 4 ml water in it!

# MORE AT www.bubble-logger.com
