#include <M5StickC.h>
#include <PoweredUpHub.h>
#include <Wire.h>
#define JOY_ADDR 0x38

int8_t x_data,y_data,button_data;

PoweredUpHub myHub;
PoweredUpHub::Port _portA = PoweredUpHub::Port::A;
PoweredUpHub::Port _portB = PoweredUpHub::Port::B;

boolean powerup_ok    = false;
int     motor_speed   = 80;
int     dead_zone_x   = 50;
int     dead_zone_y   = 20;
int     bad_value     = -128;

void setup() {
  // initialize the display
  M5.begin();
  Wire.begin(0, 26, 100000);
  myHub.init(); 

  Wire1.beginTransmission(0x34); // Found on the community forum
  Wire1.write(0x12);
  Wire1.write(0b01001011);
  Wire1.endTransmission();    
}

void loop() {  
  powerup_ok = myHub.isConnected();
  Wire.beginTransmission(JOY_ADDR);
  Wire.write(0x02); 
  Wire.endTransmission();
  Wire.requestFrom(JOY_ADDR, 3);

  if (!powerup_ok)
  {
    if (myHub.isConnecting()) {
      myHub.connectHub();  
    }
  }
  else  
    if (Wire.available()) {
      x_data = Wire.read();
      y_data = Wire.read();
      button_data = !Wire.read();
/*
      if (button_data) {
          myHub.setLedColor(LEGO_RED);
          
      }
      else {          
          myHub.setLedColor(LEGO_GREEN);                   
      }  
*/
      
      if (x_data != bad_value)
        if (x_data > dead_zone_x)
        {
          myHub.setMotorSpeed(_portA, -motor_speed);
          myHub.setMotorSpeed(_portB, -motor_speed);
        }
        else
        if (x_data < -dead_zone_x)
        {
          myHub.setMotorSpeed(_portA,  motor_speed);
          myHub.setMotorSpeed(_portB,  motor_speed);        
        }
        else
          if (y_data != bad_value)
            if (y_data > dead_zone_y)
            {
              myHub.setMotorSpeed(_portA,  motor_speed);
              myHub.setMotorSpeed(_portB, -motor_speed);
            }
            else
            if (y_data < -dead_zone_y)
            {
              myHub.setMotorSpeed(_portA, -motor_speed);
              myHub.setMotorSpeed(_portB,  motor_speed);        
            }
            else
            {
              myHub.setMotorSpeed(_portA,  0);
              myHub.setMotorSpeed(_portB,  0);                
            }      
    }
  delay(50);  
}
