// teensy 3.6 sketch to implement dynamic clamp
//
// The sketch includes a main file (this one) and other files (which appear as tabs) that define the conductances. 
// The main file is organized like this:
//      (i)   global variables are declared first. inputScaling and outputScaling are properties of the patch
//            clamp amplifier (a user should specify these for their own amplifier). The calibration parameters
//            are properties of the components on the breadboard (and are usually set by the calibration sketch).
//            The dynamic clamp parameters will be specified by the Processing GUI or by some other program (Matlab,
//            Igor Pro, Python, etc.) the user chooses. dt and t0 are used to keep time. Hardware connections
//            are chosen by the user.
// Notes: user specifies inputScaling and outputScaling, and dynamic clamp parameters
//      (ii)  setup(). Every Arduino-like sketch includes a setup() function which is run when the sketch is 
//            uploaded to the microcontroller. Serial communication is established with the host computer (for use
//            by Processing, Matlab, Igor Pro, Python, or whatever). 12-bit resolution is specified for analog
//            read and write. Any numbers that should be pre-calculated and stored in global variables (e.g.,
//            the lookup table numbers for sodium activation/inactivation) are calculated by calling the function
//            (in one of the tabbed files) that defines them. The trigger pin for EPSCs is established.
// Notes: Shulan said to edit  code to include various EPSCs (one for AMPA and one for GABA)
//      (iii) loop(). The loop() function is run continuously (i.e., once per time step). The first part checks
//            whether the microcontroller has received any serial communication instructing it to change the
//            dynamic clamp parameters (e.g., shunt conductance or OU excitatory diffusion constant). The second 
//            part reads the signal from the patch clamp amplifier's output (which has already been buffered,
//            shifted, and scaled by the breadboard electronics), converts this signal into membrane potential v
//            in millivolts (using the calibration parameters), and sends v out to the functions (in the tabbed files)
//            that calculate the dynamic clamp current in picoamps for the various conductances. All of these currents
//            are summed. The third and last part converts the summed current (in picoamps) into the output to 
//            be sent (0-4095) from the analog output pin. This output will be shifted, scaled, and added to the 
//            DAQ board's current clamp command by breadboard electronics. The total signal will then be fed to 
//            the patch clamp amplifier's command input. 
// Noes: the whole point of this code is to take in the patch clamp amplifer's membrane voltage v and to calculate 
// the dynamic clamp current in picoamps (for the various conductances, summed), and to convert and send the summer current
// into an output (to be shifted, scaled, and added to the DAQ board's current clamp command)
//            Each dynamic clamp conductance is defined by one of the tabbed files. These files can contain multiple functions
//            and their own global variables (accessible to the functions of the tabbed file). For example, the file Sodium
//            contains declarations for the activation/inactivation lookup variables, a function that calculates the activation
//            and inactivation numbers for a range of membrane potentials and stores them in the lookup table variables,
//            and the function called at every time step to calculate the sodium current to be injected. 
// Notes: each tab contains a function for one of the condutances that is necessary to emulate a membrane voltage. Each function
// outputs a current, which is then summed in the main program to create a single current to be given to the system. 
//
// Last revised 04/24/23, 10:00 pm

#include <math.h>

// Scaling for the patch clamp amplifier
float inputScaling = 10.0;                 // number of millivolts sent out by amplifier for each millivolt of membrane potential
                                           // e.g., with a scaling of 50, if the membrane potential is -65 mV, the amplifier outputs -3.25 V
float outputScaling = 400.0;               // number of picoamps injected for every volt at the amplifier's command input
                                           // e.g., if the DAC or the Teensy outputs -0.5 V, this is interpreted as -200 pA by the amplifier
                                           // when the scaling is 400
// Notes: 
// inputScaling = millivolts amplifier / millivolts of membrane potential
// outputScaling = icoamps injected / volt at amplifier command input

// Calibrating the input/output numbers given the resistor values and power supply values of the breadboard
// N.B.: these parameters (numerators) are properties of the components on the breadboard;
// they are independent of the amplifier and DAQ board
const float inputSlope = 0.55988;//0.5599;//0.5606; //0.5634;      //5.5010/inputScaling;      //theoretical  0.5499, 2%error
const float inputIntercept = -1188.9;//-1199.216;//-1197.4547;    //-1201.7356;  //-10920.27/inputScaling;    //theoretical    -1092.2, 
const float outputSlope = 1.86535; //1.8300296;//1.8838;    //504.6777/outputScaling;       //theoretical 1.8315
const float outputIntercept = 2064.2; //2003.1484;//1995.55;    //2457.016;     //theoretical 2014.7
// Notes: calibration numbers for input and output values based on values of resistors and power supply values on the breadboard

