void setup()
{
  Serial.begin(9600);
  //Serial.println("Enter a number to multiply by 2.");
}

char rx_byte = 0;
String rx_str = "";
boolean not_number = false;
int i = 1;
int result;

void loop()
{
  if (Serial.available() > 0)
  {                          // is a character available?
    rx_byte = Serial.read(); // get the character

    if ((rx_byte >= '0') && (rx_byte <= '9'))
    {
      rx_str += rx_byte;
    }
    else if (rx_byte == '\n')
    {
      // end of string
      if (not_number)
      {
        // Serial.println("Not a number");
      }
      else
      {
        rx_str += rx_byte;
      }
    }
    result = rx_str.toInt();
    //Serial.println(result);
  } // end: if (Serial.available() > 0)
  switch (result)
  {
  case 201:
    Serial.println("got 201");
    rx_str = "";
    result = 0;
    break;
  case 202:
    Serial.println("got 202");
    rx_str = "";
    result = 0;
    break;
  case 301:
    Serial.println("got 301");
    rx_str = "";
    result = 0;
    break;
  case 302:
    Serial.println("got 302");
    rx_str = "";
    result = 0;
    break;
  default:
    if (rx_str.length() > 3)
    {
      rx_str = "";
      result = 0;
    }
    break;
  }
}