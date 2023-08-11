// Each EPSC is triggered by a LOW --> HIGH transition at epscTriggerPin.
// The two-stage kinetic scheme is similar to how NMDA currents are handled 
// in Walcott, Higgins, and Desai, J. Neurosci. (2011).

volatile float xEPSC_CH1 = 0.0;
volatile float xEPSC_CH2 = 0.0;


// Increment xEPSC by 1 every time a trigger arrives at epscTriggerPin
void UpdateEpscTrain_CH1() {
  xEPSC_CH1 += 1;
}
void UpdateEpscTrain_CH2() {
  xEPSC_CH2 += 1;
}

// Calculate the net EPSC current at every time step
float EPSC_CH1(float v) {
  const float tauX = 1.0;                       // msec, rise time
  const float tauS = 10.0;                      // msec, decay time
  const float alphaS = 1.0;                     // number/msec, saturation level
  static float s_CH1 = 0.0;           
  xEPSC_CH1 = xEPSC_CH1 + dt * (-xEPSC_CH1/tauX);           
  s_CH1 = s_CH1 + dt * (-s_CH1/tauS + alphaS*xEPSC_CH1*(1-s_CH1));  // forward Euler method
  float current = -gEPSC_CH1 * s_CH1 * (v-0);           // reversal potential 0 mV
  return current;
}

float EPSC_CH2(float v) {
  const float tauX = 1.0;                       // msec, rise time
  const float tauS = 10.0;                      // msec, decay time
  const float alphaS = 1.0;                     // number/msec, saturation level
  static float s_CH2 = 0.0;           
  xEPSC_CH2 = xEPSC_CH2 + dt * (-xEPSC_CH2/tauX);           
  s_CH2 = s_CH2 + dt * (-s_CH2/tauS + alphaS*xEPSC_CH2*(1-s_CH2));  // forward Euler method
  float current = -gEPSC_CH2 * s_CH2 * (v-0);           // reversal potential 0 mV
  return current;
}
