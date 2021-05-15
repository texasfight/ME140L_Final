#include <SegmentDisplay.h>

// Instantiate integers for each pin being used.

int LDR_pin = 0;
int POT_pin = 1;
int Switch_pin = 2;

// Initialize the seven-segment and map to different digital pins
SegmentDisplay segmentDisplay(10, 3, 4, 5, 6, 7, 8, 9);

void setup() {
  pinMode(Switch_pin, INPUT);
  SerialUSB.begin (38400);
}

// Instantiate values for the inputs and outputs
bool switchState;
int LDRValue, POTValue, outputValue, LDRscaled, POTscaled;

void loop() {
  // put your main code here, to run repeatedly:
  bool switchState = digitalRead(Switch_pin);

  // Addition when the switch is on
  // read the analog in values:
  LDRValue = analogRead(LDR_pin);
  POTValue = analogRead(POT_pin);
  // map it to the range where we won't go over the max value, but take it as a measure of light intensity instead of a measure of resistance:
  LDRscaled = 15 - map(LDRValue, 0, 1023, 0, 15);
  POTscaled = 15 - map(POTValue, 0, 1023, 0, 15);
  
  if (switchState == HIGH) // Switch on
  {
    // Make the output the addition of both values, but divided by two so that we can actually display the result.
    outputValue = (POTscaled + LDRscaled) / 2;
    // print the results to the Serial Monitor:
    SerialUSB.print("Addition: ");
    SerialUSB.println(outputValue);
    segmentDisplay.displayHex(outputValue, false);
  } 
  
  else // Switch off
  {
    // Use subtraction then simple logic for handling negative values
    outputValue = LDRscaled - POTscaled;
    
    if (outputValue < 0) 
    {
      // For negative values we take the additive inverse and add the decimal point so that we can acknowledge it's negative
      SerialUSB.print("Subtraction: ");
      SerialUSB.println(outputValue);
      outputValue = -outputValue;
      segmentDisplay.displayHex(outputValue, true);
    }
  
    else
    
    {
      // Print out the simple subtraction without anything else
      SerialUSB.print("Subtraction: ");
      SerialUSB.println(outputValue);
      segmentDisplay.displayHex(outputValue, false);
    }
  }
}
