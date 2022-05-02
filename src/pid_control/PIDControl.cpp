#include "avr/io.h"
#include "avr/interrupt.h"
#include "PIDControl.h"


float PIDControl::iterate(unsigned int delta_t, float new_value) {

    float p_component;
    float i_component;
    float d_component;

    // Calculate proportional component
    
    if (this->p_coefficient == 0)
        p_component = 0;
    else
        p_component = (this->target - new_value) * this->p_coefficient;

    // Calculate integral component

    if (this->i_coefficient == 0) {
        i_component = 0;
    }else{

        if (this->last_sample_value == -1) {
            // No previous sample to integrate from.
            i_component = 0;
        }else{
            
            // Perform linear interporlation for error integration.

            float rectangle_area = delta_t * (this->target - last_sample_value);
            float  triangle_area = delta_t * (last_sample_value - new_value) / 2;
            
            this->integral_value += rectangle_area + triangle_area;

            i_component = this->integral_value;
        }
    }

    // Calculate derivative component

    if (this->d_coefficient == 0) {
        d_component = 0;
    }else{
        d_component = (new_value - last_sample_value) / delta_t;
    }

    // Update last value
    this->last_sample_value = new_value;

    // Return result
    return this->p_coefficient * p_component +
           this->i_coefficient * i_component +
           this->d_coefficient * d_component;
}