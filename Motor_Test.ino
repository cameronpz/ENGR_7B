#include <Servo.h>
int motor = 6;
Servo DCmotor;
void setup() {
  DCmotor.attach(motor);
  DCmotor.write(90);

}

void loop() {
 DCmotor.write(90);
 delay(10000);
 DCmotor.write(180);
 delay(10000);
 DCmotor.write(0);
 delay(10000);

}
