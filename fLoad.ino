void fLoad(void)  {
          lcd.LCD_clear();
          if ((EEPROM.read(0) == 0xA5) && (EEPROM.read(1) == 0xB2))
                {
                  lcd.LCD_write_string(0, 0, "EEPROM contains saved data.", MENU_NORMAL);
                  Serial.println("EEPROM contains saved data.");
                  //The recovered baseline is packed into a 16 bit word
                  baselineToApply = ((unsigned int)EEPROM.read(2) << 8) | EEPROM.read(3);
                 
                  lcd.LCD_write_string(0, 2, "Previous baseline:", MENU_NORMAL);
                  Serial.print("Previous Baseline: 0x");
                  if (baselineToApply < 0x100) Serial.print("0");
                  if (baselineToApply < 0x10) Serial.print("0");
                  Serial.println(baselineToApply, HEX);
        
                 String sBaselineToApply = String(baselineToApply, HEX);
                      if (baselineToApply < 0x100) { 
                        Serial.print("0");
                        sBaselineToApply = String("0" + sBaselineToApply);
                      }
                      if (baselineToApply < 0x10) {
                        Serial.print("0");
                        sBaselineToApply = String("0" + sBaselineToApply);
                      }
                      
                sBaselineToApply = String("0x" + sBaselineToApply);
                int n = sBaselineToApply.length();
                char cBaselineToApply[n+1]; 
                strcpy(cBaselineToApply, sBaselineToApply.c_str()); 
    
                 lcd.LCD_write_string(0, 4, cBaselineToApply, MENU_NORMAL);
                  
                  //This programs the baseline into the sensor and monitors error states
                  errorStatus = myCCS811.setBaseline( baselineToApply );
                  if ( errorStatus == CCS811Core::SENSOR_SUCCESS )
                  {
                    Serial.println("Baseline written to CCS811.");
                  }
                  else
                  {
                    printDriverError( errorStatus );
                  }
                }
                else
                {
                  lcd.LCD_write_string(0, 0, "No saved data.", MENU_NORMAL);
                  Serial.println("Saved data not found!");
                }
          delay(2000);
          lcd.LCD_clear(); // blanks the display 
  }

  

  
  

