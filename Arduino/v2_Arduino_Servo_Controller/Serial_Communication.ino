/*  Serial Decode

     Read Format

     20-20-400-200

     Write

*/

String inString = "";    // string to hold input char
String inString_buffer = "";    // string to hold input Serial msg
String inString_display_buffer = "";    // string to hold Display info

bool exclude_print_val = true;


int current_index = 0;//index of current decoding number
//long input_value[Input_size];  //inputs value(s) buffer -> moved to main tab
boolean update_flag = false; //Flag for end of success input string follow /n



void serial_decode()   // Read serial input: //TODO add serial1
{
  byte Read_Flag = 0;
  while (Serial.available() > 0)
  {
    int inChar = Serial.read();
    char_decode(inChar) ;
  }
  while (Serial1.available() > 0)
  {
    int inChar = Serial1.read();
    char_decode(inChar) ;
  }
/*
  while (Serial1.available() > 0)
  {
    int inChar = Serial1.read();
    char_decode(inChar) ;
  }
  */
}




void char_decode(int inChar)
{
  // Read_Flag=0;
  inString_buffer += (char)inChar;
  if (isDigit(inChar))       //if get a number
  {
    // convert the incoming byte to a char
    // and add it to the string:
    inString += (char)inChar;
    //Serial.println(inString);
  }
  else if (inChar == '-')       // if you get a comma, convert to a number,
  {
    if (current_index < Input_size) // within size
    {
      input_value[current_index] = inString.toInt(); //
      
      //  pdInput_value[current_index] = inString.toInt(); //
      current_index++;  //increase index
    }




    //            else// giveup the value to avoide over access array
    //            {
    //              Serial.println("overflow");
    //            }
    inString = "";   // clear the string buffer for new input:


  }
  else if (inChar == '\n' || inChar == 'N')       //if you get a newline, convert to a number and print all value
  {
    if (current_index < Input_size) // within size
    {
      input_value[current_index] = inString.toInt(); //final value
      val = input_value[0];

    }
    //            else// giveup the value to avoide over access array
    //            {
    //              Serial.println("overflow(end)");
    //            }


    inString = "";   // clear the string buffer for new input:
    current_index = 0;// reset the index
    //Set update_Flag
    update_flag = true;

  }
  else if (inChar == 'C') //Check if online
  {
    //DO STH HERE
    inString = "";   // clear the string buffer for new input:
    Serial.print("online-");
    Serial1.print("online-");
    update_flag = true;
    exclude_print_val = true;
  }
  else if (inChar == 'Q') //MODE1
  {
    //DO STH HERE
    inString = "";   // clear the string buffer for new input:
    Serial.println("mode_a");
    Serial1.println("mode_a");
    
  }
  else if (inChar == 'S') //SAVE EEPROM
  {

    inString = "";   // clear the string buffer for new input:
    inString_display_buffer = F("Write Config");
        for (int i = 0; i < int_array_size; i++)
        {
          int_array[i] = input_value[i];

          Serial.print("\t");
          Serial.print(i);
          Serial.print(":");
          Serial.print(int_array[i]);

          Serial1.print("\t");
          Serial1.print(i);
          Serial1.print(":");
          Serial1.print(int_array[i]);
        }
    Write_Flash();
    Load_To_Variables();
    Serial.print("save-");
    Serial1.print("save-");
    for (int i = 0; i < int_array_size; i++) {

      Serial.print(int_array[i]);
      Serial1.print(int_array[i]);
      if (i != int_array_size - 1) {
        Serial.print("-");
        Serial1.print("-");
      }
    }
    Serial.print("\t");
    Serial1.print("\t");

    exclude_print_val = true;

  }
  else if (inChar == 'L') //LOAD EEPROM
  {
    //DO STH HERE
    inString = "";   // clear the string buffer for new input:
    Load_Flash();
    Serial.print("load-");
    Serial1.print("load-");
    for (int i = 0; i < int_array_size; i++) {

      Serial.print(int_array[i]);
      Serial1.print(int_array[i]);
      if (i != int_array_size - 1) {
        Serial.print("-");
        Serial1.print("-");
      }
    }
    Serial.print("\t");
    Serial1.print("\t");
    exclude_print_val = true;

  }
    else if (inChar == 'E') //EMERGENCY STOP
  {
    //DO STH HERE
    inString = "";   // clear the string buffer for new input:
    
    Serial.print("EMERGENCY STOP ON === R to release");
    Serial1.print("EMERGENCY STOP ON === R to release");
   isEmergencyStop = true;
    Serial.print("\t");
    Serial1.print("\t");

    exclude_print_val = true;

  }

      else if (inChar == 'R') //EMERGENCY STOP
  {
    //DO STH HERE
    inString = "";   // clear the string buffer for new input:
    
    Serial.print("EMERGENCY STOP OFF");
    Serial1.print("EMERGENCY STOP OFF");
   isEmergencyStop = false;
    Serial.print("\t");
    Serial1.print("\t");

    exclude_print_val = true;

  }

}


void check_update()   //Check update flag and write value when string finish
{
  if ( update_flag == true) //update value
  {

    if (exclude_print_val == false) {
      //print all value
      for (int index = 0; index < Input_size; index++)
      {
        //Serial.print(index);
        //Serial.print(":");
        Serial.print(input_value[index]);
        Serial.print("-");
                
        Serial1.print(input_value[index]);
        Serial1.print("-");
      }
      
    }
    //Finish Serial.print, Next Line
    Serial.println("|");
    Serial1.println("|");

    exclude_print_val = false;
    update_flag = false; //clear flag
    inString_display_buffer = inString_buffer;    // string to hold input Serial msg
    inString_buffer = "";   // clear the string buffer for new input:
  }


}