// Conductance parameters are sent by the host computer over the USB port
const int nPars = 14;              // number of adjustable parameters
float gShunt = 0.0;               // nS, shunting conductance
float gH = 0.0;                   // nS, maximal H (HCN) conductance
float gNa = 0.0;                  // nS, maximal fast, transient sodium conductance
float OU1_mean = 0.0;             // nS, excitatory Ornstein-Uhlenbeck (OU) mean conductance
float OU1_D = 0.0;                // nS^2/msec, excitatory OU diffusion constant
float OU2_mean = 0.0;             // nS, inhibitory OU mean conductance
float OU2_D = 0.0;                // nS^2/msec, inhibitory OU diffusion constant
float OU1_2_mean = 0.0;             // nS, excitatory Ornstein-Uhlenbeck (OU) mean conductance
float OU1_2_D = 0.0;                // nS^2/msec, excitatory OU diffusion constant

float gEPSC = 0.0;                // nS, maximal conductance of EPSCs
float fPoissonE = 0.0;                // Hz, frequency of poisson train of EPSCs
float fUncaging = 0.0;                // Hz, frequency of poisson train of EPSCs

float gIPSC = 0.0;                // nS, maximal conductance of IPSCs
float fPoissonI = 0.0;                // Hz, frequency of poisson train of IPSCs

float gNap = 0.0;                 //nS, conductance of Nap to be canceled 
float trig = 0.0;                 //output trigger, to trigger the start of ephys recording
float y = 0.0;                    // random variable of bernoulli trials for poisson train
// Notes: constants to figure out what the frequency of the Poisson train, the conductance value itself, etc....
// Note: instead of having a conductance value set (like above), the conductances need to follow an exponential shape

// Common global variables
float dt = 0.01;                  // msec, the time step -- on a Teensy/Arduino dt will be variable
elapsedMicros t0;        // microsec, t0 is used to calculate dt on every iteration
// ?? - why does this not have a type? the code won't run until this has a type...
elapsedMillis epscTime;           // msec, time since last EPSC was triggered
elapsedMillis ipscTime;     // msec, time since last IPSC was triggered
elapsedMillis uncagingTime;           // msec, time since last EPSC was triggered

// Hardware connections
const int analogInPin = 0;        // ADC pin used to read membrane potential
const int analogOutPin = A21;     // DAC pin used to output current
const int epscTriggerPin = A2;     // pin used to trigger EPSC train 
const int ipscTriggerPin = A5;     // pin used to trigger EPSC train 
const int uncagingTriggerPin = A3;     // pin used to uncaging train
const int startTriggerPin = A4; // pin used to trigger start of uncaging          
// Notes: 
// ?? - What is the difference between uncaging here and EPSC? everything looks the same between these two here
// just setting pin constants for analog in and out out of Arduino board for ease of use

const int uniformPoolSize = 100;
float uniformRandomNumbers[uniformPoolSize] = {0.0};
void GenerateUniformNumbers() {
  for (int x=0; x<uniformPoolSize; x++){
    uniformRandomNumbers[x] = random(1001)/1000.0;
}
}
// Notes: ?? - generating random numbers...? what does uniform random mean?                                                   


void setup() {
  Serial.begin(115200);
  analogWriteResolution(12);
  analogReadResolution(12);
  while (Serial.available()>0) {             // make sure serial buffer is clear 
    char foo = Serial.read();
  }
  GenerateGaussianNumbers();                // generate a pool of Gaussian numbers for use by the OU processes  
  GenerateSodiumLUT();                      // generate sodium activation/inactivation lookup table
  GenerateInapLUT();                        // generate inap activation lookup table
  GenerateHcnLUT();                         // generate HCN activation lookup table
  GenerateUniformNumbers();
  pinMode(epscTriggerPin, INPUT);           // define the pin that receives a trigger when an EPSC should be injected
  pinMode(ipscTriggerPin, INPUT);           // define the pin that receives a trigger when an IPSC should be injected
  pinMode(uncagingTriggerPin, OUTPUT);
  pinMode(startTriggerPin, OUTPUT);
  
}

// Notes: ?? - am I creating a new input pin for IPSC triggering?

