#include <stdint.h>

/**
 * We will activate the TRIAC at a calculated offset from each zero-crossing in order to apply
 * the target relative power.
 *
 * By Ohm's Law we have that Power = V²R which means that power is linearly porpotional to V².
 *
 * Taking mains voltage as V = sin(t*f*2π), the energy of of one full half-cycle comes out as
 * ∫sin(t*f*2π)² dt from zero to 1/2f and corresponds to full power and is equal to 1/4f.
 *
 * The integral ∫sin(t*f*2π)² from a delayed instant t to the end of the half cycle is:
 *
 * E(t) = 1 - 2ft + sin(tf4π)/2π
 * 
 * Dividing E(t) by the max energy 1/4f yields a relative power from 0% to 100%.
 * 
 * The following pre-computed lookup table indicates the appropriate zero-crossing
 * TRAIC activation delay to achieve a target relative power level.
 * 
 * These values must be scaled by multiplying by the duration of one half-cycle.
 * Time delays are stored as float since they will be rounded to micro-seconds either way.
 * Consists of 1% steps.
 * 
 * Lookup: t = delay_lookup_table[Energy(%) - 1]
 **/
const float delay_lookup_table[] = {
    0.884004000061055,
    0.853068400077338,
    0.831031300088938,
    0.813247900098298,
    0.798040800106302,
    0.784588900113383,
    0.772421900119787,
    0.761242000125672,
    0.750847400131143,
    0.741094200136277,
    0.731875900141129,
    0.723111100145742,
    0.71473590015015,
    0.706699200154381,
    0.698959500158454,
    0.69148230016239,
    0.684238800166203,
    0.677204700169905,
    0.670358900173509,
    0.663683500177022,
    0.657162700180454,
    0.650782600183813,
    0.644531000187103,
    0.638397000190332,
    0.632371000193504,
    0.626444000196623,
    0.620608300199695,
    0.614856600202722,
    0.609182300205709,
    0.603579300208658,
    0.598042100211573,
    0.592565500214455,
    0.587144800217309,
    0.581775500220135,
    0.576453300222936,
    0.571174400225715,
    0.565935000228473,
    0.560731600231212,
    0.555560800233933,
    0.550419600236639,
    0.545304700239331,
    0.540213300242011,
    0.535142400244681,
    0.53008950024734,
    0.525051700249992,
    0.520026400252637,
    0.515011100255277,
    0.510003300257913,
    0.505000400260546,
    0.500000000263178,
    0.494999600263034,
    0.48999670026289,
    0.484988900262746,
    0.479973600262602,
    0.474948300262458,
    0.469910500262313,
    0.464857600262167,
    0.459786700262022,
    0.454695300261875,
    0.449580400261728,
    0.44443920026158,
    0.439268400261432,
    0.434065000261282,
    0.428825600261131,
    0.423546700260979,
    0.418224500260826,
    0.412855200260672,
    0.407434500260516,
    0.401957900260359,
    0.396420700260199,
    0.390817800260038,
    0.385143400259875,
    0.37939170025971,
    0.373556000259542,
    0.367629100259371,
    0.361603000259198,
    0.355469100259022,
    0.349217500258842,
    0.342837400258659,
    0.336316500258471,
    0.329641100258279,
    0.322795400258082,
    0.31576120025788,
    0.308517800257672,
    0.301040600257457,
    0.293300900257234,
    0.285264200257003,
    0.276889000256762,
    0.26812410025651,
    0.258905900256245,
    0.249152700255965,
    0.238758100255666,
    0.227578200255344,
    0.215411200254994,
    0.201959400254608,
    0.18675230025417,
    0.168969000253659,
    0.146932000253025,
    0.115996700252136,
    0.002477000249829
};

/**
 * Calculates the required zero-crossing delay to achieve a target relative power. 
 * @param power     The target relative power. (0.0 to 1.0)
 * @param frequency Mains frequency
 * @returns The calculated delay, in microseconds.
 **/
unsigned int calculateDelay(double relative_power, float frequency) {
    
    // Constrain input to range.
    if (relative_power <= 0) relative_power = 0.01;
    if (relative_power > 1)  relative_power = 1;

    uint8_t index = relative_power * 100 - 1;

    return 1000000 * (delay_lookup_table[index] / (2 * frequency));
}