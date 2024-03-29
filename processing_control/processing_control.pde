// This Processing sketch opens a GUI in which users can specify the
// conductance parameters used by the Teensy microcontroller. There are eight of 
// them at present: shunt conductance (g_shunt, nS), maximum HCN conductance (g_hcn, nS),
// maximum sodium conductance (g_Na, nS), mean excitatory Ornstein-Uhlenbeck
// conductance (m_OU_exc, nS), diffusion constant of excitatory Ornstein-Uhlenbeck
// conductance (D_OU_exc, nS^2/ms), mean inhibitory Ornstein-Uhlenbeck conductance
// (m_OU_inh, nS), diffusion constant of inhibitory Ornstein-Uhlenbeck conductance
// (D_OU_inh, nS^2/ms), and maximum EPSC conductance (g_epsc, nS).
//
// The numbers can be adjusted using the sliders.
//
// Pressing "upload" will send the numbers in the GUI to the microcontroller.
// Pressing "zero" will set all the numbers to zero and send zeros to the microcontroller.
//
// The sketch requires the ControlP5 library.
//
// Last updated 04/20/17 at 10:37 pm.


// import libraries
import controlP5.*;
import processing.serial.*;
import java.text.*;
import java.io.*;
import java.nio.*;

// define variables for the ControlP5 object (the GUI) and 
// a serial object (the port to communicate with the microcontroller)
ControlP5 dcControl;
Serial myPort;

// initialize the variables set by the GUI
float g_shunt_CH1 = 0;
float m_exc_CH1 = 0;
float D_exc_CH1 = 0;
float m_inh_CH1 = 0;
float D_inh_CH1 = 0;
float m_exc_NMDA_CH1 = 0;
float D_exc_NMDA_CH1 = 0;
float g_epsc_CH1 = 0;
float f_poisson_CH1 = 0;
float g_Nap_CH1 = 0;
float g_Nap_E2_CH1 = 0;
float g_Cal_CH1 = 0;
float g_Cah_CH1 = 0;
float g_hcn_CH1 = 0;

float g_shunt_CH2 = 0;
float m_exc_CH2 = 0;
float D_exc_CH2 = 0;
float m_inh_CH2 = 0;
float D_inh_CH2 = 0;
float m_exc_NMDA_CH2 = 0;
float D_exc_NMDA_CH2 = 0;
float g_epsc_CH2 = 0;
float f_poisson_CH2 = 0;
float g_Nap_CH2 = 0;
float g_Nap_E2_CH2 = 0;
float g_Cal_CH2 = 0;
float g_Cah_CH2 = 0;
float g_hcn_CH2 = 0;
void setup() {
  
    // specify GUI window size, color, and text case
    size(900,850);
    background(150);
    Label.setUpperCaseDefault(false);
    
    // create the ControlP5 object, add sliders, specify the font, and add buttons
    dcControl = new ControlP5(this);
    dcControl.addSlider("g_shunt_CH1", 0, 10, 0, 100, 50, 200, 30);
    dcControl.addSlider("m_exc_CH1", 0, 10, 0, 100, 100, 200, 30);
    dcControl.addSlider("D_exc_CH1", 0, 5, 0, 100, 150, 200, 30);
    dcControl.addSlider("m_inh_CH1", 0, 10, 0, 100, 200, 200, 30);
    dcControl.addSlider("D_inh_CH1", 0, 5, 0, 100, 250, 200, 30);
    dcControl.addSlider("m_exc_NMDA_CH1", 0, 10, 0, 100, 300, 200, 30);
    dcControl.addSlider("D_exc_NMDA_CH1", 0, 5, 0, 100, 350, 200, 30);
    dcControl.addSlider("g_epsc_CH1", 0, 50, 0, 100, 400, 200, 30);
    dcControl.addSlider("f_poisson_CH1", 0, 10, 0, 100, 450, 200, 30);
    dcControl.addSlider("g_Nap_CH1", -10, 10, 0, 100, 500, 200, 30);
    dcControl.addSlider("g_Nap_E2_CH1", -10, 10, 0, 100, 550, 200, 30);
    dcControl.addSlider("g_Cal_CH1", -10, 10, 0, 100, 600, 200, 30);
    dcControl.addSlider("g_Cah_CH1", -10, 10, 0, 100, 650, 200, 30);
    dcControl.addSlider("g_hcn_CH1", -10, 10, 0, 100, 700, 200, 30);
    
    dcControl.addSlider("g_shunt_CH2", 0, 10, 0, 550, 50, 200, 30);
    dcControl.addSlider("m_exc_CH2", 0, 10, 0, 550, 100, 200, 30);
    dcControl.addSlider("D_exc_CH2", 0, 5, 0, 550, 150, 200, 30);
    dcControl.addSlider("m_inh_CH2", 0, 10, 0, 550, 200, 200, 30);
    dcControl.addSlider("D_inh_CH2", 0, 5, 0, 550, 250, 200, 30);
    dcControl.addSlider("m_exc_NMDA_CH2", 0, 10, 0, 550, 300, 200, 30);
    dcControl.addSlider("D_exc_NMDA_CH2", 0, 5, 0, 550, 350, 200, 30);
    dcControl.addSlider("g_epsc_CH2", 0, 50, 0, 550, 400, 200, 30);
    dcControl.addSlider("f_poisson_CH2", 0, 10, 0, 550, 450, 200, 30);
    dcControl.addSlider("g_Nap_CH2", -10, 10, 0, 550, 500, 200, 30);
    dcControl.addSlider("g_Nap_E2_CH2", -10, 10, 0, 550, 550, 200, 30);
    dcControl.addSlider("g_Cal_CH2", -10, 10, 0, 550, 600, 200, 30);
    dcControl.addSlider("g_Cah_CH2", -10, 10, 0, 550, 650, 200, 30);
    dcControl.addSlider("g_hcn_CH2", -10, 10, 0, 550, 700, 200, 30);
    
    PFont pfont = createFont("Arial", 8, true);
    ControlFont font = new ControlFont(pfont, 18);
    dcControl.setFont(font);
    dcControl.addBang("upload").setPosition(350,750).setSize(60,50).setColorForeground(color(100,100,100));
    dcControl.addBang("zero").setPosition(475,750).setSize(60,50).setColorForeground(color(100,100,100));
    
    // create the serial port used to communicate with the microcontroller
    myPort = new Serial(this,"COM9",115200);
    myPort.clear();
    
}



