/* 
 * This file is part of the XXX distribution (https://github.com/xxxx or http://xxx.github.io).
 * Copyright (c) 2021 Shyam KJ.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
SSD1306Wire display(0x3c,5, 4);
int input =1;
char blue;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("x audio"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  Serial.println();
  Serial.println();
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  pinMode(25, OUTPUT); 
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
}

void loop() {
  
  //write through serial to bluetooth
  //if (Serial.available()) {
  //SerialBT.write(Serial.read());
  //}
  if (SerialBT.available()) {
  blue=SerialBT.read();
  Serial.write(blue);
  }
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);
  
  switch (blue)
   {
    case '1': // code to be executed if n = 1;
        display.clear();
        display.drawString(0, 26, "input 1");
        turnoff();
        digitalWrite(25, HIGH); 
        digitalWrite(26, LOW); 
        break;
    case '2': // code to be executed if n = 2;
        display.clear();
        display.drawString(0, 26, "input 2");
        turnoff();
        digitalWrite(27, HIGH); 
        digitalWrite(28, LOW);
        break;
    case '3': // code to be executed if n = 3;
        display.clear();
        display.drawString(0, 26, "input 3");
        digitalWrite(32, HIGH); 
        digitalWrite(33, LOW);
        break;
    case '4': // code to be executed if n = 4;
        display.clear();
        display.drawString(0, 26, "input 3");
        digitalWrite(34, HIGH); 
        digitalWrite(35, LOW);
        break;
    default: // code to be executed if n doesn't match any cases
        display.clear();
        display.drawString(0, 26, "input 3");
        digitalWrite(34, HIGH); 
        digitalWrite(35, LOW);
   }

  
  display.display();
  
  delay(10);
}


void turnoff()
{
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);
  digitalWrite(27, LOW);
  digitalWrite(28, LOW);
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  digitalWrite(34, LOW);
  digitalWrite(35, LOW);
}
