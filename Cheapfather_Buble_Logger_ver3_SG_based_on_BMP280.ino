  //..................................................................................
  //         "Cheapfather´s Bubble-Logger!"                                                                     
  //         Script by www.Cheapfather.com, Klaus Bagge Nielsen                       
  //         Email: klaus"a"bagge-nielsen.dk, October 2019.                                                                  
  //
  //         Detection of Sound-Bubbles, Gravity calculation, Temperature,
  //         + setpoint temperature control of Relay with Fallback (hence, if Ubidots
  //         connection falls the last known setpoint for temperature is used)!
  //         For frontend please make an account on Unibots and add
  //         details in below "setting"! 
  //
  //         Sound Sensor Detecting Module LM393 is on Pin D5/Pin14
  //         Relay on D7/Pin 13.
  //         Temperature probe is on pin2/D4 of nodemcu!
  //         BMP280 pressure module is on pin 4/D2 for SDA and SCL is on pin5/D1.                                                                              
  //..................................................................................
   
  

  

//............ Settings/SETUP - PLEASE CHANGE accordingly............................
#define TOKEN  "Your Token"                                          // Put here your Ubidots TOKEN  
#define WIFISSID "Your SSID"                                                          // Put here your Wi-Fi SSID                     
#define PASSWORD "Your pass"                                                                  // Put here your Wi-Fi password                            
char my_polynominal[100] = "(-0.0000004*sum^2)+(0.009*sum)";                                 // Inset your polnominal if calibrated, sum = SBM/L
double TEMP_OFFSET = 0;                                                                       // Offset temperature of ds18b20 sensor
double PRESSURE_OFFSET = 5;                                                                   // Offset Pressure of BMP280 sensor
//.............BELOW NEED TO BE CHANGE BEFORE EACH BREW..............................
#define DEVICE  "YOUR brew"                                                                        // Name of brew, whatever
double START_TEMP = 10;                                                                       // Start temperature
double brewsize = 23;                                                                         // The sieze of you brew in liters
double OG = 1049 ;                                                                             // Your start gravity, OG                                               
//...................................................................................


