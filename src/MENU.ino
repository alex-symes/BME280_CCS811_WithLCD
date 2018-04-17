


void showMenu(const char menuList[][NBCHAR_X]) {
  //lcd.LCD_clear();
 

  lcd.LCD_write_string(CENTER(projectName), 0, projectName, MENU_NORMAL);

 for(short int i = 0; i <= max_menu_item - min_menu_item ; i++)
  {
    lcd.LCD_write_string(CENTER(menuList[i]), OFFSET_Y + i, menuList[i], MENU_NORMAL);
  }

  lcd.LCD_write_string(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_HIGHLIGHT);
}



void browseMenu(const char menuList[][NBCHAR_X], FONCTION *menuFunction)
{
  static unsigned long int timer = 0;

  longPress = false;

  updateButtonStatus();

  if(millis() - timer > TIMEBACKLIGHT)
  {
    turnBacklightOn(false);
    timer = 0;
  }

  for(short int i = 0; i < NUM_KEYS; i++)
  {
    if(button_flag[i] != 0)
    {
      button_flag[i] = 0;

      turnBacklightOn(true);
      timer = millis();

      switch(i)
      {
        case UP_KEY:
           lcd.LCD_write_string(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_NORMAL);

          current_menu_item--;

          if(current_menu_item < min_menu_item)
          {
            current_menu_item = max_menu_item;
          }

           lcd.LCD_write_string(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_HIGHLIGHT);
          break;

        case LEFT_KEY:
           lcd.LCD_write_string(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_NORMAL);

          current_menu_item = min_menu_item;

           lcd.LCD_write_string(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_HIGHLIGHT);
          break;

        case RIGHT_KEY:
           lcd.LCD_write_string(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_NORMAL);

          current_menu_item = max_menu_item;

           lcd.LCD_write_string(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_HIGHLIGHT);
          break;

        case DOWN_KEY:
           lcd.LCD_write_string(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_NORMAL);

          current_menu_item++;

          if(current_menu_item > max_menu_item)
          {
            current_menu_item = min_menu_item;
          }

           lcd.LCD_write_string(CENTER(menuList[current_menu_item]), OFFSET_Y + current_menu_item, menuList[current_menu_item], MENU_HIGHLIGHT);
          break;

        case CENTER_KEY:
          do
          {
            updateButtonStatus();

          } while(button_status[CENTER_KEY] != 0);
           //while(button_status[CENTER_KEY] != 0 && millis() - timer < TIMELONGPRESS);

          if(millis() - timer < TIMELONGPRESS)
          {
           // lcd.LCD_clear();
             //lcd.LCD_write_string(CENTER("OK"), 5, "OK", MENU_HIGHLIGHT);

            (*menuFunction[current_menu_item])();

            String sMenu = menuList[current_menu_item];
          
            if (sMenu!="Read") {
              lcd.LCD_clear();
              showMenu(menuList);
              i = NUM_KEYS;
              break;
            }
            
            timer = millis();
            button_status[CENTER_KEY] = 0;

            do
            {
              updateButtonStatus();
            
             if (sMenu=="Read") {
             // lcd.LCD_clear();
                 (*menuFunction[current_menu_item])();
              }
            } while(button_status[CENTER_KEY] == 0);
            //while(button_status[CENTER_KEY] == 0 && millis() - timer < TIMEINFCT);

            if(button_status[CENTER_KEY] != 0)
            {
              timer = millis();
            } 
             lcd.LCD_clear();
            showMenu(menuList);
            i = NUM_KEYS;
          } else
          {
            longPress = true;
            timer = millis();
          }
          break;

        default:
          break;
      }
    }
  }
}



void turnBacklightOn(bool switchOn)
{
  if(switchOn)
  {
    digitalWrite(LCD_BACKLIGHT, HIGH);
  }

  else
  {
    digitalWrite(LCD_BACKLIGHT, LOW);
  }
}
