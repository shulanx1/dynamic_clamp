// Inverse of persistent Na+ conductance using the Hodgkin-Huxley formalism.
// For speed, the parameters m_inf is pre-calculated and put in lookup tables stored as global variables.
// created by Shulan 11/03/2021

// Declare the lookup table variables
float m_inap_inf[1501] = {0.0};                          

// Generate the lookup tables
void GenerateInapLUT() {
  float v;
  float  V_slope = -5.31;
  float V_h = -41.14;
  for (int x=0; x<1501; x++) {
    v = (float)x/10 - 100.0;                        // We use membrane potentials between -100 mV and +50 mV
    m_inap_inf[x] = 1/(1+expf((v-V_h)/V_slope));
  }
}

// At every time step, calculate the sodium current in the Hodgkin-Huxley manner
float inap(float v) {       // HH style
  static float mNapVar = m_inap_inf[250];    // activation gate
  float v10 = v*10.0;
  int vIdx = (int)v10 + 1000;
  vIdx = constrain(vIdx,0,1500);
  mNapVar = mNapVar + dt * ( m_inap_inf[vIdx]-mNapVar)/1;        //tau = 1ms
  if (mNapVar < 0.0) mNapVar = 0.0;
  float current1 = gNap * mNapVar * (v - 53.4);  // ENa = +54.3 mV
  return current1;
}

// At every time step, calculate the sodium current in the Hodgkin-Huxley manner
//float inap(float v) {                                          // time invarient
//  float current1 = gNap /(1+expf(-(v+45.02)/5.01))* (v - 50);  // g = gmax*(1/(1+exp(-(V-Ehalf))/K)) Ehalf = -45.02, K = 5.01
//  return current1;
//}
