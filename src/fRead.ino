


void fRead(void) { 
      unsigned long currentMillis = millis();

     
     if (currentMillis - previousMillis >= interval) { //this if is alternative to delay() - used to keep buttonstatus() working
        // save the last time you blinked the LED
        previousMillis = currentMillis;
        bEndDelay = 1;
        lcd.LCD_clear();
        if (myCCS811.dataAvailable())
          {
            //Calling this function updates the global tVOC and eCO2 variables
            myCCS811.readAlgorithmResults();
            //printData fetches the values of tVOC and eCO2
            printData();
        
            float BMEtempC = myBME280.readTempC();
            float BMEhumid = myBME280.readFloatHumidity();
        

        
            //This sends the temperature data to the CCS811
            myCCS811.setEnvironmentalData(BMEhumid, BMEtempC);
          }
          else if (myCCS811.checkForStatusError())
          {
            Serial.println(myCCS811.getErrorRegister()); //Prints whatever CSS811 error flags are detected
          }
        
       
      } 
    


  }

  

