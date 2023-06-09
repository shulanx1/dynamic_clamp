// Inverse of high threshold Ca conductance using the Hodgkin-Huxley formalism.
// For speed, the parameters m_inf is pre-calculated and put in lookup tables stored as global variables.
// created by Shulan 06/09/2023

// Declare the lookup table variables
float m_nap_alpha[1501] = {0.0};
float m_nap_beta[1501] = {0.0};
float h_nap_inf[1501] = {0.0};
float h_nap_tau[1501] = {0.0};

// Generate the lookup tables
void GenerateNapLUT() {
  float v;
  for (int x=0; x<1501; x++) {
    v = (float)x/10 - 100.0;                        // We use membrane potentials between -100 mV and +50 mV
    if (v == -38) v = v + 0.0001; 
    if (v == -17) v = v + 0.0001;   
    m_nap_alpha[x] = (0.182 * (v + 38.0))/(1-(expf(-(v + 38.0)/6.0)));
    m_nap_beta[x] = (-0.124 * (v + 38.0))/(1-(expf((v + 38.0)/6.0)));
    h_nap_inf[x] = 0.000457*expf(-(v + 13)/50);
    h_nap_tau[x] =  1/( (-2.88e-6 * (v + 17)/(1-expf((v+17)/4.63))) + (6.94e-6 * (v+ 64.4)/(1 - expf(-(v + 64.4)/2.63))) )/2.95;
  }
}

// At every time step, calculate the sodium current in the Hodgkin-Huxley manner
float nap_E2(float v) {       // HH style
  static float mNap_E2Var = m_nap_alpha[250]/(m_nap_alpha[250] + m_nap_beta[250]);    // activation gate, initiated as v = -75
  static float hNap_E2Var = h_nap_inf[250];    // inactivation gate, initiated as v = -75
  float v10 = v*10.0;
  int vIdx = (int)v10 + 1000;
  vIdx = constrain(vIdx,0,1500);
  mNap_E2Var = mNap_E2Var + dt * (m_nap_alpha[vIdx]*(1-mNap_E2Var) - m_nap_beta[vIdx]*mNap_E2Var );
  if (mNap_E2Var < 0.0) mNap_E2Var = 0.0;
  hNap_E2Var = hNap_E2Var + dt * (h_nap_inf[vIdx]-hNap_E2Var)/h_nap_tau[vIdx];
  if (hNap_E2Var < 0.0) hNap_E2Var = 0.0;
  float current1 = gNap_E2 * mNap_E2Var * mNap_E2Var * mNap_E2Var  * hNap_E2Var * (v - 53.4);  // ENa
  //Serial.println( hNap_E2Var);
  return current1;
}
