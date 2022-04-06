void drive(int fL, int fR, int rL, int rR){
  analogWrite(forwardLeft, fL);
  analogWrite(forwardRight, fR);
  analogWrite(reverseLeft, rL);
  analogWrite(reverseRight, rR);
}

void moveForward(int speedLeft, int speedRight) {
   drive(speedLeft, speedRight, LOW, LOW);
}

void moveBackwards(int speedLeft, int speedRight) {
  drive(LOW, LOW, speedLeft, speedRight);
}

void turnRight(int speedRight) {
  drive(LOW, speedRight, LOW, LOW);
}

void turnLeft(int speedLeft) {
  drive(speedLeft, LOW, LOW, LOW);
}

void stopVehicle() {
  drive(LOW, LOW, LOW, LOW);
}
