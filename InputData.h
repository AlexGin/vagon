#pragma once

struct InputData
{
    double db_Len = 0.0; // Length of road (km)
    int n_N = 0;         // Stop points (total quantity)
    int n_T = 0;         // Duration of waiting at point (sec) 
    double db_A = 0.0;   // Acceleration (m/sec^2)
    double db_V = 0.0;   // Velocity (km/h)
};