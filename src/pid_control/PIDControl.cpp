#include "avr/io.h"
#include "avr/interrupt.h"
#include "PIDControl.h"


float PIDControl::getPcomponent() { return this->p_component; }
float PIDControl::getIcomponent() { return this->i_component; }
float PIDControl::getDcomponent() { return this->d_component; }


float PIDControl::iterate(unsigned int delta_t, float new_value) {

    // Calculate proportional component
    
    if (this->p_coefficient == 0)
        this->p_component = 0;
    else
        this->p_component = (this->target - new_value) * this->p_coefficient;

    // Calculate integral component

    if (this->i_coefficient == 0) {
        this->i_component = 0;
    }else{

        if (this->last_sample_value == -1) {
            // No previous sample to integrate from.
            this->i_component = 0;
        }else{
            
            // Perform linear interporlation for error integration.

            float rectangle_area = delta_t * (this->target - last_sample_value);
            float  triangle_area = delta_t * (last_sample_value - new_value) / 2;
            
            this->integral_value += rectangle_area + triangle_area;

            this->i_component = this->integral_value;
        }
    }

    // Calculate derivative component

    if (this->d_coefficient == 0) {
        this->d_component = 0;
    }else{
        this->d_component = (new_value - last_sample_value) / delta_t;
    }

    // Update last value
    this->last_sample_value = new_value;

    // Return result
    return this->p_coefficient * this->p_component +
           this->i_coefficient * this->i_component +
           this->d_coefficient * this->d_component;
}