void loop() {

  // Part 1:    Serial Communication & Parameter Setting
  // The host computer tells the Teensy when the dynamic clamp parameters should change.
  // To do this, it sends 4-byte-long numbers, one for each parameter. So if there are 
  // 8 adjustable parameters, it will send 32 bytes. At present, the adjustable parameters
  // are the shunt conductance (nS), maximal HCN conductance (nS), maximal Na conductance (nS),
  // excitatory OU conductance (OU1) mean (nS), excitatory OU conductance diffusion constant (nS^2/ms),
  // inhibitory OU conductance (OU2) mean (nS), inhibitory OU conductance (OU2) diffusion
  // constant (nS^2/ms), and maximal EPSC conductance (nS).
// Notes: at present there are 15 adjustable parameters (32 bytes of information sent over serial communication port) - look below

  union {
    byte asByte[4];
    float asFloat;
  } data1;
  static float dataTemp[nPars] = {0.0};
  static byte parNo = 0;
  if (Serial.available()>3) {
    for (int x=0; x<4; x++) data1.asByte[x] = Serial.read();
    dataTemp[parNo] = data1.asFloat;
    parNo++;
    if (parNo==nPars) {
      gShunt = dataTemp[0];
      gH = dataTemp[1];
      gNa = dataTemp[2];
      OU1_mean = dataTemp[3];
      OU1_D = dataTemp[4];
      OU2_mean = dataTemp[5];
      OU2_D = dataTemp[6];
      OU1_2_mean = dataTemp[7];
      OU1_2_D = dataTemp[8];
      gEPSC = dataTemp[9];
      gIPSC = dataTemp[10];
      fPoissonE= dataTemp[11];
      fPoissonI= dataTemp[12];
//      fUncaging= dataTemp[13];
      gNap = dataTemp[13];
//      trig = dataTemp[15];
      parNo = 0;
    }
  }

  // Part 2:    Read membrane potential & calculate dynamic clamp currents
  float v = inputSlope * analogRead(analogInPin) + inputIntercept;      // mv, given amplifier settings (input calibration done here?)
  float injectionCurrent = 0.0;
  dt = 0.001*(float)t0;
  t0 = 0;

  if (gShunt>0) {
    injectionCurrent += Shunting(v);
  }
  if (gH>0) {
    injectionCurrent += HCN(v);
  }
  if (gNa>0) {
    injectionCurrent += Sodium(v);
  }
  if (gNap!=0) {            // note that gNap just need to be nonzero to elicit a difference in the injection current
    injectionCurrent += inap(v);
  }
  if (OU1_2_mean ==0 && (OU1_mean>0 || OU2_mean>0) ) {
    injectionCurrent += OrnsteinUhlenbeck(v);
  }
  if (OU1_2_mean >0 && (OU1_mean>0 || OU2_mean>0) ) {
    injectionCurrent += OrnsteinUhlenbeck_wNMDA(v);     // ?? - what is NMDA?
  }
  if (gEPSC>0 && fPoissonE== 0) {                                        // ?? - shouldn't the poisson train not run if fPOisson = 0?
    if ((digitalReadFast(epscTriggerPin)==HIGH)&&(epscTime>2)) {         // poll epscTriggerPin to see if a trigger has arrived
      UpdateEpscTrain();
      epscTime = 0;
    }
    injectionCurrent += EPSC(v);
  }

  if (gIPSC>0 && fPoissonI== 0) {                                        // ?? - shouldn't the poisson train not run if fPOisson = 0?
    if ((digitalReadFast(ipscTriggerPin)==HIGH)&&(ipscTime>2)) {         // poll epscTriggerPin to see if a trigger has arrived
      UpdateIpscTrain();
      ipscTime = 0;
    }
    injectionCurrent += IPSC(v);
  }
  
  if (fUncaging>0) { // send poisson uncaging pulse
    y = random(10001)/10000.0;
    //Serial.println(y);
    if ((y<dt/10000.0*fUncaging)&&(uncagingTime>1000)) {
      digitalWriteFast(uncagingTriggerPin, HIGH);
      delay(2);
      digitalWriteFast(uncagingTriggerPin, LOW);
      uncagingTime = 0;
    }
  }
  if (gEPSC>0 && fPoissonE>0){
    //y = uniformRandomNumbers[random(uniformPoolSize)];
    y = random(1001)/1000.0;
    if ((y <=dt/1000.0*fPoissonE)&&(epscTime>5)) {                       // poll epscTriggerPian to see if a a trigger has arrived
      UpdateEpscTrain();
      epscTime = 0;
    }
    injectionCurrent += EPSC(v);
  }
  if (gIPSC>0 && fPoissonI>0){
    //y = uniformRandomNumbers[random(uniformPoolSize)];
    y = random(1001)/1000.0;
    if ((y <=dt/1000.0*fPoissonI)&&(ipscTime>5)) {                       // poll epscTriggerPian to see if a a trigger has arrived
      UpdateIpscTrain();
      ipscTime = 0;
    }
    injectionCurrent += IPSC(v);
  }
  if(trig>0) {                                                // ?? - what is the function of this?
    trig_out();
    trig = 0;
  }
  

  // Part 3:      Send out the calculated dynamic clamp current
  injectionCurrent = outputSlope * injectionCurrent + outputIntercept;  //pA converted into analog output integer (0 to 4095 I believe)
  int outputSignal = constrain((int)injectionCurrent,0,4095);           // make sure the output is an integer between 0 and 4095 (12 bits)
  analogWrite(analogOutPin, outputSignal);                              // send the output to the patch clamp or summing amplifier
}