// Inverse of low threshold Ca conductance using the Hodgkin-Huxley formalism.
// For speed, the parameters m_inf is pre-calculated and put in lookup tables stored as global variables.
// created by Shulan 06/09/2023

// Declare the lookup table variables
float m_cal_inf[1501] = {0.0};
float m_cal_tau[1501] = {0.0};
float h_cal_inf[1501] = {0.0};
float h_cal_tau[1501] = {0.0};

// Generate the lookup tables
void GenerateCalLUT() {
  float v;
  for (int x=0; x<1501; x++) {
    v = (float)x/10 - 100.0;                        // We use membrane potentials between -100 mV and +50 mV
    m_cal_inf[x] = 1/(1 + expf((v + 30)/(-6)));
    m_cal_tau[x] = (5.0000 + 20.0000/(1+expf((v + 25.000)/5)))/2.95;
    h_cal_inf[x] = 1/(1 + expf((v + 80)/6.4));
    h_cal_tau[x] = (20.0 + 50.0/(1+expf((v + 40.000)/7)))/2.95; 
  }
}

// At every time step, calculate the sodium current in the Hodgkin-Huxley manner
float cal(float v) {       // HH style
  static float mCalVar = m_cal_inf[250];    // activation gate, initialized as v = -75
  static float hCalVar = h_cal_inf[250];    // activation gate, initialized as v = -75
  float v10 = v*10.0;
  int vIdx = (int)v10 + 1000;
  vIdx = constrain(vIdx,0,1500);
  mCalVar = mCalVar + dt * ( m_cal_inf[vIdx]-mCalVar)/m_cal_tau[vIdx];
  hCalVar = hCalVar + dt * ( h_cal_inf[vIdx]-hCalVar)/h_cal_tau[vIdx];        
  if (hCalVar < 0.0) hCalVar = 0.0;
  if (mCalVar < 0.0) mCalVar = 0.0;
  float current1 = gCal * mCalVar * mCalVar * hCalVar * (v - 120.0);  // ECa = +120 mV
  return current1;
}
