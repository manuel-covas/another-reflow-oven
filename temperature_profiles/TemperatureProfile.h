#ifndef TEMPERATURE_PROFILE_H
#define TEMPERATURE_PROFILE_H

class TemperatureProfile {
    public:
        virtual float computeTemperature(unsigned long time_ms) = 0;
};

#endif