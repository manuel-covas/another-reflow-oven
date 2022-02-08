#ifndef GENERIC_PASTE_PROFILE_H
#define GENERIC_PASTE_PROFILE_H

#include "TemperatureProfile.h"

class GenericPasteProfile : public TemperatureProfile {
    public:
        /**
         * @brief Returns the target temperature in celcius for the given time.
         * @param time_ns The current time in milliseconds with respect to this profile's start time.
         **/
        float computeTemperature(unsigned long time_ms);
};

#endif