#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <tinyexpr.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "UbidotsMicroESP8266.h"
#include <Arduino.h>
 
  
  bool DEBUG = true;
  

  // temperature, onewire statements
  #define ONE_WIRE_BUS 2 //temperture sensor on pin2/D4 of nodemcu!
  

  

  // Soundbubbler statements
  int sensorPin = 14; // select the input pin for the Sound detector 
  int sensorValue = 0; // variable to store the value coming from the sensor
  int bubbleCount = 0; // Count for bubbles (resets after sending)
  #define VARIABLE_SBM_sum_pt_L "SBM_sum_pt_L"
  double SBM_sum_pt_L;
  

  // timer statements
  int timer = 0; //
  int timerTrigger = 6000; //  every 2 minutes 
  

  

  //relay_temp statement
  #define RELAY0_PIN 13
  #define VARIABLE_settemp  "settemp"
  int powerstate;
  double temp_set_for_relay;
  double temp_store;
  double settemp;
  
  // statement for BMP280 pressure
  Adafruit_BMP280 bmp; // I2C
  #define BMP280_ADDRESS (0x76) // if not working use 77 insteed also change down below at begin!!!! 

  Ubidots client(TOKEN);
  OneWire oneWire(ONE_WIRE_BUS); 
  DallasTemperature sensors(&oneWire);
  

  

  void setup(void) {
    
    Serial.begin(115200);
    Serial.print("Cheapfather´s Bubble-logger");
    pinMode (sensorPin, INPUT);
    pinMode(RELAY0_PIN, OUTPUT);
    digitalWrite(RELAY0_PIN, HIGH);
    client.setDataSourceName(DEVICE); 
    client.setDataSourceLabel(DEVICE); 
    client.wifiConnection(WIFISSID, PASSWORD);
    sensors.begin();
    temp_store = START_TEMP;
    settemp = START_TEMP; 

  Serial.println(F("BMP280 test"));
  
  if (!bmp.begin(0x76)) {  // if not working use 77 insteed
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

}
  

  

  void loop(void) {
  

   
      // read the value from the sound sensor
  

         sensorValue = digitalRead(sensorPin); 
           
          if (sensorValue == 0) {
            bubbleCount++; // Increment bubble count
            if (DEBUG) { Serial.println("Bubble!"); }
            }
  

      // Increment the timer
        delay(20);
        timer++;
  

  

     if (timer > timerTrigger) {
                  
          // main loop every 2 mins
          
          // claculating P_faktor (pressure factor) as Atmospheric pressure impact on Bubble rate amount
            double PT_faktor;

 
                  double Pressure =   PRESSURE_OFFSET + (bmp.readPressure()/100);  // the 100 to get fron Pa tll hPa.
                  Serial.print(F("  Pressure = "));
                  Serial.print(Pressure);
                  Serial.println(" hPa");
                  
                  PT_faktor = 1-((1015-Pressure)/60)*TEMP/20); // Function for taking pressure and temperature impact on bubble rate into account.
                  
          // Recalulating Bubbles over 2 min till SBM (sound-bubbles/min) and calculate SBM sum. If bubbles over 300 SBM disregarded as error!
              double SBM;
            

              double SBM_sum_pt_L_GET;
              
              if (bubbleCount <= 600){
                                   SBM = (bubbleCount/2);// change if longer than 2min
                                   }
              
              SBM_sum_pt_L_GET = client.getValueWithDevice(DEVICE, VARIABLE_SBM_sum_pt_L);
              if (SBM_sum_pt_L_GET >= 0){
                                   SBM_sum_pt_L = (SBM_sum_pt_L_GET + ((P_faktor*SBM*2)/brewsize)); // change 2 if longer than 2min  
                                   }
              
          
              
          // Getting temp
              sensors.requestTemperatures(); 
              float temp_ds18b20 = sensors.getTempCByIndex(0); 
              double temp;
              // disregard temperatures below 0    
              if (temp_ds18b20 >= 0){
                                  temp = temp_ds18b20 - TEMP_OFFSET; // change if longer than 2min  
                                   }
  
  
              
        // calculate Gravity(SG)
   
              float gravity = 0;
                int err;
                double _sum = SBM_sum_pt_L;
                te_variable vars[] = {{"sum", &_sum}};
      
                te_expr *expr = te_compile(my_polynominal, vars, 1, &err);
      
              if (expr)
                {
                 gravity = te_eval(expr);
                 te_free(expr);
                 
                }
              else
               {
               Serial.printf("Parse error at %d\n", err);
               }
  

               double SG;
               SG = (OG-gravity);
  

              
           // printing data serial 
              Serial.print(F("  PT-faktor = "));
              Serial.print(PT_faktor);  
              Serial.print(F("  SG = "));
              Serial.print(SG);
              Serial.print("  ");
               Serial.print(F("  Temp = "));
              Serial.print(temp);
              Serial.print(" Celsius, ");
                       
              Serial.print(F("  SBM = "));
              Serial.print(SBM);
        
              Serial.print(F("  SBM_sum_pt_L = "));
              Serial.print(SBM_sum_pt_L);
                   
                 
             //temp_relay control and check if Ubidots is posting settemp value, else using Fallback/backup temp (last settemp over 0)!  
                                             
                  temp_set_for_relay = client.getValueWithDevice(DEVICE, VARIABLE_settemp);                
                  
                    if (temp_set_for_relay > 1)
                    {temp_store = temp_set_for_relay;}
                    Serial.print("  Tstore = ");
                    Serial.print(temp_store);      
                        
                  if (temp_set_for_relay > 1){
                                      if (temp > temp_set_for_relay){
                                          digitalWrite(RELAY0_PIN, HIGH);
                                          Serial.print("   Relay OFF ");
                                          Serial.print("  TempSet = ");
                                          Serial.print(temp_set_for_relay);
                                          }
                                      else if (temp < temp_set_for_relay)
                                          {
                                           digitalWrite(RELAY0_PIN, LOW); 
                                           Serial.print("   Relay On - Heating!! ");
                                           Serial.print("  TempSet = ");
                                           Serial.print(temp_set_for_relay);
                                          }
                                      }
                   else if (temp_set_for_relay < 1 ){
                                      if (temp > temp_store){
                                          digitalWrite(RELAY0_PIN, HIGH);
                                          Serial.print("   Relay OFF ");
                                          Serial.print("  Using Fallback Temp  = ");
                                          Serial.print(temp_store);
                                          temp_set_for_relay = temp_store;                
                                          }
                                      else if (temp < temp_store)
                                          {
                                           digitalWrite(RELAY0_PIN, LOW); 
                                           Serial.print("   Relay On - Heating!! ");
                                           Serial.print("  Using Fallback Temp = ");
                                           Serial.print(temp_store);
                                           temp_set_for_relay = temp_store;               
                                          }
                                      }
            
                                                     
       // Make a request to the endpoint with the bubble count, SBM_sum, powerload, gravity and temperature
                client.add("Temperature", temp);
                client.add("SBM", SBM);
                client.add("SBM_sum_pt_L", SBM_sum_pt_L);
                client.add("Settemp", temp_set_for_relay);
                client.add("SG", SG);
                client.sendAll(true);
        
        // Reset, timer, bubbles, bubble sum, gravity 
                timer = 0;
                bubbleCount = 0;
                gravity = 0;
  

     }                                        
}
