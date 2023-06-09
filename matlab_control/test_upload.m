port = serial('COM9','BaudRate',115200);
fopen(port);

% Specify the conductance values (in nS) and diffusion constant values (in
% nS^2/ms).
g_shunt = 0.0;
m_OU_exc = 0.0;
D_OU_exc = 0.0; 
m_OU_inh = 0.0;
D_OU_inh = 0.0;
m_OU_exc_NMDA = 0.0;
D_OU_exc_NMDA = 0.0;
g_EPSC = 0.0; 
f_poisson = 0.0;
g_nap = 0.0;
g_nap_E2 = -10.0;                 
g_cal = 0.0;                 
g_cah = 0.0;                 
g_HCN = 0.0;                   

out = [g_shunt; m_OU_exc; D_OU_exc; m_OU_inh; D_OU_inh; m_OU_exc_NMDA; D_OU_exc_NMDA; g_EPSC; f_poisson; g_nap; g_nap_E2; g_cal; g_cah; g_HCN; 0];
upload_to_teensy(out, port); 
fclose(port);
 