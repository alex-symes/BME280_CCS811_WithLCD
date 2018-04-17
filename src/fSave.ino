void fSave(void) {
    delay(displayDelay);
          lcd.LCD_clear(); // blanks the display
          delay(500);
          lcd.LCD_write_string(0, 1, "Saving New Baseline", MENU_NORMAL);
           delay(1000);
           lcd.LCD_clear(); // blanks the display
         
           //This gets the latest baseline from the sensor
              result = myCCS811.getBaseline();
              Serial.print("baseline for this sensor: 0x");
              lcd.LCD_write_string(0, 1, "baseline for this sensor:", MENU_NORMAL);
              
              String sResult = String(result, HEX);
              if (result < 0x100) { 
                Serial.print("0");
                sResult = String("0" + sResult);
              }
              if (result < 0x10) {
                Serial.print("0");
                sResult = String("0" + sResult);
              }
              sResult = String("0x" + sResult);
              Serial.println(result, HEX);
              Serial.println(result);
              Serial.print("sResult: ");
              Serial.println(sResult);
                
                int n = sResult.length();
                char cResult[n+1]; 
                strcpy(cResult, sResult.c_str()); 
    
              lcd.LCD_write_string(0, 3, cResult, MENU_NORMAL);
              //The baseline is saved (with valid data indicator bytes)
              EEPROM.write(0, 0xA5);
              EEPROM.write(1, 0xB2);
              EEPROM.write(2, (result >> 8) & 0x00FF);
              EEPROM.write(3, result & 0x00FF);


          delay(2000);
          lcd.LCD_clear(); // blanks the display
  }
  

