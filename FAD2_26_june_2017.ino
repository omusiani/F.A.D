#include "HX711.h"
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// author @deeweks@gmail.com $ omusianieno@gmail.com
// arduino code for an FAD system done under guidance/request from ndambukijulius02@yahoo.com

//setting up variables
//1.lcd
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);
//***********************************************************************

//defining station select buttons
const int clean = 8;
const int  okay= 9;
const int  adultered = 10;
const int  done = 11;

boolean petrol,diesel,parrafin,water = false;

//*********************************************************************
//variables
float density;
int inByte;
String systemLogo = "... FUEL A_D ...";
String display_var = "PLEASE SELECT";
String condition = "WAIT..";
float reading ;//reading of mass from the sensor
float total_weight;
float avg_weight;
int count;

//************************************************************************************
//densities of near pure samples at 20*c obtained from the internet
float density_petrol = 0.73722;//737.22 kg/m3
float density_diesel= 0.885;//820 - 950 kg/m3
float density_parrafin = 0.87515;//817.15 kg/m3
float density_water = 1.0;//grammspercm3
//********************************************************************************
//volume set
float set_volume = 150.00; //cm^3
//_________________________________________________________________________________
//hx711 values
// HX711.DOUT- pin # A1

// HX711.PD_SCK- pin # A0

 

HX711 scale (A1, A0); // parameter "gain" is ommited.
//****************************************************************************

void setup() {
 for (int i = 2; i <= 7; i++){pinMode(i,INPUT);}//SET ALL PINS 2-7 AS INPUTS
 for (int i = 8; i <= 11; i ++){pinMode(i,OUTPUT);}//SET ALL PINS 8-11 AS OUTPUTS
 pinMode(clean, OUTPUT);
 pinMode(okay, OUTPUT);
 pinMode(adultered,OUTPUT);
 //setup LCD
 //************************************************************************************

    lcd.setBacklightPin(3,POSITIVE);
    lcd.setBacklight(LOW);
    lcd.begin(16,2);
 //***********************************************************************************
  Serial.begin (38400);
  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare(); 
  
  //Display system logo at the first Row
   lcd.setCursor(0, 0);
   lcd.print(systemLogo);
   lcd.clear();
}
void loop() {
   if (buttonCheck(2) == 2){petrol = true;parrafin = false;diesel = false;water = false;
   lcd.clear();}
   if (buttonCheck(3) == 3){diesel = true;petrol = false;parrafin = false;water = false;
   lcd.clear();}
   if (buttonCheck(4) == 4){parrafin = true;petrol = false;diesel = false;water = false;
   lcd.clear();}
   if (buttonCheck(5) == 5){water = true;parrafin = false;diesel = false;petrol = false;
   lcd.clear();}

  if(petrol){
    display_var = "PETROL: ";
    density = mass()/set_volume;
    float density_diff = density_petrol - density;
    density_diff = sqrt((density_diff*density_diff));
    if (density_diff > 0.1){condition = "ALTRD";}
    else{condition = "OKAY..";}   
  }
  if(diesel){
    display_var = "DIESEL: ";
    density = mass()/set_volume;
    float density_diff = density_diesel - density;
    density_diff = sqrt((density_diff*density_diff));
    if (density_diff > 0.1){condition = "ALTERD";}
    else{condition = "OKAY..";}   
    }
  if(parrafin){
    display_var = "KEROSENE: ";
    density = mass()/set_volume;
    float density_diff = density_parrafin - density;
    density_diff = sqrt((density_diff*density_diff));
    if (density_diff > 0.1){condition = "ALTRD";}
    else{condition = "OKAY..";}     
    }
  if(water){
    display_var = "WATER: ";
    density = mass()/set_volume;
    float density_diff = density_water - density;
    density_diff = sqrt((density_diff*density_diff));
    if (density_diff > 0.1){condition = "ALTRD";}
    else{condition = "OKAY..";}                 
    }
  
   lcd.setCursor(0,0);
   lcd.print(display_var);
   lcd.setCursor(0,1);
   Serial.println(mass());
   lcd.print(density);
   lcd.print("g/cm3");
   lcd.setCursor(10,1); 
   lcd.print(condition);
}
//*****************************************METHODS
//BITTONCHECK METHOD CHECK IF PIN HAS BEEN PRESSED
int buttonCheck(int buttonPin1){
   if(digitalRead(buttonPin1) == 0){
        delay(50);
          if(digitalRead(buttonPin1) == 0){
             return buttonPin1;
            }else{return 0;}
      }
}
float mass(){
  //get the reading from the scale
  float reading = scale.get_units()*2.256;
  for (int i = 0; i <= 800; i ++){total_weight += reading;count++;}
  if(count > 800){avg_weight = total_weight/800;total_weight=0;count=0;}
  //reading = reading + 44;
  if(avg_weight < 0){avg_weight = 0;}

  scale.power_down();              // put the ADC in sleep mode
  delay(100);
  scale.power_up();

    //get value from the sensor
  return avg_weight;
  }
