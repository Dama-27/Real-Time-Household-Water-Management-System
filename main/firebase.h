#ifndef FIREBASE_H
#define FIREBASE_H

#include "cJSON.h"
#include <stdbool.h>  // Include to use bool type

// Declare the function to send data to Firebase
// Make sure the declaration matches the definition
void firebase_send_data(float inflow, 
    float outflow, int tds, float temperature, int total_in, int total_out, 
    int water_level, const char *water_quality);

bool firebase_get_button_state();

#endif // FIREBASE_H
