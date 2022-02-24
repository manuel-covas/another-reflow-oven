#include "GenericPasteProfile.h"

const unsigned long PROFILE_START_MS = 0;
const unsigned long PROFILE_END_MS   = 360 * 1000;

const float         SOAK_STAGE_CELCIUS  = 150;
const unsigned long SOAK_STAGE_START_MS = 90 * 1000;
const unsigned long SOAK_STAGE_END_MS   = 180 * 1000;

const float         REFLOW_STAGE_CELCIUS  = 248;
const unsigned long REFLOW_STAGE_START_MS = 240 * 1000;
const unsigned long REFLOW_STAGE_END_MS   = 270 * 1000;


const char* GenericPasteProfile::getName() { return "Generic Solder Paste"; }

float GenericPasteProfile::computeTemperature(unsigned long time_ms) {

    // Pre-start
    if (time_ms < PROFILE_START_MS)
        return 0;

    // Linear slope to soak stage
    if (time_ms < SOAK_STAGE_START_MS)
        return SOAK_STAGE_CELCIUS * (time_ms - PROFILE_START_MS) / (SOAK_STAGE_START_MS - PROFILE_START_MS);

    // Soak stage
    if (time_ms < SOAK_STAGE_END_MS)
        return SOAK_STAGE_CELCIUS;

    // Linear slope to reflow stage
    if (time_ms < REFLOW_STAGE_START_MS)
        return SOAK_STAGE_CELCIUS + (REFLOW_STAGE_CELCIUS - SOAK_STAGE_CELCIUS) * (time_ms - SOAK_STAGE_END_MS) / (REFLOW_STAGE_START_MS - SOAK_STAGE_END_MS);

    // Reflow stage
    if (time_ms < REFLOW_STAGE_END_MS)
        return REFLOW_STAGE_CELCIUS;

    // Linear slope to zero
    if (time_ms < PROFILE_END_MS)
        return REFLOW_STAGE_CELCIUS * (1 - ((float) (time_ms - REFLOW_STAGE_END_MS) / (PROFILE_END_MS - REFLOW_STAGE_END_MS)));

    // Profile ended
    return 0;
}