
/*
Rover Corps
Gabriel Herrera, Cameron Peterson, Ismail Agsalud, Thomas Liu, Junchi (Jacky) Chen, Samuel Stout
Date of last edit: 3/11/2022 (besides the commenting of course)

The code structure consisted of 3 main sections: macro definitions, setup code, and loop code. 
In the macro definitions section, all global variables (such as pin numbers) were declared, 
initialized and given descriptions, in addition to the calling of all appropriate header files. 
The setup code involved declaring pins as inputs and outputs, as well as the initializing of 
certain variables including the serial monitor, motor, and servos. The loop code consisted of 
3 main parts: receiving input from photoresistors to navigate along the line, detection and 
retrieval of the cup, and following the line once more to the end of the course. 
The code utilizing input from the sensors to adjust the steering based on the degree of light
difference between the two photoresistors. We utilized this technique for following the line and
centering the rover to the cup in order to pick it up. Coefficients and other values were 
determined experimentally. 
 */
///stating the header files needed for functions to work
#include <Servo.h>
#include <Pixy2.h>
///stating the port number for connection (defining macros)
int servo_pin= 9; /// for steering servo
int servo_pin2= 5; ///for claw servo
int motor = 6;
int PHOTOL= A0; 
int PHOTOR = A1; 
Servo Servo1;  ///declare servo
Servo Servo2; ///declare the claw servo 
Servo DCmotor; 
Pixy2 pixy; // naming pixy2 
int SensorValL, SensorValR = 0;  ///initialize variables
int corr;

void setup()
{
Servo1.attach(servo_pin); ///servo pin...always an output
Servo2.attach(servo_pin2); ///servo pin for the claw servo 
DCmotor.attach(motor);
DCmotor.write(90); ///start with zero speed
pinMode(PHOTOL, INPUT); ///PHOTO Left is an input
pinMode(PHOTOR, INPUT); ///PHOTO Right is an input 
pinMode(motor, OUTPUT); ///motor is an output
Servo1.write(90);  ///set reference angle to 90
Servo2.write(135); /// we might need to change this, hopefully this makes the claw open 
Serial.begin(115200); ///set at for Pixycam 
Serial.print("Starting...\n");  
pixy.init(); //initalize pixy2 
delay (2000); ///time for rover to set up 
pinMode(2, OUTPUT); ///LED pin
pinMode(4, OUTPUT); ///LED2 pin
}

void loop()  ///loop for photoresistor
{
digitalWrite(2, HIGH); ///turn on LED
digitalWrite(4, HIGH); ///turn on second LED
SensorValR= analogRead(PHOTOR); ///read input from PHOTO right
SensorValL= analogRead(PHOTOL); ///read input from PHOTO left
Serial.println("PhotoR "); 
Serial.println(SensorValR); ///print photoresistor right values
Serial.println("PhotoL ");
Serial.println(SensorValL); ///print photoresistor left values
corr = SensorValL - SensorValR; /*if value is positive => L>R => L = white => turn right => greater than 90. 
if R>L => value is negative => we turn left => less than 90*/
int i; 
 
DCmotor.write(53); ///wires were connected in reverse for motor 
///0 = full speed forward, 180 = full speed backwords, 90 = stop
if (pixy.ccc.getBlocks() == 0); ///if I don't see any NEW blocks
{
  delay (20);
  if (pixy.ccc.getBlocks() == 0); ///now I know that no blocks are detected AT ALL

  {
  Servo1.write(90+corr*1); ///steering control adjustment 
  delay (10);
  ///print the values from photoresistor
  Serial.println("PhotoR "); 
  Serial.println(SensorValR);
  Serial.println("PhotoL ");
  Serial.println(SensorValL);
  }
}
if (pixy.ccc.getBlocks()!=0) ///if I see blocks
  {
    Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks); 
    for (i=0; i<pixy.ccc.numBlocks; i++) 
    {
      Serial.print("  block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();
      delay(100);
      ///we have printed the blocks
    }
   while ((pixy.ccc.getBlocks() !=0) && ((pixy.ccc.blocks[i].m_x <300) || (pixy.ccc.blocks[i].m_x > 340))) 
   //while it is not at the center
    {
      if(pixy.ccc.blocks[i].m_x > 320) //turn left
      {
        Servo1.write(90-(pixy.ccc.blocks[i].m_x - 320)*0.28125);
      }
      else if(pixy.ccc.blocks[i].m_x < 320) //turn right
      {
        Servo1.write(90+(pixy.ccc.blocks[i].m_x - 320)*0.28125);
      }
      //90%320=0.28125, this is the angle ratio we want to turn 
      ///we are adjusting the rover to be centered
    }
    while ((pixy.ccc.getBlocks() !=0) && (pixy.ccc.blocks[i].m_width >= 210)) //while the rover is close enough to the can 
    {
      DCmotor.write(90); ///DC motor is stopped as we are now in position to pick up the can
      delay(10);
      Servo2.write(55); ///closed but not fully closed (can has been picked up) 
      delay(10);
    break;
    }
    ///tested width is 250, height is 120 for now for the claw mechanism to activate , mid point at (320,200)
  }  
DCmotor.write(53); ///start moving forward again
Servo1.write(90+corr*1);  ///starts following the black line again
delay (10);
}
