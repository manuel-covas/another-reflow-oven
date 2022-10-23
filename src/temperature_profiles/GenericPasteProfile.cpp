#include "GenericPasteProfile.h"

const unsigned long PROFILE_START_MS = 0;
const unsigned long PROFILE_END_MS   = 360000;

const float         AMBIENT_CELCIUS  = 25.0;

const float         SOAK_STAGE_CELCIUS  = 150.0;
const unsigned long SOAK_STAGE_START_MS = 90000;
const unsigned long SOAK_STAGE_END_MS   = 180000;

const float         REFLOW_STAGE_CELCIUS  = 248;
const unsigned long REFLOW_STAGE_START_MS = 240000;
const unsigned long REFLOW_STAGE_END_MS   = 320000;


const char* GenericPasteProfile::getName() { return "Generic Solder Paste"; }

float GenericPasteProfile::computeTemperature(unsigned long time_ms) {

    // Pre-start
    if (time_ms < PROFILE_START_MS)
        return AMBIENT_CELCIUS;

    // Linear slope to soak stage
    if (time_ms < SOAK_STAGE_START_MS)
        return AMBIENT_CELCIUS + (SOAK_STAGE_CELCIUS - AMBIENT_CELCIUS) * (time_ms - PROFILE_START_MS) / (SOAK_STAGE_START_MS - PROFILE_START_MS);

    // Soak stage
    if (time_ms < SOAK_STAGE_END_MS)
        return SOAK_STAGE_CELCIUS;

    // Linear slope to reflow stage
    if (time_ms < REFLOW_STAGE_START_MS)
        return SOAK_STAGE_CELCIUS + (REFLOW_STAGE_CELCIUS - SOAK_STAGE_CELCIUS) * (time_ms - SOAK_STAGE_END_MS) / (REFLOW_STAGE_START_MS - SOAK_STAGE_END_MS);

    // Reflow stage
    if (time_ms < REFLOW_STAGE_END_MS)
        return REFLOW_STAGE_CELCIUS;

    // Cooldown
    return 0;
}