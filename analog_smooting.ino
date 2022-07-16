// gauge set up values
float gaugeValue = 0;
// the A2D converter generates a value between 0 and 1023
// long is sensible choice for the average
long gaugeCount = 0;
float gauge_reading()                       
{
  gaugeCount = 0;
  // take 100 readings
  for (int i=0; i < ANALOG_READ_SAMPLES; i++)
  {
    gaugeValue = analogRead(BATT_LEVEL) / 209.66;
    gaugeCount += gaugeValue;
    delay(10);
  }
  // spit out an average
  float average = gaugeCount / ANALOG_READ_SAMPLES;
  return average;
}
