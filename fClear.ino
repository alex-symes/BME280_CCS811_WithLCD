void fClear(void) {
        
          delay(displayDelay);
          lcd.LCD_clear(); // blanks the display
          delay(500);
          lcd.LCD_write_string(0, 1, "Clearing ", MENU_NORMAL);
          lcd.LCD_write_string(0, 2, "EEPROM ", MENU_NORMAL);
           delay(1000);
           lcd.LCD_clear(); // blanks the display
            EEPROM.write(0, 0x00);
            EEPROM.write(1, 0x00);
            EEPROM.write(2, 0x00);
            EEPROM.write(3, 0x00);



          //  browseMenu(menuList, menuFunction);

   
  }


  
