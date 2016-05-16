#include <Wire.h>
#include <DHT.h>
// Only used for sprintf
#include <stdio.h>

#include <avr/sleep.h>
#include <BH1750.h>


BH1750 lightMeter;


volatile int sleep_count = 0; // Keep track of how many sleep
// cycles have been completed.
const int interval = 5; // Interval in minutes between waking
// and doing tasks.
const int sleep_total = 1;
//(interval*60)/8; // Approximate number 

#define DHTPIN 2 // what digital pin we're connected to
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

char strC1[10],strC2[10],strTH1[10],strTH2[10],strTH3[10],strL[10];
String strSend;
int t1,t2,t3,c1,c2,h1,h2,h3,l;
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
 // pinMode(34, OUTPUT);
 // turnOnSens();
 // watchdogOn();
 
 Serial.begin(9600); 
 Serial.println("start");
 lightMeter.begin();
 dht.begin();
 delay(3000);
}

void H1T1(){
    float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
   float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
}

void turnOnSens(){
  digitalWrite(34,HIGH);
}
void turnOffSens(){
  digitalWrite(34,LOW);
}

void lightMeter_BH1750() {
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
}

void loop()
{
  //goToSleep(); // ATmega328 goes to sleep for about 8 seconds
// and continues to execute code when it wakes up
//  if (sleep_count >= sleep_total) {
 //   turnOnSens();
   // delay(10000);
   delay(3000);
    sleep_count=0;
    strSend = "";
  
    
  // while(readd() != "10"){
 // for (int i=0; i <= 10; i++){
      
      delay(1000);
      lightMeter_BH1750();
      H1T1();
//    turnOffSens();
    
 // }
}

String readd(){
  String aa = Serial.readString();
  Serial.println(aa);
  return aa;
}




void goToSleep()   
{
// The ATmega328 has five different sleep states.
// See the ATmega 328 datasheet for more information.
// SLEEP_MODE_IDLE -the least power savings 
// SLEEP_MODE_ADC
// SLEEP_MODE_PWR_SAVE
// SLEEP_MODE_STANDBY
// SLEEP_MODE_PWR_DOWN -the most power savings
// I am using the deepest sleep mode from which a
// watchdog timer interrupt can wake the ATMega328

set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode.
sleep_enable(); // Enable sleep mode.
sleep_mode(); // Enter sleep mode.
// After waking from watchdog interrupt the code continues
// to execute from this point.

sleep_disable(); // Disable sleep mode after waking.
                     
}

void watchdogOn() {
  
// Clear the reset flag, the WDRF bit (bit 3) of MCUSR.
MCUSR = MCUSR & B11110111;
  
// Set the WDCE bit (bit 4) and the WDE bit (bit 3) 
// of WDTCSR. The WDCE bit must be set in order to 
// change WDE or the watchdog prescalers. Setting the 
// WDCE bit will allow updtaes to the prescalers and 
// WDE for 4 clock cycles then it will be reset by 
// hardware.
WDTCSR = WDTCSR | B00011000; 

// Set the watchdog timeout prescaler value to 1024 K 
// which will yeild a time-out interval of about 8.0 s.
WDTCSR = B00100001;

// Enable the watchdog timer interupt.
WDTCSR = WDTCSR | B01000000;
MCUSR = MCUSR & B11110111;

}

ISR(WDT_vect)
{
sleep_count ++; // keep track of how many sleep cycles
// have been completed.
}
