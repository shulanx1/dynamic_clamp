// H current from model in Vaidya and Johnston, Nat. Neurosci. (2013).
// For speed, the parameters sinf and tau are pre-calculated for a wide range of Vm
// and put in lookup tables stored as global variables.

// Declare the lookup table variables
float sinf1[1501] = {0.0};                                     // Pre-calculate the activation parameters for
float tau1[1501] = {0.0};                                      // HCN currents: Vm from -100 mV to +50 mV in
                                                              // steps of 0.1 mV
// Generate the lookup tables
void GenerateHcnLUT() {
  float v;
  for (int x=0; x<1501; x++) {
    v = (float)x/10 - 100;
    sinf1[x] = 1/(1 + expf((v+82.0)/8)); 
    tau1[x] = 10*expf(.033*(v+75))/(0.11*(1+expf(0.083*(v+75)))); 
  }
}

// At every time step, calculate the HCN current in the Hodgkin-Huxley manner
float HCN_CH1(float v) {
  static float sVar_CH1 = sinf1[250];                                    // activation gate
  float v10 = v*10.0;
  int vIdx = (int)v10 + 1000;
  vIdx = constrain(vIdx,0,1500);
  sVar_CH1 = sVar_CH1 + dt * ( -(sVar_CH1-sinf1[vIdx])/tau1[vIdx] );      // forward Euler method
  if (sVar_CH1<0.0) sVar_CH1=0.0;                                     // non-negative only
  float current = gH_CH1 * sVar_CH1 * (v + 30);                      // injected current (pA) 
  return current;
}

float HCN_CH2(float v) {
  static float sVar_CH2 = sinf1[250];                                    // activation gate
  float v10 = v*10.0;
  int vIdx = (int)v10 + 1000;
  vIdx = constrain(vIdx,0,1500);
  sVar_CH2 = sVar_CH2 + dt * ( -(sVar_CH2-sinf1[vIdx])/tau1[vIdx] );      // forward Euler method
  if (sVar_CH2<0.0) sVar_CH2=0.0;                                     // non-negative only
  float current = gH_CH2 * sVar_CH2 * (v + 30);                      // injected current (pA) 
  return current;
}
