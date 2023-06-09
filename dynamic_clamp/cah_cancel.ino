// Inverse of high threshold Ca conductance using the Hodgkin-Huxley formalism.
// For speed, the parameters m_inf is pre-calculated and put in lookup tables stored as global variables.
// created by Shulan 06/09/2023

// Declare the lookup table variables
float m_cah_alpha[1501] = {0.0};
float m_cah_beta[1501] = {0.0};
float h_cah_alpha[1501] = {0.0};
float h_cah_beta[1501] = {0.0};

// Generate the lookup tables
void GenerateCahLUT() {
  float v;
  for (int x=0; x<1501; x++) {
    v = (float)x/10 - 100.0;                        // We use membrane potentials between -100 mV and +50 mV
    if (v == -27) v = v + 0.0001;    
    m_cah_alpha[x] = (0.055 * (v + 27.0))/(1-(expf(-(v + 27.0)/3.8)));
    m_cah_beta[x] = 0.94 * expf(-(v + 75)/17);
    h_cah_alpha[x] = 0.000457*expf(-(v + 13)/50);
    h_cah_beta[x] =  0.0065/(1 + expf(-(v + 15)/28));
  }
}

// At every time step, calculate the sodium current in the Hodgkin-Huxley manner
float cah(float v) {       // HH style
  static float mCahVar = m_cah_alpha[250]/(m_cah_alpha[250] + m_cah_beta[250]);    // activation gate, initiated as v = -75
  static float hCahVar = h_cah_alpha[250]/(h_cah_alpha[250] + h_cah_beta[250]);    // inactivation gate, initiated as v = -75
  float v10 = v*10.0;
  int vIdx = (int)v10 + 1000;
  vIdx = constrain(vIdx,0,1500);
  mCahVar = mCahVar + dt * (m_cah_alpha[vIdx]*(1-mCahVar) - m_cah_beta[vIdx]*mCahVar );
  if (mCahVar < 0.0) mCahVar = 0.0;
  hCahVar = hCahVar + dt * (h_cah_alpha[vIdx]*(1-hCahVar) - h_cah_beta[vIdx]*hCahVar );
  if (hCahVar < 0.0) hCahVar = 0.0;
  float current1 = gCah * mCahVar * mCahVar  * hCahVar * (v - 120);  // ECa = +120 mV
  return current1;
}
