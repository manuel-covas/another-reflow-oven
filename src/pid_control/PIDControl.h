#ifndef PID_CONTROL_H
#define PID_CONTROL_H

#include <stdlib.h>

class PIDControl {
    
    private:
        float p_coefficient, i_coefficient, d_coefficient;
        unsigned int d_rolling_average_width;
        
        float target;
        float integral_value;

        unsigned int* time_buffer;
        float*        sample_buffer;

        unsigned int last_sample_index;
        bool sample_buffer_filled;

        float p_component, i_component, d_component;

    public:

        /**
         * @brief Construct a new PIDControl object
         * 
         * @param p_coefficient the coefficient for the proportional component
         * @param i_coefficient the coefficient for the integral component
         * @param d_coefficient the coefficient for the derivative component
         * @param d_rolling_average_width the width of the derivative rolling average
         */
        PIDControl(float p_coefficient, float i_coefficient, float d_coefficient, unsigned int d_rolling_average_width) {

            this->p_coefficient = p_coefficient;
            this->i_coefficient = i_coefficient;
            this->d_coefficient = d_coefficient;
            this->d_rolling_average_width = d_rolling_average_width;

            target = 0;
            integral_value = 0;

            time_buffer = (unsigned int*) calloc(d_rolling_average_width, sizeof(unsigned int));
            sample_buffer = (float*) calloc(d_rolling_average_width, sizeof(float));
            
            sample_buffer_filled = false;
            last_sample_index = 0;
        };

        ~ PIDControl() {
            free(this->sample_buffer);
        };

        /**
         * @brief Sets the target value
         * 
         * @param target the new target value 
         */
        void setTarget(float target) {
            this->target = target;
        }

        /**
         * @brief Ierate algorithm with a newly aquired value.
         * 
         * @param time_ms the current time in milliseconds.
         * @param new_sample the newly aquired sample.
         * @return the new target power.
         */
        float iterate(unsigned int time_ms, float new_sample);
        

        /**
         * @brief Returns the current value of the proportional component.
         * 
         * @return value of the proportional component.
         */
        float getPvalue();
        
        /**
         * @brief Returns the current value of the integral component.
         * 
         * @return value of the integral component.
         */
        float getIvalue();

        /**
         * @brief Returns the current value of the derivative component.
         * 
         * @return value of the derivative component.
         */
        float getDvalue();
};

#endif