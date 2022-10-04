#ifndef PID_CONTROL_H
#define PID_CONTROL_H

class PIDControl {
    
    private:
        float p_coefficient, i_coefficient, d_coefficient;
        float target;

        float integral_value;
        float last_sample_value;

        float p_component, i_component, d_component;

    public:

        /**
         * @brief Construct a new PIDControl object
         * 
         * @param p_coefficient the coefficient for the proportional component
         * @param i_coefficient the coefficient for the integral component
         * @param d_coefficient the coefficient for the derivative component
         */
        PIDControl(float p_coefficient, float i_coefficient, float d_coefficient)  {
            this->p_coefficient = p_coefficient;
            this->i_coefficient = i_coefficient;
            this->d_coefficient = d_coefficient;
            this->target = 0;

            this->integral_value = 0;
            this->last_sample_value = -1;
        };
        ~PIDControl() {};

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
         * @param delta_t how much time has passed since the last aquisition in any unit.
         * @param current_value the newly aquired value.
         * @return the new target power.
         */
        float iterate(unsigned int delta_t, float current_value);
        

        /**
         * @brief Returns the current value of the proportional component.
         * 
         * @return value of the proportional component.
         */
        float getPcomponent();
        
        /**
         * @brief Returns the current value of the integral component.
         * 
         * @return value of the integral component.
         */
        float getIcomponent();

        /**
         * @brief Returns the current value of the derivative component.
         * 
         * @return value of the derivative component.
         */
        float getDcomponent();
};

#endif