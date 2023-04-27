// A constant conductance with a reversal potential of -70 mV
void trig_out() {
  digitalWrite(startTriggerPin, HIGH);
  delay(20);
  digitalWrite(startTriggerPin, LOW);
}
