// PCF8574 Binary Counter Test
// Hari Wiguna, 2016

#include <Wire.h>

uint8_t error, address, curVal, val;

void OutputDemo()
{
  for (val = 0; val <= 255; val++)
  {
    Serial.print(val);
    Serial.print(" ");
    Wire.beginTransmission(address);
    Wire.write(~val);
    error = Wire.endTransmission();
    Serial.println(error);
    delay(200);
  }
}

void InputDemo()
{
  //-- To read we need to first set all outputs to HIGH --
  Wire.beginTransmission(address);
  Wire.write(0xFF); // Get ready to read all bits
  error = Wire.endTransmission();

  //-- Read all switches --
  Wire.beginTransmission( address );
  Wire.requestFrom((int)address, 1); // Ask for 1 byte from slave
  val = ~Wire.read(); // read that one byte
  error = Wire.endTransmission();

  Serial.print(val, BIN);
  Serial.print(" ");
  Serial.println(error);
  delay(500);
}

void IODemo()
{
  //-- Input --
  Wire.beginTransmission( address );
  Wire.write(0xFF); // Get ready to read all bits
  Wire.requestFrom((int)address, 1); // Ask for 1 byte from slave
  val = Wire.read(); // read that one byte
  error = Wire.endTransmission();

  Serial.println(val, BIN);

  //-- Process --
  uint8_t newVal = 0x0F;
  if (bitRead(val, 3) == 0) bitClear(newVal, 7); else bitSet(newVal, 7);
  if (bitRead(val, 2) == 0) bitClear(newVal, 6); else bitSet(newVal, 6);
  if (bitRead(val, 1) == 0) bitClear(newVal, 5); else bitSet(newVal, 5);
  if (bitRead(val, 0) == 0) bitClear(newVal, 4); else bitSet(newVal, 4);

  //-- Output --
  Wire.beginTransmission(address);
  Wire.write(newVal);
  error = Wire.endTransmission();
  delay(10); // let humans
}

void ToggleDemo()
{
  //-- To read we need to first set all outputs to HIGH --
  Wire.beginTransmission(address);
  Wire.write(0xFF); // Get ready to read all bits
  error = Wire.endTransmission();

  //-- Read all switches --
  Wire.beginTransmission( address );
  Wire.requestFrom((int)address, 1); // Ask for 1 byte from slave
  val = Wire.read(); // read that one byte
  error = Wire.endTransmission();

  //Serial.print(val, HEX);
  //Serial.print(" ");
  if ( val != 0xFF) // Unless they're all high...
  {
    //-- Process --
    //Serial.print(curVal, BIN);
    //Serial.print(" ");
    curVal ^= ~val;  // Invert every switch that is pressed (LOW).
    //Serial.print(curVal, BIN);
    //Serial.print(" ");
  }

  //-- Wait for button release --
  while ( val != 0xFF )
  {
    Wire.beginTransmission( address );
    Wire.requestFrom((int)address, 1); // Ask for 1 byte from slave
    val = Wire.read(); // read that one byte
    error = Wire.endTransmission();
  }

  //Serial.println();

  //-- Output --
  Wire.beginTransmission(address);
  Wire.write(curVal);
  error = Wire.endTransmission();
  delay(100); // Allow humans to see curVal
}



void ExplosionDemo()
{
  //-- To read we need to first set all outputs to HIGH --
  Wire.beginTransmission(address);
  Wire.write(0xFF); // Get ready to read all bits
  error = Wire.endTransmission();

  //-- Read all switches --
  Wire.beginTransmission( address );
  Wire.requestFrom((int)address, 1); // Ask for 1 byte from slave
  val = Wire.read(); // read that one byte
  error = Wire.endTransmission();

  //Serial.print(val, HEX);
  //Serial.print(" ");
  if ( val != 0xFF) // Unless they're all high...
  {
    //-- Process --
    //Serial.print(curVal, BIN);
    //Serial.print(" ");
    curVal ^= ~val;  // Invert every switch that is pressed (LOW).
    //Serial.print(curVal, BIN);
    //Serial.print(" ");
  }

  //-- Wait for button release --
  while ( val != 0xFF )
  {
    Wire.beginTransmission( address );
    Wire.requestFrom((int)address, 1); // Ask for 1 byte from slave
    val = Wire.read(); // read that one byte
    error = Wire.endTransmission();
  }

  //Serial.println();

// Find lowest pressed switch --
int sw = -1;
for (byte b=0; b<8; b++)
if (bitRead(curVal,b)==0) {
  sw = b;
  curVal=0xFF;
  exit;
}
  //=== Explode ===
  if(sw!=-1)
  for (int i=0; i<8; i++)
  {
  uint8_t oo = 0xFF;
    int left = sw-i;
    int rite = sw+i;
    if (left>=0)  bitClear(oo,left);
    if (rite<=7)  bitClear(oo,rite);
  
  //-- Output --
  Wire.beginTransmission(address);
  Wire.write(oo);
  error = Wire.endTransmission();
  
  delay(70); // Allow humans to see curVal
  
   Wire.beginTransmission(address);
  Wire.write(0xFF);
  error = Wire.endTransmission();
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Arduino needs (SDA=A4,SCL=A5)
  //Wire.begin(0,2); // ESP8266 needs (SDA=GPIO0,SCL=GPIO2)
  address = 0x20;
  curVal = 0xFF; // all off

  //OutputDemo();
}

void loop() {
  //InputDemo();
  //IODemo();
  //ToggleDemo();
  ExplosionDemo();
}

//  //-- Show current bit states --
//  Wire.beginTransmission( address );
//  Wire.write(~curVal);
//  error = Wire.endTransmission();
//delay(2000);
//  Serial.println("Waiting for keypress..");
//
//  //do
//  {
//    Wire.beginTransmission( address );
//    Wire.write(0xFF); // Get ready to read all bits
//    Wire.requestFrom((int)address, 1); // Ask for 1 byte from slave
//    val = Wire.read(); // read that one byte
//    error = Wire.endTransmission();
//
//    Wire.beginTransmission( address );
//    Wire.write(~curVal);
//    error = Wire.endTransmission();
//
//  } //while (val == 0xFF);
//
//  Serial.print(val, BIN);
//  Serial.print(" ");
//
//  for (byte b = 0; b < 8; b++)
//  {
//    if (bitRead(val, b) == 0) {
//      Serial.print("* ");
//      if (bitRead(curVal, b) == 0) {
//        bitSet(curVal, b);
//      }
//      else {
//        bitClear(curVal, b);
//      }
//      delay(1000);
//    }
//  }
//
//  Serial.println(curVal, BIN);
//  delay(500);


