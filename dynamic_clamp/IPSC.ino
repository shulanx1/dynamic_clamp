// Each IPSC is triggered by a LOW --> HIGH transition at epscTriggerPin.
// The two-stage kinetic scheme is similar to how NMDA currents are handled 
// in Walcott, Higgins, and Desai, J. Neurosci. (2011).

// Notes: ?? - Is each IPSC trigered by a HIGH --> LOW transition at ipscTriggerPin?

volatile float xIPSC = 0.0;       // a pure number, EPSC intermediate gating variable

 // Increment xIPSC by 1 every time a trigger arrives at ipscTriggerPin
 void UpdateIpscTrain()   {
  xIPSC += 1;
 }

 // Calculate the net IPSC current at every time step
 float IPSC(float v)  {
  const float tauX = 2.0;                      // msec, rise time
  const float tauS = 8.0;                     // msec, decay time
  const float alphaS = 1.0;                    // number/msec, saturation level 
  static float s = 0.0;
  xIPSC = xIPSC + dt * (-xIPSC/tauX);
  s = s + dt * (-s/tauS + alphaS*xIPSC*(1-s)); // forward Euler method
  float current = -gIPSC * s * (v+80);          // reversal potential -80 mV
  return current;
 }
