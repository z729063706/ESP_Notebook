#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

String replaceCRLF(String input);
LiquidCrystal_I2C lcd(0x27, 16, 2); 
String messageNow1 = ""; 
String messageNow2 = "";
String messageNow = "";


void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  if (Serial.available() > 0) {
    String message = Serial.readString();
    message = replaceCRLF(message);
    for (int i = 0; i < message.length(); i++) {
      char ch = message[i];
      if (ch == '\b') {
        if (messageNow.length() > 0) {
          messageNow.remove(messageNow.length() - 1);
        }
      }
      else if (ch == '\n' || ch == '\r') {
        int j = 16-messageNow.length()%16;
        for (int i = 0; i < j; i++) {
          //messageNow += "_";
          messageNow += " ";
        } 
        //messageNow += ']';
      }
      else {
        messageNow += ch;
      }
    }
    if (messageNow.length() > 32) {
      messageNow1 = messageNow.substring(messageNow.length()-messageNow.length()%16-16, messageNow.length()-messageNow.length()%16);
      messageNow2 = messageNow.substring(messageNow.length()-messageNow.length()%16, messageNow.length());
    }
    else if (messageNow.length() > 16) {
      messageNow1 = messageNow.substring(0, 16);
      messageNow2 = messageNow.substring(16, messageNow.length());
    }
    else {
      messageNow1 = messageNow;
      messageNow2 = "";
    }
    

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(messageNow1);
    lcd.setCursor(0, 1);
    lcd.print(messageNow2);
    Serial.println(messageNow+' '+messageNow.length());
    Serial.println(messageNow1+'\n'+messageNow2);
  }
  
  delay(100); 
}
String replaceCRLF(String input) {
  String output = "";
  for (int i = 0; i < input.length(); i++) {
    if (input[i] == '\r' && i + 1 < input.length() && input[i + 1] == '\n') {
      output += '\n';
      i++;
    } else {
      output += input[i];
    }
  }
  return output;
}