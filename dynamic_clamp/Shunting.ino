// A constant conductance with a reversal potential of -70 mV
float Shunting_CH1(float v) {
  float current = -gShunt_CH1 * (v + 70);
  return current;
}

float Shunting_CH2(float v) {
  float current = -gShunt_CH2 * (v + 70);
  return current;
}
