bool isLongPress(void)
{
  return longPress;
}



char getKey(unsigned int input)
{
  short int k;
  static short int adc_key_val[NUM_KEYS] = {50, 200, 400, 600, 800};

  for(k = 0; k < NUM_KEYS; k++)
  {
    if(input < adc_key_val[k])
    {
      return k;
    }
  }

  return -1;
}


void updateButtonStatus(void)
{
  char key_in = getKey(analogRead(0));
  //Serial.print("key");
  //Serial.println(key_in);
  for(short int i = 0; i < NUM_KEYS; i++)
  {
    if(key_in == i)
    {
      if(button_count[i] < DEBOUNCE_MAX)
      {
        button_count[i]++;

        if(button_count[i] > DEBOUNCE_ON)
        {
          if(button_status[i] == 0)
          {
            button_flag[i] = 1;
            button_status[i] = 1;
          }
        }
      }
    }

    else
    {
      if(button_count[i] > 0)
      { 
        button_flag[i] = 0;
        button_count[i]--;

        if(button_count[i] < DEBOUNCE_OFF)
        {
          button_status[i] = 0;
        }
      }
    }
  }
}


