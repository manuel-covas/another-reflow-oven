#include "avr/io.h"
#include "avr/interrupt.h"
#include "PIDControl.h"


float PIDControl::getPvalue() { return p_component * p_coefficient; }
float PIDControl::getIvalue() { return i_component * i_coefficient; }
float PIDControl::getDvalue() { return d_component * d_coefficient; }


float PIDControl::iterate(unsigned int time_ms, float new_sample) {

    unsigned int oldest_sample_index =
        (last_sample_index == (d_rolling_average_width - 1) ?
            0 : last_sample_index + 1);

    unsigned int delta_t = time_ms - time_buffer[last_sample_index];
    unsigned int delta_t_oldest = time_ms - time_buffer[oldest_sample_index];

    float last_sample = sample_buffer[last_sample_index];
    float oldest_sample = sample_buffer[oldest_sample_index];

    // Calculate proportional component
    
    if (p_coefficient == 0)
        p_component = 0;
    else
        p_component = (target - new_sample) * p_coefficient;

    // Calculate integral component

    if (i_coefficient != 0) {

        if (!sample_buffer_filled) {
            i_component = 0;
        }else{
            
            // Perform linear interporlation for error integration.

            float rectangle_area = delta_t * (target - last_sample);
            float  triangle_area = delta_t * (last_sample - new_sample) / (float) 2;
            
            integral_value += rectangle_area + triangle_area;

            i_component = integral_value;
        }
        
    }else{
        i_component = 0;
    }

    // Calculate derivative component

    if (d_coefficient == 0 | !sample_buffer_filled | delta_t_oldest < 1) {
        d_component = 0;
    }else{
        d_component = (new_sample - oldest_sample) / ((float) delta_t_oldest / 1000);
    }

    // Update buffers
    
    if (oldest_sample_index < last_sample_index)
        if (!sample_buffer_filled)
            sample_buffer_filled = true;  // First loop around occurred, set flag
    
    time_buffer[oldest_sample_index] = time_ms;
    sample_buffer[oldest_sample_index] = new_sample;

    last_sample_index = oldest_sample_index;

    // Return result
    return p_coefficient * p_component +
           i_coefficient * i_component +
           d_coefficient * d_component;
}