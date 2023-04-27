% Open the USB port. In this example, we assume that the Teensy
% microcontroller is connected to COM3.
port = serial('COM9','BaudRate',115200);
fopen(port);

% Specify the conductance values (in nS) and diffusion constant values (in
% nS^2/ms).
g_shunt = 0.0;
g_HCN = 0.0;
g_Na = 0.0;
m_OU_exc = 0.0;
D_OU_exc = 0.0; 
m_OU_inh = 0.0;
D_OU_inh = 0.0;
m_OU_exc_NMDA = 0.0;
D_OU_exc_NMDA = 0.0;
g_EPSC = 0.0; 
f_poisson = 0.0;
f_uncaging = 0.0;
g_nap = 0.0;

out = [g_shunt; g_HCN; g_Na; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; m_OU_exc_NMDA; D_OU_exc_NMDA; g_EPSC; f_poisson; f_uncaging; g_nap; 1];
upload_to_teensy(out, port); 
java.lang.Thread.sleep(2);
out = [g_shunt; g_HCN; g_Na; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; m_OU_exc_NMDA; D_OU_exc_NMDA; g_EPSC; f_poisson; f_uncaging; g_nap; 0];
upload_to_teensy(out, port); 

pause(17);

%%--------------------------------------------------------------------------

m_OU_exc = 2.0;
D_OU_exc = 1.0; 
m_OU_inh = 4.0;
D_OU_inh = 1.0;
m_OU_exc_NMDA = 0.0;
D_OU_exc_NMDA = 0.0;


out = [g_shunt; g_HCN; g_Na; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; m_OU_exc_NMDA; D_OU_exc_NMDA; g_EPSC; f_poisson; f_uncaging; g_nap; 1];
upload_to_teensy(out, port); 
java.lang.Thread.sleep(2);
out = [g_shunt; g_HCN; g_Na; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; m_OU_exc_NMDA; D_OU_exc_NMDA; g_EPSC; f_poisson; f_uncaging; g_nap; 0];
upload_to_teensy(out, port); 

pause(17);

%%--------------------------------------------------------------------------

m_OU_exc = 3.0;
D_OU_exc = 1.0; 
m_OU_inh = 6.0;
D_OU_inh = 1.0;
m_OU_exc_NMDA = 0.0;
D_OU_exc_NMDA = 0.0;


out = [g_shunt; g_HCN; g_Na; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; m_OU_exc_NMDA; D_OU_exc_NMDA; g_EPSC; f_poisson; f_uncaging; g_nap; 1];
upload_to_teensy(out, port); 
java.lang.Thread.sleep(2);
out = [g_shunt; g_HCN; g_Na; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; m_OU_exc_NMDA; D_OU_exc_NMDA; g_EPSC; f_poisson; f_uncaging; g_nap; 0];
upload_to_teensy(out, port); 

pause(17);

%%--------------------------------------------------------------------------

m_OU_exc = 2.0;
D_OU_exc = 1.0; 
m_OU_inh = 6.0;
D_OU_inh = 1.0;
m_OU_exc_NMDA = 0.0;
D_OU_exc_NMDA = 0.0;


out = [g_shunt; g_HCN; g_Na; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; m_OU_exc_NMDA; D_OU_exc_NMDA; g_EPSC; f_poisson; f_uncaging; g_nap; 1];
upload_to_teensy(out, port); 
java.lang.Thread.sleep(2);
out = [g_shunt; g_HCN; g_Na; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; m_OU_exc_NMDA; D_OU_exc_NMDA; g_EPSC; f_poisson; f_uncaging; g_nap; 0];
upload_to_teensy(out, port); 

pause(17);

%%--------------------------------------------------------------------------

m_OU_exc = 3.0;
D_OU_exc = 1.0; 
m_OU_inh = 9.0;
D_OU_inh = 1.0;
m_OU_exc_NMDA = 0.0;
D_OU_exc_NMDA = 0.0;


out = [g_shunt; g_HCN; g_Na; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; m_OU_exc_NMDA; D_OU_exc_NMDA; g_EPSC; f_poisson; f_uncaging; g_nap; 1];
upload_to_teensy(out, port); 
java.lang.Thread.sleep(2);
out = [g_shunt; g_HCN; g_Na; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; m_OU_exc_NMDA; D_OU_exc_NMDA; g_EPSC; f_poisson; f_uncaging; g_nap; 0];
upload_to_teensy(out, port); 

pause(17);

% Close and delete the serial port when done.
% fclose(port);
% delete(port);
%
% Specify the conductance values (in nS) and diffusion constant values (in
% nS^2/ms).
g_leak = 0;
g_HCN = 0;
g_Na = 0;
g_K = 0;
m_OU_exc = 0;
D_OU_exc = 0; 
m_OU_inh = 0;
D_OU_inh = 0;
g_EPSC = 0; 
f_poisson = 0;
g_nap = 0;

% Put them together in one variable to send out the USB port. Convert the
% numbers to single-point (32-bit) precision. (Matlab's default precision 
% is double = 64 bit.) Convert the single point numbers to byte arrays.
out = [g_leak; g_HCN; g_Na; g_K; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; g_EPSC; f_poisson; g_nap;0];
out = single(out);
out = typecast(out, 'uint8');

% Open the USB port. In this example, we assume that the Teensy
% microcontroller is connected to COM3.
% port = serial('COM9','BaudRate',115200);
% fopen(port);

% Send the numbers to the Teensy microcontroller.
fwrite(port, out);

% Close and delete the serial port when done.
fclose(port);
delete(port);