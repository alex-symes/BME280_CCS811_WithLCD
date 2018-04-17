
/*-----( Declare User-written Functions )-----*/
void printData() {
   char buffer[10];
    time = millis();
    Serial.print("DATA,DATE,TIME");
    //Serial.print(time/1000);
    String sCO2 = String("CO2: ") + dtostrf(myCCS811.getCO2(),5,1,buffer);
    String sTVOC = String("TVOC: ") + dtostrf(myCCS811.getTVOC(),5,1,buffer);
    String sTempF = String("Temp ") + dtostrf(myBME280.readTempF(),5,1,buffer) + "F";
    String sPressure = String("Pressure: ") + dtostrf(myBME280.readFloatPressure(),5,1,buffer);
    String sHumid = String("Humid: ") + dtostrf(myBME280.readFloatHumidity(),5,1,buffer);
   
    int n = sCO2.length();
    char cCO2[n+1]; 
    strcpy(cCO2, sCO2.c_str()); 
    
    n = sTVOC.length();
    char cTVOC[n+1]; 
    strcpy(cTVOC, sTVOC.c_str()); 

    n = sTempF.length();
    char cTempF[n+1]; 
    strcpy(cTempF, sTempF.c_str()); 

    n = sPressure.length();
    char cPressure[n+1]; 
    strcpy(cPressure, sPressure.c_str()); 

    n = sHumid.length();
    char cHumid[n+1]; 
    strcpy(cHumid, sHumid.c_str()); 


    lcd.LCD_write_string(0, 0, "Reading Data",MENU_NORMAL);
    lcd.LCD_write_string(0, 1, cCO2 , MENU_NORMAL);
    lcd.LCD_write_string(0, 2, cTVOC, MENU_NORMAL);    
    lcd.LCD_write_string(0, 3, cTempF, MENU_NORMAL);
    lcd.LCD_write_string(0, 4, cPressure , MENU_NORMAL);
    lcd.LCD_write_string(0, 5, cHumid , MENU_NORMAL);

     Serial.print(",");
     Serial.print(myCCS811.getCO2());
     Serial.print(",");
     Serial.print(myCCS811.getTVOC());
     Serial.print(",");
     Serial.print(myBME280.readTempF(), 1);

     Serial.print(",");
     Serial.print(myBME280.readFloatPressure(), 2);

     Serial.print(",");
     Serial.print(myBME280.readFloatHumidity(), 0);
     Serial.println();
  
  //  lcd.LCD_write_string(0, 1, dtostrf(myCCS811.getCO2(),5,1,buffer) , MENU_NORMAL);

  //  lcd.LCD_write_string(0, 2,  dtostrf(myCCS811.getTVOC(),5,1,buffer), MENU_NORMAL);

    //Serial.print(" temp[");
    //Serial.print(myBME280.readTempC(), 1);
    //Serial.print("]C");


    //Come back to these 
  

    //Serial.print(" pressure[");
    //Serial.print((myBME280.readFloatPressure() * 0.0002953), 2);
    //Serial.print("]InHg");

    //Serial.print("altitude[");
    //Serial.print(myBME280.readFloatAltitudeMeters(), 2);
    //Serial.print("]m");

    //Serial.print("altitude[");
    //Serial.print(myBME280.readFloatAltitudeFeet(), 2);
    //Serial.print("]ft");

 //delay(2000); //Wait for next reading
   
//    printRunTime();

}

