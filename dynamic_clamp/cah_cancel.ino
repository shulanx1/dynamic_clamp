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
float cah_CH1(float v) {       // HH style
  static float mCahVar_CH1 = m_cah_alpha[250]/(m_cah_alpha[250] + m_cah_beta[250]);    // activation gate, initiated as v = -75
  static float hCahVar_CH1 = h_cah_alpha[250]/(h_cah_alpha[250] + h_cah_beta[250]);    // inactivation gate, initiated as v = -75
  float v10 = v*10.0;
  int vIdx = (int)v10 + 1000;
  vIdx = constrain(vIdx,0,1500);
  mCahVar_CH1 = mCahVar_CH1 + dt * (m_cah_alpha[vIdx]*(1-mCahVar_CH1) - m_cah_beta[vIdx]*mCahVar_CH1 );
  if (mCahVar_CH1 < 0.0) mCahVar_CH1 = 0.0;
  hCahVar_CH1 = hCahVar_CH1 + dt * (h_cah_alpha[vIdx]*(1-hCahVar_CH1) - h_cah_beta[vIdx]*hCahVar_CH1 );
  if (hCahVar_CH1 < 0.0) hCahVar_CH1 = 0.0;
  float current1 = gCah_CH1 * mCahVar_CH1 * mCahVar_CH1  * hCahVar_CH1 * (v - 120);  // ECa = +120 mV
  return current1;
}

float cah_CH2(float v) {       // HH style
  static float mCahVar_CH2 = m_cah_alpha[250]/(m_cah_alpha[250] + m_cah_beta[250]);    // activation gate, initiated as v = -75
  static float hCahVar_CH2 = h_cah_alpha[250]/(h_cah_alpha[250] + h_cah_beta[250]);    // inactivation gate, initiated as v = -75
  float v10 = v*10.0;
  int vIdx = (int)v10 + 1000;
  vIdx = constrain(vIdx,0,1500);
  mCahVar_CH2 = mCahVar_CH2 + dt * (m_cah_alpha[vIdx]*(1-mCahVar_CH2) - m_cah_beta[vIdx]*mCahVar_CH2 );
  if (mCahVar_CH2 < 0.0) mCahVar_CH2 = 0.0;
  hCahVar_CH2 = hCahVar_CH2 + dt * (h_cah_alpha[vIdx]*(1-hCahVar_CH2) - h_cah_beta[vIdx]*hCahVar_CH2 );
  if (hCahVar_CH2 < 0.0) hCahVar_CH2 = 0.0;
  float current1 = gCah_CH2 * mCahVar_CH2 * mCahVar_CH2  * hCahVar_CH2 * (v - 120);  // ECa = +120 mV
  return current1;
}