void draw(){
  // nothing to see here: the Processing language requires every sketch to contain a draw() function
}


// Upload the numbers in the GUI to the microcontroller.
void upload(){
      writetoteensy(g_shunt_CH1);
      writetoteensy(m_exc_CH1);
      writetoteensy(D_exc_CH1);
      writetoteensy(m_inh_CH1);
      writetoteensy(D_inh_CH1);
      writetoteensy(m_exc_NMDA_CH1);
      writetoteensy(D_exc_NMDA_CH1);
      writetoteensy(g_epsc_CH1);
      writetoteensy(f_poisson_CH1);
      writetoteensy(g_Nap_CH1);
      writetoteensy(g_Nap_E2_CH1);
      writetoteensy(g_Cal_CH1);
      writetoteensy(g_Cah_CH1);
      writetoteensy(g_hcn_CH1);
      
      writetoteensy(g_shunt_CH2);
      writetoteensy(m_exc_CH2);
      writetoteensy(D_exc_CH2);
      writetoteensy(m_inh_CH2);
      writetoteensy(D_inh_CH2);
      writetoteensy(m_exc_NMDA_CH2);
      writetoteensy(D_exc_NMDA_CH2);
      writetoteensy(g_epsc_CH2);
      writetoteensy(f_poisson_CH2);
      writetoteensy(g_Nap_CH2);
      writetoteensy(g_Nap_E2_CH2);
      writetoteensy(g_Cal_CH2);
      writetoteensy(g_Cah_CH2);
      writetoteensy(g_hcn_CH2);
      writetoteensy(0);
}



// Zero all the numbers in the GUI and transmit zeros to the microcontroller.
void zero(){
    dcControl.getController("g_shunt_CH1").setValue(0.0);
    dcControl.getController("m_exc_CH1").setValue(0.0);
    dcControl.getController("D_exc_CH1").setValue(0.0);
    dcControl.getController("m_inh_CH1").setValue(0.0);
    dcControl.getController("D_inh_CH1").setValue(0.0);
    dcControl.getController("m_exc_NMDA_CH1").setValue(0.0);
    dcControl.getController("D_exc_NMDA_CH1").setValue(0.0);
    dcControl.getController("g_epsc_CH1").setValue(0.0);
    dcControl.getController("f_poisson_CH1").setValue(0.0);
    dcControl.getController("g_Nap_CH1").setValue(0.0);
    dcControl.getController("g_Nap_E2_CH1").setValue(0.0);
    dcControl.getController("g_Cal_CH1").setValue(0.0);
    dcControl.getController("g_Cah_CH1").setValue(0.0);
    dcControl.getController("g_hcn_CH1").setValue(0.0);
    
    dcControl.getController("g_shunt_CH2").setValue(0.0);
    dcControl.getController("m_exc_CH2").setValue(0.0);
    dcControl.getController("D_exc_CH2").setValue(0.0);
    dcControl.getController("m_inh_CH2").setValue(0.0);
    dcControl.getController("D_inh_CH2").setValue(0.0);
    dcControl.getController("m_exc_NMDA_CH2").setValue(0.0);
    dcControl.getController("D_exc_NMDA_CH2").setValue(0.0);
    dcControl.getController("g_epsc_CH2").setValue(0.0);
    dcControl.getController("f_poisson_CH2").setValue(0.0);
    dcControl.getController("g_Nap_CH2").setValue(0.0);
    dcControl.getController("g_Nap_E2_CH2").setValue(0.0);
    dcControl.getController("g_Cal_CH2").setValue(0.0);
    dcControl.getController("g_Cah_CH2").setValue(0.0);
    dcControl.getController("g_hcn_CH2").setValue(0.0);
    upload();
}



// The numbers from the GUI (floats) are converted to unsigned bytes
// and written to the Teensy.
void writetoteensy(float foo) {    
    byte[] b;
    ByteBuffer byteBuffer = ByteBuffer.allocate(4);
    byteBuffer.order(ByteOrder.LITTLE_ENDIAN);
    b = byteBuffer.putFloat(foo).array();
    for (int y=0; y<4; y++) {    // This is probably not necessary
      int boo = (b[y]&0xFF);     // since negative bytes get converted
      myPort.write(boo);         // when written
    }
}


// dispose() is invoked when the applet window closes.
// It just cleans everything up.
void dispose() {
    myPort.clear();
    myPort.stop();
    println("Stopping ...");
}  
