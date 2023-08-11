port = serial('COM9','BaudRate',115200);
fopen(port);

% Specify the conductance values (in nS) and diffusion constant values (in
% nS^2/ms).
g_shunt_CH1 = 0.0;
m_OU_exc_CH1 = 0.0;
D_OU_exc_CH1 = 0.0; 
m_OU_inh_CH1 = 0.0;
D_OU_inh_CH1 = 0.0;
m_OU_exc_NMDA_CH1 = 0.0;
D_OU_exc_NMDA_CH1 = 0.0;
g_EPSC_CH1 = 0.0; 
f_poisson_CH1 = 0.0;
g_nap_CH1 = 0.0;
g_nap_E2_CH1 = 0.0;                 
g_cal_CH1 = 0.0;                 
g_cah_CH1 = 0.0;                 
g_HCN_CH1 = 0.0;                   

g_shunt_CH2 = 0.0;
m_OU_exc_CH2 = 0.0;
D_OU_exc_CH2 = 0.0; 
m_OU_inh_CH2 = 0.0;
D_OU_inh_CH2 = 0.0;
m_OU_exc_NMDA_CH2 = 0.0;
D_OU_exc_NMDA_CH2 = 0.0;
g_EPSC_CH2 = 0.0; 
f_poisson_CH2 = 0.0;
g_nap_CH2 = 0.0;
g_nap_E2_CH2 = 0.0;                 
g_cal_CH2 = 0.0;                 
g_cah_CH2 = 0.0;                 
g_HCN_CH2 = 0.0; 

out = [g_shunt_CH1; m_OU_exc_CH1; D_OU_exc_CH1; m_OU_inh_CH1; D_OU_inh_CH1; m_OU_exc_NMDA_CH1; D_OU_exc_NMDA_CH1; g_EPSC_CH1; f_poisson_CH1; g_nap_CH1; g_nap_E2_CH1; g_cal_CH1; g_cah_CH1; g_HCN_CH1;
    g_shunt_CH2; m_OU_exc_CH2; D_OU_exc_CH2; m_OU_inh_CH2; D_OU_inh_CH2; m_OU_exc_NMDA_CH2; D_OU_exc_NMDA_CH2; g_EPSC_CH2; f_poisson_CH2; g_nap_CH2; g_nap_E2_CH2; g_cal_CH2; g_cah_CH2; g_HCN_CH2; 0];
upload_to_teensy(out, port); 
fclose(port);
 