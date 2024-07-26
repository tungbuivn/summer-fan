#include "i2c-scan.h"
#include <Wire.h>


I2CScan::I2CScan() {
    // type=
    Wire.begin();
//  Wire.setSDA(SDA);
//     Wire.setSCL(SCL);
   
}
void I2CScan::execute(){

 
  byte error, address;
  int nDevices;
 
//   Wire.beginTransmission(0x3f);       // transmit to device 0x3f
//   Wire.write(0x00);                  // Co=0,A0=0. data= Co-A0-0-0-0-0-0-0. 
//   Wire.write(226);                   // sends restart command. 
//   Wire.endTransmission(); 
  // Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  // if (nDevices == 0)
  //   // Serial.println("No I2C devices found\n");
  // else
  //   Serial.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
};