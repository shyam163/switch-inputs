/* 
 * This file is part of (https://github.com/xxxx ).
 * Copyright (c) 2021 Shyam KJ.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY . See the GNU* General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Wire.h>
#include <Arduino.h>  
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "BluetoothSerial.h"
#include "Button2.h"; //  https://github.com/LennartHennigs/Button2
#include "ESPRotary.h";
#include <IRremote.h>

//############ change this section according to the remote used ###################################################

#define IR_RECIEVER_PIN 15 // pin connected to IR reciever
#define IR_CODE_PHONO 0x20DF8877 //IR code for phono
#define IR_CODE_CD 0x20DF48B7 //IR code for cd
#define IR_CODE_TAPE1 0x20DFC837 //IR code for tape1
#define IR_CODE_TAPE2 0x20DF28D7 //IR code for tape2



//############ change this section according to the ecoder used and pin assignment ###############################

#define ROTARY_PIN1  17    //pin connected to dt of encoder 
#define ROTARY_PIN2  16    //pin connected to clk of encoder 
#define BUTTON_PIN   5     //pin connected to the encoder button
#define CLICKS_PER_STEP   2   // this number depends on your rotary encoder \\

//############ change this section according to the motor controller  used and pin assignment  for relays  #######

#define PHONO_PIN_1 25
#define PHONO_PIN_2 26
#define CD_PIN_1    27
#define CD_PIN_2    28
#define TAPE1_PIN_1 32
#define TAPE1_PIN_2 33
#define TAPE2_PIN_1 34
#define TAPE2_PIN_2 35

//#################################################################################################################

IRrecv reciever(IR_RECIEVER_PIN);
BluetoothSerial SerialBT;
SSD1306Wire display(0x3c,5, 4);
ESPRotary r;
Button2 b;
decode_results results;

unsigned rotor=1;
int blue=1,irinputs=1;

void setup() {
  
  //Initialize Serial initerface for PC deugging
  Serial.begin(115200);
  
  //Initialize Bluetooth 
  SerialBT.begin("x audio"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  Serial.println();
  Serial.println();
  
  //initialize display
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);

  //deafult input
  setinput(1);
  
  //Initialise Pins for relay control
  pinMode(PHONO_PIN_1, OUTPUT); 
  pinMode(PHONO_PIN_2, OUTPUT);
  pinMode(CD_PIN_1, OUTPUT);
  pinMode(CD_PIN_2, OUTPUT);
  pinMode(TAPE1_PIN_1, OUTPUT);
  pinMode(TAPE1_PIN_2, OUTPUT);
  pinMode(TAPE2_PIN_1, OUTPUT);
  pinMode(TAPE2_PIN_2, OUTPUT);

  //initialise rotary encoder
  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);

  //Initialize button of the rotary encoder
  b.begin(BUTTON_PIN);
  b.setTapHandler(click);
  b.setLongClickHandler(resetPosition);

  //Initialize IR reciever
  reciever.enableIRIn();
}

void loop() {
  
  //recieve ir
  if(reciever.decode(&results)){
    irinputs=results.value;
    Serial.println(irinputs,HEX);
    switch (irinputs)
         {
          case IR_CODE_PHONO:
               setinput(1);
               break;
           case IR_CODE_CD:
               setinput(2);
               break;
           case IR_CODE_TAPE1:
               setinput(3);
               break;
           case IR_CODE_TAPE2 0x20DF28D7:
               setinput(4);
               break;
         }
    reciever.resume();
  }
  
  r.loop();
  b.loop();
  
  //write through serial to bluetooth
  //if (Serial.available()) {
  //SerialBT.write(Serial.read());
  //}
 
  //read from bluetooth serial
  if(SerialBT.available()) {
  blue=SerialBT.read();
  Serial.write(blue);
  setinput(blue);   //call function to cahnge input according to bluetooth input
  }
  

  delay(10);
}


//turns everything off before changing to a differnt state , probably :P, i dont really know the bahaviour
//of bistable latching relays,or have them in my possession, please tweak this area accordingly .
void turnoff()
{
  digitalWrite(PHONO_PIN_1, LOW);
  digitalWrite(PHONO_PIN_2, LOW);
  digitalWrite(CD_PIN_1, LOW);
  digitalWrite(CD_PIN_2, LOW);
  digitalWrite(TAPE1_PIN_1, LOW);
  digitalWrite(TAPE1_PIN_2, LOW);
  digitalWrite(TAPE2_PIN_1, LOW);
  digitalWrite(TAPE2_PIN_2, LOW);
}

//function to handle rotation
void rotate(ESPRotary& r) {
  rotor=r.getPosition()%5;
   Serial.println(rotor);
   setinput(rotor); //call function to cahnge input according to encoder input
}

//function to change inputs and display on screen
void setinput(int inputs)
{
     switch (inputs)
   {
    case 1: // code to be executed if n = 1;
        display.clear();
        display.drawString(0, 26, "   PHONO");
        display.display();
        turnoff();
        digitalWrite(PHONO_PIN_1, HIGH); 
        digitalWrite(PHONO_PIN_2, LOW); 
        break;
    case 2: // code to be executed if n = 2;
        display.clear();
        display.drawString(0, 26, "      CD");
        display.display();
        turnoff();
        digitalWrite(CD_PIN_1, HIGH); 
        digitalWrite(CD_PIN_2, LOW);
        break;
    case 3: // code to be executed if n = 3;
        display.clear();
        display.drawString(0, 26, "   TAPE 1");
        display.display();
        digitalWrite(TAPE1_PIN_1, HIGH); 
        digitalWrite(TAPE1_PIN_2, LOW);
        break;
    case 4: // code to be executed if n = 4;
        display.clear();
        display.drawString(0, 26, "   TAPE 2");
        display.display();
        digitalWrite(TAPE2_PIN_1, HIGH); 
        digitalWrite(TAPE2_PIN_2, LOW);
        break;
    default: // code to be executed if n doesn't match any cases
        //display.clear();
        //display.drawString(0, 26, "input 3");
        //digitalWrite(TAPE2_PIN_1, HIGH); 
        //digitalWrite(TAPE2_PIN_2, LOW);
        //display.display();
        ;
   }
}

// on left or right rotation
void showDirection(ESPRotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
}
 
// single click
void click(Button2& btn) {
  Serial.println("Click!");
}

// long click
void resetPosition(Button2& btn) {
  r.resetPosition();
  Serial.println("Reset!");
}
